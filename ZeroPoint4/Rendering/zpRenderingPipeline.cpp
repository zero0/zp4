#include "zpRendering.h"

const zp_char* g_comparisonFunc[] =
{
	"None",
	"Less",
	"LessEqual",
	"Equal",
	"NotEqual",
	"GreaterEqual",
	"Greater",
	"Always",
};
const zp_char* g_textureFilter[] =
{
	"Point",
	"Linear",
	"Anisotropic",
	"ComparisonPoint",
	"ComparisonLinear",
	"ComparisonAnisotropic",
};
const zp_char* g_textureWrap[] =
{
	"Repeat",
	"Mirror",
	"Clamp",
	"Border",
	"MirrorOnce",
};
const zp_char* g_cullMode[] =
{
	"None",
	"Front",
	"Back",
};
const zp_char* g_blend[] =
{
	"Zero",
	"One",
	"SrcColor",
	"InvSrcColor",
	"SrcAlpha",
	"InvSrcAlpha",
	"DestAlpha",
	"InvDestAlpha",
	"DestColor",
	"InvDestColor",
	"SrcAlphaSat",
	"BlendFactor",
	"Src1Color",
	"InvSrc1Color",
	"Src1Alpha",
	"InvSrc1Alpha",
};
const zp_char* g_blendOp[] =
{
	"Add",
	"Sub",
	"RevSub",
	"Min",
	"Max",
};
const zp_char* g_stencilOp[] =
{
	"Keep",
	"Zero",
	"Replace",
	"IncrSat",
	"DecrSat",
	"Invert",
	"Incr",
	"Decr",
};


#define ENUM_COUNT( e ) e##_count
#define ENUM_STR( e ) g_##e

template< typename T, zp_uint count >
zp_bool _strToEnum( const zp_char* (&e)[ count ], const zpBison::Value& str, T& t )
{
	zp_bool ok = false;

	if( str.isString() )
	{
		const zp_char* s = str.asCString();
		for( zp_uint i = 0; i < count; ++i )
		{
			if( zp_strcmp( s, e[ i ] ) == 0 )
			{
				t = (T)i;
				ok = true;
				break;
			}
		}
	}

	return ok;
}
zp_bool _valueToColor( const zpBison::Value& v, zpColor4f& c )
{
	zp_bool ok = false;

	if( !v.isEmpty() )
	{
		ok = true;
		if( v.isObject() )
		{
			switch( v.size() )
			{
			case 4:
				c.setAlpha( v[ "a" ].asFloat() );
			case 3:
				c.setRed( v[ "r" ].asFloat() );
				c.setGreen( v[ "g" ].asFloat() );
				c.setBlue( v[ "b" ].asFloat() );
				break;
			default:
				ok = false;
				break;
			}
		}
		else if( v.isArray() )
		{
			switch( v.size() )
			{
			case 4:
				c.setAlpha( v[ 3 ].asFloat() );
			case 3:
				c.setRed( v[ 0 ].asFloat() );
				c.setGreen( v[ 1 ].asFloat() );
				c.setBlue( v[ 2 ].asFloat() );
				break;
			default:
				ok = false;
				break;
			}
		}
		else
		{
			ok = false;
		}
	}

	return ok;
}

zpRenderingPipeline::zpRenderingPipeline()
	: m_engine()
	, m_screenshotType( ZP_SCREENSHOT_TYPE_NONE )
{}
zpRenderingPipeline::~zpRenderingPipeline()
{
	
}

const zpRenderingEngine* zpRenderingPipeline::getRenderingEngine() const
{
	return &m_engine;
}
zpRenderingEngine* zpRenderingPipeline::getRenderingEngine()
{
	return &m_engine;
}

void zpRenderingPipeline::initialize()
{
	m_engine.initialize();

	zpRasterStateDesc raster;
	raster.cullMode = ZP_CULL_MODE_BACK;
	raster.frontFace = ZP_FRONT_FACE_CW;
	raster.fillMode = ZP_FILL_MODE_SOLID;
	//raster.depthClipEnable = true;
	
	m_engine.createRasterState( m_raster, raster );

	zpBlendStateDesc blend;
	blend.renderTargets[0].enabled = true;
	blend.renderTargets[0].srcBlend = ZP_BLEND_SRC_ALPHA;
	blend.renderTargets[0].destBlend = ZP_BLEND_INV_SRC_ALPHA;
	blend.renderTargets[0].blendOp = ZP_BLEND_OP_ADD;

	m_engine.createBlendState( m_alphaBlend, blend );

	// create buffers
	m_engine.createBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_CAMERA ], ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpCameraBufferData ) );
	m_engine.createBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_FRAME ], ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpFrameBufferData ) );
	m_engine.createBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL ], ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpDrawCallBufferData ) );
	m_engine.createBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ], ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpLightBufferData ) );
	m_engine.createBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_SH ], ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpSphericalHarmonicsData ) );

	// fill free camera buffer
	m_cameras.resize( 8 );
	zpCamera* bc = m_cameras.begin();
	zpCamera* ec = m_cameras.end();
	for( ; bc != ec; ++bc )
	{
		m_freeCameras.pushBack( bc );
	}

	// fill free light buffer
	m_lights.resize( 64 );
	zpLightBufferData* bl = m_lights.begin();
	zpLightBufferData* el = m_lights.end();
	for( ; bl != el; ++bl )
	{
		m_freeLights.pushBack( bl );
	}

}
void zpRenderingPipeline::setup()
{
	zp_bool ok;

	ok = m_fontContent.getResource( "fonts/arial32.fontb", m_debugFont );
	ZP_ASSERT( ok, "" );

	const zpVector2i& size = m_engine.getScreenSize();

	zpViewport viewport;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.width = (zp_float)size.getX();
	viewport.height = (zp_float)size.getY();

	zpCamera* cam;
	cam = getCamera( ZP_CAMERA_TYPE_MAIN );
	cam->setActive( true );
	cam->setProjectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE );
	cam->setPosition( zpMath::Vector4( 20, 20, 20, 1 ) );
	cam->setLookAt( zpMath::Vector4( 0, 0, 0, 1 ) );
	cam->setUp( zpMath::Vector4( 0, 1, 0, 0 ) );
	cam->setAspectRatio( viewport.width / viewport.height );
	cam->setFovy( 45.0f );
	cam->setNearFar( 1.0f, 1000.0f );
	cam->setOrthoRect( zpRecti( 0, 0, size.getX(), size.getY() ) );
	cam->setViewport( viewport );
	cam->setClipRect( zpRecti( 0, 0, (zp_int)viewport.width, (zp_int)viewport.height ) );
	cam->setClearColor( zpColor4f( 0.39f, 0.58f, 0.93f, 1 ) );
	cam->setRenderLayers( 1 << 0 );
	cam->setStencilClear( 0 );
	cam->setDepthClear( 1.0f );
	cam->setClearMode( ZP_CAMERA_CLEAR_MODE_DEFAULT );
	cam->setRenderTarget( 0, m_engine.getBackBufferRenderTarget() );
	cam->setDepthStencilBuffer( m_engine.getBackBufferDepthStencilBuffer() );
	m_debugCamera = cam;

	cam = getCamera( ZP_CAMERA_TYPE_UI );
	cam->setActive( true );
	cam->setProjectionType( ZP_CAMERA_PROJECTION_ORTHO );
	cam->setPosition( zpMath::Vector4( 0, 0, -1, 1 ) );
	cam->setLookTo( zpMath::Vector4( 0, 0, 1, 0 ) );
	cam->setUp( zpMath::Vector4( 0, 1, 0, 0 ) );
	cam->setAspectRatio( viewport.width / viewport.height );
	cam->setFovy( 45.0f );
	cam->setNearFar( 1.0f, 100.0f );
	cam->setOrthoRect( zpRecti( 0, 0, size.getX(), size.getY() ) );
	cam->setViewport( viewport );
	cam->setClipRect( zpRecti( 0, 0, (zp_int)viewport.width, (zp_int)viewport.height ) );
	cam->setClearColor( zpColor4f( 0.39f, 0.58f, 0.93f, 1 ) );
	cam->setRenderLayers( 1 << 4 );
	cam->setStencilClear( 0 );
	cam->setDepthClear( 1.0f );
	cam->setClearMode( ZP_CAMERA_CLEAR_MODE_DEPTH );
	cam->setRenderTarget( 0, m_engine.getBackBufferRenderTarget() );
	cam->setDepthStencilBuffer( ZP_NULL );
	m_debugUICamera = cam;

	m_dirLight = getLight( ZP_LIGHT_TYPE_DIRECTIONAL );
	m_dirLight->direction = zpMath::Vector4Normalize3( zpMath::Vector4( 1, -1, 1, 0 ) );
	m_dirLight->color = zpColor4f( 1, 1, 1, 1 );
}
void zpRenderingPipeline::teardown()
{
	releaseLight( m_dirLight );

	releaseCamera( m_debugCamera );
	releaseCamera( m_debugUICamera );

	m_debugFont.release();

	m_engine.getImmediateRenderingContext()->finalizeCommands();

	m_engine.teardown();

}
void zpRenderingPipeline::shutdown()
{
	m_engine.destroyBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_CAMERA ] );
	m_engine.destroyBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_FRAME ] );
	m_engine.destroyBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL ] );
	m_engine.destroyBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ] );
	m_engine.destroyBuffer( m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_SH ] );

	m_engine.shutdown();
}

void zpRenderingPipeline::update()
{
}

void zpRenderingPipeline::beginFrame( zpRenderingContext* i, zpTime* time )
{
	i->clearState();

	i->setRasterState( &m_raster );

	zpFrameBufferData perFrameData;
	perFrameData.deltaTime = time->getDeltaSeconds();
	perFrameData.actualDeltaTime = time->getActualDeltaSeconds();
	perFrameData.fixedDeltaTime = 0.0f;
	perFrameData.timeFromStart = time->getSecondsSinceStart();

	i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_FRAME ], &perFrameData, sizeof( zpFrameBufferData ) );
	i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_PER_FRAME, &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_FRAME ] );

	// clear previous camera
	m_prevCamera = ZP_NULL;
}

void zpRenderingPipeline::submitRendering( zpRenderingContext* i )
{
	zpTexture* t = m_engine.getBackBufferRenderTarget();
	zpDepthStencilBuffer* d = m_engine.getBackBufferDepthStencilBuffer();
	
	// 1) fill buffers
	i->fillBuffers();

	zpCamera* camera;
	zpFixedArrayList< zpCamera*, 8 > activeCameras;

	// 3D Cameras
	{
		zpArrayList< zpCamera* >& cameraList = m_usedCameras[ ZP_CAMERA_TYPE_MAIN ];
		activeCameras.reset();

		zpCamera** b = cameraList.begin();
		zpCamera** e = cameraList.end();
		for( ; b != e; ++b )
		{
			camera = *b;

			// only process active cameras
			if( camera->isActive() )
			{
				activeCameras.pushBack( camera );
			}
		}

		activeCameras.sort( []( zpCamera* a, zpCamera* b ) {
			return a->getOrder() < b->getOrder();
		} );

		b = activeCameras.begin();
		e = activeCameras.end();
		for( ; b != e; ++b )
		{
			camera = *b;
			// 2) process commands, sorting, etc.
			i->preprocessCommands( camera, camera->getRenderLayers() );
			useCamera( i, camera, &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_CAMERA ] );

			// 3) render opaque commands
			i->setBlendState( ZP_NULL, ZP_NULL, 0xFFFFFFFF );
			processRenderingQueue( ZP_RENDERING_QUEUE_OPAQUE, true );
			processRenderingQueue( ZP_RENDERING_QUEUE_OPAQUE_DEBUG, false );

			// 4) render skybox commands
			processRenderingQueue( ZP_RENDERING_QUEUE_SKYBOX, false );

			// 5) render transparent commands
			i->setBlendState( &m_alphaBlend, ZP_NULL, 0xFFFFFFFF );
			processRenderingQueue( ZP_RENDERING_QUEUE_TRANSPARENT, true );
			processRenderingQueue( ZP_RENDERING_QUEUE_TRANSPARENT_DEBUG, false );

			// 6) render overlay commands
			processRenderingQueue( ZP_RENDERING_QUEUE_OVERLAY, false );
		}
	}

	// perform no UI screenshot
	if( m_screenshotType == ZP_SCREENSHOT_TYPE_NO_UI ) m_engine.performScreenshot();

	// 2D Cameras
	{
		zpArrayList< zpCamera* >& cameraList = m_usedCameras[ ZP_CAMERA_TYPE_UI ];
		activeCameras.reset();

		zpCamera** b = cameraList.begin();
		zpCamera** e = cameraList.end();
		for( ; b != e; ++b )
		{
			camera = *b;

			// only process active cameras
			if( camera->isActive() )
			{
				activeCameras.pushBack( camera );
			}
		}

		activeCameras.sort( []( zpCamera* a, zpCamera* b ) {
			return a->getOrder() < b->getOrder();
		} );

		b = activeCameras.begin();
		e = activeCameras.end();
		for( ; b != e; ++b )
		{
			i->preprocessCommands( camera, camera->getRenderLayers() );
			useCamera( i, camera, &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_CAMERA ] );

			processRenderingQueue( ZP_RENDERING_QUEUE_UI, false );
			processRenderingQueue( ZP_RENDERING_QUEUE_UI_DEBUG, false );
		}
	}

	// perform full screenshot
	if( m_screenshotType == ZP_SCREENSHOT_TYPE_ALL ) m_engine.performScreenshot();
}

void zpRenderingPipeline::endFrame( zpRenderingContext* i )
{
	i->finalizeCommands();
}
void zpRenderingPipeline::present()
{
	m_engine.present();

	switch( m_screenshotType )
	{
	case ZP_SCREENSHOT_TYPE_ALL:
	case ZP_SCREENSHOT_TYPE_NO_UI:
		m_screenshotType = ZP_SCREENSHOT_TYPE_PROCESSING;
		break;
	case ZP_SCREENSHOT_TYPE_PROCESSING:
		m_screenshotType = ZP_SCREENSHOT_TYPE_NONE;
		performScreenshot();
		break;
	}
}

zpMaterialContentManager* zpRenderingPipeline::getMaterialContentManager()
{
	return &m_materialContent;
}

zpShaderContentManager* zpRenderingPipeline::getShaderContentManager()
{
	return &m_shaderContent;
}
zpTextureContentManager* zpRenderingPipeline::getTextureContentManager()
{
	return &m_textureContent;
}
zpMeshContentManager* zpRenderingPipeline::getMeshContentManager()
{
	return &m_meshContent;
}
zpSkeletonContentManager* zpRenderingPipeline::getSkeletonContentManager()
{
	return &m_skeletonContent;
}
zpAnimationContentManager* zpRenderingPipeline::getAnimationContentManager()
{
	return &m_animationContent;
}
zpFontContentManager* zpRenderingPipeline::getFontContentManager()
{
	return &m_fontContent;
}

void zpRenderingPipeline::onFocusGained()
{

}
void zpRenderingPipeline::onFocusLost()
{

}

void zpRenderingPipeline::generateSamplerStateDesc( const zpBison::Value& sampler, zpSamplerStateDesc& outSamplerDesc )
{
	// start with default sampler state
	outSamplerDesc = zpSamplerStateDesc();

	if( !sampler.isEmpty() )
	{
		const zpBison::Value& cmpFuncValue = sampler[ "CmpFunc" ];
		const zpBison::Value& minFilterValue = sampler[ "MinFilter" ];
		const zpBison::Value& magFilterValue = sampler[ "MagFilter" ];
		const zpBison::Value& mipFilterValue = sampler[ "MipFilter" ];
		const zpBison::Value& texWrapUValue = sampler[ "TextureWrapU" ];
		const zpBison::Value& texWrapVValue = sampler[ "TextureWrapV" ];
		const zpBison::Value& texWrapWValue = sampler[ "TextureWrapW" ];
		const zpBison::Value& maxAnisotrpyValue = sampler[ "MaxAnisotrpy" ];
		const zpBison::Value& lodMinValue = sampler[ "LodMin" ];
		const zpBison::Value& lodMaxValue = sampler[ "LodMax" ];
		const zpBison::Value& lodBiasValue = sampler[ "LodBias" ];
		const zpBison::Value& borderColorValue = sampler[ "BorderColor" ];

		zpTextureWrap wrap;
		zpTextureFilter filter;
		zpComparisonFunc cmp;
		if( _strToEnum( g_comparisonFunc,  cmpFuncValue, cmp ) ) outSamplerDesc.cmpFunc = cmp;
		if( _strToEnum( g_textureFilter,   minFilterValue, filter ) ) outSamplerDesc.minFilter = filter;
		if( _strToEnum( g_textureFilter,   magFilterValue, filter ) ) outSamplerDesc.magFilter = filter;
		if( _strToEnum( g_textureFilter,   mipFilterValue, filter ) ) outSamplerDesc.mipFilter = filter;
		if( _strToEnum( g_textureWrap,     texWrapUValue, wrap ) ) outSamplerDesc.texWrapU = wrap;
		if( _strToEnum( g_textureWrap,     texWrapVValue, wrap ) ) outSamplerDesc.texWrapV = wrap;
		if( _strToEnum( g_textureWrap,     texWrapWValue, wrap ) ) outSamplerDesc.texWrapW = wrap;

		if( maxAnisotrpyValue.isIntegral() )
		{
			outSamplerDesc.maxAnisotrpy = (zp_byte)maxAnisotrpyValue.asInt();
		}

		if( lodMinValue.isFloat() )
		{
			outSamplerDesc.lodMin = lodMinValue.asFloat();
		}

		if( lodMaxValue.isFloat() )
		{
			outSamplerDesc.lodMax = lodMaxValue.asFloat();
		}

		if( lodBiasValue.isFloat() )
		{
			outSamplerDesc.lodBias = lodBiasValue.asFloat();
		}

		_valueToColor( borderColorValue, outSamplerDesc.borderColor );
	}
}
void zpRenderingPipeline::generateRasterStateDesc( const zpBison::Value& raster, zpRasterStateDesc& outRasterDesc )
{
	// start with default raster state
	outRasterDesc = zpRasterStateDesc();

	if( !raster.isEmpty() )
	{
		const zpBison::Value& cullModeValue = raster[ "CullMode" ];
		const zpBison::Value& fillModeSolidValue = raster[ "FillModeSolid" ];
		const zpBison::Value& frontFaceCWValue = raster[ "FrontFaceCW" ];
		const zpBison::Value& depthClipEnabledValue = raster[ "DepthClipEnabled" ];
		const zpBison::Value& scissorEnabledValue = raster[ "ScissorEnabled" ];
		const zpBison::Value& multisampleEnabledValue = raster[ "MultiSampleEnabled" ];
		const zpBison::Value& antialiasedLinedEnabledValue = raster[ "AntialiasedLinedEnabled" ];
		const zpBison::Value& depthBiasValue = raster[ "DepthBias" ];
		const zpBison::Value& depthBiasClampValue = raster[ "DepthBiasClamp" ];
		const zpBison::Value& slopeScaledDepthBiasValue = raster[ "SlopeScaledDepthBias" ];

		_strToEnum( g_cullMode, cullModeValue, outRasterDesc.cullMode );

		if( fillModeSolidValue.isBool() )
		{
			outRasterDesc.fillMode = fillModeSolidValue.asBool() ? ZP_FILL_MODE_SOLID : ZP_FILL_MODE_WIREFRAME;
		}

		if( frontFaceCWValue.isBool() )
		{
			outRasterDesc.frontFace = frontFaceCWValue.asBool() ? ZP_FRONT_FACE_CW : ZP_FRONT_FACE_CCW;
		}

		if( depthClipEnabledValue.isBool() )
		{
			outRasterDesc.depthClipEnable = depthClipEnabledValue.asBool();
		}

		if( scissorEnabledValue.isBool() )
		{
			outRasterDesc.scissorEnable = scissorEnabledValue.asBool();
		}

		if( multisampleEnabledValue.isBool() )
		{
			outRasterDesc.multisampleEnable = multisampleEnabledValue.asBool();
		}

		if( antialiasedLinedEnabledValue.isBool() )
		{
			outRasterDesc.antialiasedLineEnable = antialiasedLinedEnabledValue.asBool();
		}

		if( depthBiasValue.isIntegral() )
		{
			outRasterDesc.depthBias = depthBiasValue.asInt();
		}

		if( depthBiasClampValue.isFloat() )
		{
			outRasterDesc.depthBiasClamp = depthBiasClampValue.asFloat();
		}

		if( slopeScaledDepthBiasValue.isFloat() )
		{
			outRasterDesc.slopeScaledDepthBias = slopeScaledDepthBiasValue.asFloat();
		}
	}
}
void zpRenderingPipeline::generateBlendStateDesc( const zpBison::Value& blend, zpBlendStateDesc& outBlendStateDesc )
{
	outBlendStateDesc = zpBlendStateDesc();

	const zpBison::Value& alphaToConverage = blend[ "AlphaToCoverageEnabled" ];
	const zpBison::Value& independentBlend = blend[ "IndependentBlendEnabled" ];
	const zpBison::Value& targets = blend[ "RenderTargets" ];
	
	outBlendStateDesc.alphaToConverageEnabled = alphaToConverage.asBool();
	outBlendStateDesc.independentBlendEnabled = independentBlend.asBool();
	
	if( !targets.isEmpty() )
	{
		zp_uint count = targets.size();
		for( zp_uint b = 0; b < count; ++b )
		{
			const zpBison::Value& targetDesc = targets[ b ];
			zpBlendTargetDesc& target = outBlendStateDesc.renderTargets[ b ];

			const zpBison::Value& srcBlend = targetDesc[ "SrcBlend" ];
			const zpBison::Value& destBlend = targetDesc[ "DestBlend" ];
			const zpBison::Value& blendOp = targetDesc[ "BlendOp" ];
			const zpBison::Value& srcBlendAlpha = targetDesc[ "SrcBlendAlpha" ];
			const zpBison::Value& destBlendAlpha = targetDesc[ "DestBlendAlpha" ];
			const zpBison::Value& blendOpAlpha = targetDesc[ "BlendOpAlpha" ];

			// enable when defined
			target.enabled = true;

			_strToEnum( g_blend, srcBlend, target.srcBlend );
			_strToEnum( g_blend, destBlend, target.destBlend );
			_strToEnum( g_blendOp, blendOp, target.blendOp );

			_strToEnum( g_blend, srcBlendAlpha, target.srcBlendAlpha );
			_strToEnum( g_blend, destBlendAlpha, target.destBlendAlpha );
			_strToEnum( g_blendOp, blendOpAlpha, target.blendOpAlpha );

			// TODO: implement write mask
			target.renderTargetWriteMask = ZP_COLOR_WRITE_ENABLE_ALL;
		}
	}
}
void zpRenderingPipeline::generateDepthStencilStateDesc( const zpBison::Value& depthStencil, zpDepthStencilStateDesc& outDepthStencilStateDesc )
{
	outDepthStencilStateDesc = zpDepthStencilStateDesc();

	const zpBison::Value& depthEnabled = depthStencil[ "DepthEnabled" ];
	const zpBison::Value& depthWriteMaskAll = depthStencil[ "DepthWriteMaskAll" ];
	const zpBison::Value& depthFunc = depthStencil[ "DepthFunc" ];
	const zpBison::Value& stencilEnabled = depthStencil[ "StencilEnabled" ];
	const zpBison::Value& stencilReadMask = depthStencil[ "StencilReadMask" ];
	const zpBison::Value& stencilWriteMask = depthStencil[ "StencilWriteMask" ];
	const zpBison::Value& frontFace = depthStencil[ "FrontFace" ];
	const zpBison::Value& backFace = depthStencil[ "BackFace" ];

	if( depthEnabled.isIntegral() ) outDepthStencilStateDesc.depthEnabled = depthEnabled.asBool();
	if( depthWriteMaskAll.isIntegral() ) outDepthStencilStateDesc.depthWriteMaskAll = depthWriteMaskAll.asBool();

	_strToEnum( g_comparisonFunc, depthFunc, outDepthStencilStateDesc.depthFunc );

	if( stencilEnabled.isIntegral() ) outDepthStencilStateDesc.stencilEnabled = stencilEnabled.asBool();
	if( stencilReadMask.isIntegral() ) outDepthStencilStateDesc.stencilReadMask = (zp_byte)stencilReadMask.asInt();
	if( stencilWriteMask.isIntegral() ) outDepthStencilStateDesc.stencilWriteMask = (zp_byte)stencilWriteMask.asInt();

	if( !frontFace.isEmpty() )
	{
		_strToEnum( g_stencilOp,      frontFace[ "StencilFail" ],          outDepthStencilStateDesc.frontFace.stencilFail );
		_strToEnum( g_stencilOp,      frontFace[ "StencilPassDepthFail" ], outDepthStencilStateDesc.frontFace.stencilPassDepthFail );
		_strToEnum( g_stencilOp,      frontFace[ "StencilPassDepthPass" ], outDepthStencilStateDesc.frontFace.stencilPassDepthPass );
		_strToEnum( g_comparisonFunc, frontFace[ "StencilFunc" ],          outDepthStencilStateDesc.frontFace.stencilFunc );
	}
	
	if( !backFace.isEmpty() )
	{
		_strToEnum( g_stencilOp,      backFace[ "StencilFail" ],          outDepthStencilStateDesc.backFace.stencilFail );
		_strToEnum( g_stencilOp,      backFace[ "StencilPassDepthFail" ], outDepthStencilStateDesc.backFace.stencilPassDepthFail );
		_strToEnum( g_stencilOp,      backFace[ "StencilPassDepthPass" ], outDepthStencilStateDesc.backFace.stencilPassDepthPass );
		_strToEnum( g_comparisonFunc, backFace[ "StencilFunc" ],          outDepthStencilStateDesc.backFace.stencilFunc );
	}
}

zp_uint zpRenderingPipeline::getNumCameras( zpCameraType type ) const
{
	return m_usedCameras[ type ].size();
}
const zpArrayList< zpCamera* >& zpRenderingPipeline::getUsedCameras( zpCameraType type ) const
{
	return m_usedCameras[ type ];
}


zpCamera* zpRenderingPipeline::getCamera( zpCameraType type )
{
	ZP_ASSERT( !m_freeCameras.isEmpty(), "Ran out of cameras" );
	
	zpCamera* camera = m_freeCameras.back();
	m_freeCameras.popBack();
	
	m_usedCameras[ type ].pushBack( camera );
	camera->setCameraType( type );

	return camera;
}
void zpRenderingPipeline::releaseCamera( zpCamera* camera )
{
	zp_uint count = m_usedCameras[ camera->getCameraType() ].eraseAll( camera );

	ZP_UNUSED( count );
	ZP_ASSERT( count != 0, "Unknown camera being removed from queue" );

	m_freeCameras.pushBack( camera );
}

void zpRenderingPipeline::useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer )
{
	zpTexture* const* rts = camera->getRenderTargets();
	zpDepthStencilBuffer* d = camera->getDepthStencilBuffer();

	// set render targets for camera
	i->setRenderTarget( 0, camera->getNumRenderTargets(), rts, d );

	// clear camera
	const zpFlag8& clearMode = camera->getClearMode();
	if( clearMode.isAllMarked( ZP_CAMERA_CLEAR_MODE_BEFORE_RENDER ) )
	{
		if( clearMode.isAllMarked( ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL ) )
		{
			i->clearDepthStencilBuffer( d, camera->getDepthClear(), camera->getStencilClear() );
		}
		else if( clearMode.isAllMarked( ZP_CAMERA_CLEAR_MODE_DEPTH ) )
		{
			i->clearDepthBuffer( d, camera->getDepthClear() );
		}
		else if( clearMode.isAllMarked( ZP_CAMERA_CLEAR_MODE_STENCIL ) )
		{
			i->clearStencilBuffer( d, camera->getStencilClear() );
		}

		if( clearMode.isAllMarked( ZP_CAMERA_CLEAR_MODE_COLOR ) )
		{
			i->clearRenderTarget( rts[0], camera->getClearColor() );
		}
	}

	// if the camera needs to updated, update the camera data
	if( camera->update() || m_prevCamera != camera )
	{
		m_prevCamera = camera;
		i->update( cameraBuffer, &camera->getCameraBufferData(), sizeof( zpCameraBufferData ) );
	}

	// set camera constant buffer
	i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_CAMERA, cameraBuffer );

	// set viewport and scissor rect
	i->setViewport( camera->getViewport() );
	i->setScissorRect( camera->getClipRect() );
}

zp_bool zpRenderingPipeline::takeScreenshot( zpScreenshotType type, const zp_char* directoryPath, zp_long time )
{
	zp_bool ok = false;

	if( m_screenshotType == ZP_SCREENSHOT_TYPE_NONE )
	{
		ok = true;
		zpFixedStringBuffer< 255 > path;
		path << directoryPath << zpFile::sep << "Screenshot_" << time << ".tga";

		m_screenshotType = type;
		m_screenshotFilename = path.str();
	}

	return ok;
}

zpLightBufferData* zpRenderingPipeline::getLight( zpLightType type )
{
	ZP_ASSERT( !m_freeLights.isEmpty(), "" );

	zpLightBufferData* light = m_freeLights.back();
	m_freeLights.popBack();
	zp_zero_memory( light );

	m_usedLights[ type ].pushBack( light );
	light->type = type;

	return light;
}
void zpRenderingPipeline::releaseLight( zpLightBufferData* light )
{
	zp_int index = m_usedLights[ light->type ].indexOf( light );
	ZP_ASSERT( index != -1, "" );

	m_usedLights[ light->type ].erase( index );

	m_freeLights.pushBack( light );
}

void zpRenderingPipeline::processRenderingQueue( zpRenderingQueue layer, zp_bool useLighting )
{
	zpRenderingContext* i = m_engine.getImmediateRenderingContext();

	const zpArrayList< zpRenderingCommand* >& queue = i->getFilteredCommands( layer );

	//m_numTotalDrawCommands += queue.size();

	const zpRenderingCommand* const* cmd = queue.begin();
	const zpRenderingCommand* const* end = queue.end();
	
	if( cmd != end )
	{
		i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL, &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL ] );
	
		if( useLighting )
		{
			i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_LIGHT, &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ] );

			zpDrawCallBufferData drawCallData;
			for( ; cmd != end; ++cmd )
			{
				drawCallData.world = (*cmd)->matrix;
				i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL ], &drawCallData, sizeof( zpDrawCallBufferData ) );

				// base pass with 1 directional
				// additional passes with each other light
				zpLightBufferData** b, **e;

				// directional light base pass
				b = m_usedLights[ ZP_LIGHT_TYPE_DIRECTIONAL ].begin();
				e = m_usedLights[ ZP_LIGHT_TYPE_DIRECTIONAL ].end();
				if( b != e )
				{
					i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ], *b, sizeof( zpLightBufferData ) );
					i->processCommand( *cmd );

					for( ++b; b != e; ++b )
					{
						i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ], *b, sizeof( zpLightBufferData ) );
						i->processCommand( *cmd );
					}
				}
				// no directional lights, just render normally
				else
				{
					i->processCommand( *cmd );
				}

				zpBoundingSphere lightSphere;
				b = m_usedLights[ ZP_LIGHT_TYPE_POINT ].begin();
				e = m_usedLights[ ZP_LIGHT_TYPE_POINT ].end();
				for( ; b != e; ++b )
				{
					zpLightBufferData* data = *b;

					lightSphere.setCenter( data->position );
					lightSphere.setRadius( zpMath::Scalar( data->radius ) );
					if( ZP_IS_COLLISION( (*cmd)->boundingBox, lightSphere ) )
					{
						i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ], *b, sizeof( zpLightBufferData ) );
						i->processCommand( *cmd );
					}
				}

				b = m_usedLights[ ZP_LIGHT_TYPE_SPOT ].begin();
				e = m_usedLights[ ZP_LIGHT_TYPE_SPOT ].end();
				for( ; b != e; ++b )
				{
					i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_LIGHT ], *b, sizeof( zpLightBufferData ) );
					i->processCommand( *cmd );
				}

				//m_numTotalVerticies += (*cmd)->vertexCount;
			}
		}
		else
		{
			zpDrawCallBufferData drawCallData;
			for( ; cmd != end; ++cmd )
			{
				drawCallData.world = (*cmd)->matrix;
				i->update( &m_constantBuffers[ ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL ], &drawCallData, sizeof( zpDrawCallBufferData ) );

				i->processCommand( *cmd );
			}
		}
	}
}

zp_bool zpRenderingPipeline::performScreenshot()
{
	const zpVector2i& screenSize = m_engine.getScreenSize();

	zp_bool ok;
	zpDataBuffer data;
	zp_uint imageSize = screenSize.getX() * screenSize.getY() * 4;
	data.reserve( imageSize + 50 );

	const zp_byte TGAHeaderColor[12] =
	{
		0,// 0 ID length = no id
		0,// 1 color map type = no color map
		2,// 2 image type = uncompressed true color
		0, 0, 0, 0, 0,// color map spec = empty
		0, 0,  // x origin of image 
		0, 0   // y origin of image
	};

	data.writeBulk( TGAHeaderColor, sizeof( TGAHeaderColor ) );
	data.write< zp_short >( screenSize.getX() );
	data.write< zp_short >( screenSize.getY() );
	data.write< zp_byte >( 32 );
	data.write< zp_byte >( 0x24 );

	zp_uint index = data.size();

	ok = m_engine.takeScreenshot( data );

	zp_byte* d = data.getData() + index;

	// swap RGB to BGR
	for( zp_uint i = 0; i < imageSize; i += 4 )
	{
		d[ i ] ^= d[ i + 2 ] ^= d[ i ] ^= d[ i + 2 ];
		d[ i + 3 ] = 0xFF;
	}

	zpFile screenshot( m_screenshotFilename.str() );
	if( screenshot.open( ZP_FILE_MODE_BINARY_TRUNCATE_WRITE ) )
	{
		screenshot.writeBuffer( data );
		screenshot.close();

		ok = true;
	}

	return ok;
}
