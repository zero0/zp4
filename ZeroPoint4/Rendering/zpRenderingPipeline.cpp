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
	: m_engine( zpRenderingFactory::getRenderingEngine() )
{}
zpRenderingPipeline::~zpRenderingPipeline()
{
	m_engine = ZP_NULL;
	zpRenderingFactory::destroyRenderingEngine();
}

zpRenderingEngine* zpRenderingPipeline::getRenderingEngine() const
{
	return m_engine;
}

void zpRenderingPipeline::initialize()
{
	zp_bool ok;

	ok = m_fontContent.getResource( "fonts/arial32.fontb", m_debugFont );
	ZP_ASSERT( ok, "" );

	const zpVector2i& size = m_engine->getScreenSize();

	zpViewport viewport;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.width = (zp_float)size.getX();
	viewport.height = (zp_float)size.getY();

	zpRasterStateDesc raster;
	raster.cullMode = ZP_CULL_MODE_BACK;
	raster.frontFace = ZP_FRONT_FACE_CW;
	raster.fillMode = ZP_FILL_MODE_SOLID;
	//raster.depthClipEnable = true;
	
	m_engine->createRasterState( m_raster, raster );

	zpBlendStateDesc blend;
	blend.renderTargets[0].enabled = true;
	blend.renderTargets[0].srcBlend = ZP_BLEND_SRC_ALPHA;
	blend.renderTargets[0].destBlend = ZP_BLEND_INV_SRC_ALPHA;
	blend.renderTargets[0].blendOp = ZP_BLEND_OP_ADD;

	m_engine->createBlendState( m_alphaBlend, blend );

	m_engine->createBuffer( m_cameraBuffer, ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpCameraBufferData ) );
	m_engine->createBuffer( m_perFrameBuffer, ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpFrameBufferData ) );

	m_cameras.resize( zpCameraType_Count );

	m_cameraStack.reserve( zpCameraType_Count );
	for( zp_uint i = 0; i < zpCameraType_Count; ++i )
	{
		m_cameraStack.pushBackEmpty().reserve( 5 );
	}

	zpCamera* cam;
	cam = getCamera( ZP_CAMERA_TYPE_MAIN );
	cam->setProjectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE );
	cam->setPosition( zpVector4f( 10, 10, 10, 1 ) );
	cam->setLookAt( zpVector4f( 0, 0, 0, 1 ) );
	cam->setUp( zpVector4f( 0, 1, 0, 0 ) );
	cam->setAspectRatio( viewport.width / viewport.height );
	cam->setFovy( 45.0f );
	cam->setNearFar( 1.0f, 1000.0f );
	cam->setOrthoRect( zpRecti( 0, 0, size.getX(), size.getY() ) );
	cam->setViewport( viewport );
	cam->setClipRect( zpRecti( 0, 0, (zp_int)viewport.width, (zp_int)viewport.height ) );
	cam->setClearColor( zpColor4f( 1, 0, 0, 1 ) );
	cam->setRenderLayers( 1 << 0 );
	cam->setStencilClear( 0 );
	cam->setDepthClear( 1.0f );
	cam->setClearMode( ZP_CAMERA_CLEAR_MODE_DEFAULT );
	cam->setRenderTarget( 0, m_engine->getBackBufferRenderTarget() );
	cam->setDepthStencilBuffer( m_engine->getBackBufferDepthStencilBuffer() );

	cam = getCamera( ZP_CAMERA_TYPE_UI );
	cam->setProjectionType( ZP_CAMERA_PROJECTION_ORTHO );
	cam->setPosition( zpVector4f( 0, 0, -1, 1 ) );
	cam->setLookTo( zpVector4f( 0, 0, 1, 0 ) );
	cam->setUp( zpVector4f( 0, 1, 0, 0 ) );
	cam->setAspectRatio( viewport.width / viewport.height );
	cam->setFovy( 45.0f );
	cam->setNearFar( 1.0f, 100.0f );
	cam->setOrthoRect( zpRecti( 0, 0, size.getX(), size.getY() ) );
	cam->setViewport( viewport );
	cam->setClipRect( zpRecti( 0, 0, (zp_int)viewport.width, (zp_int)viewport.height ) );
	cam->setClearColor( zpColor4f( 1, 0, 0, 1 ) );
	cam->setRenderLayers( 1 << 4 );
	cam->setStencilClear( 0 );
	cam->setDepthClear( 1.0f );
	cam->setClearMode( ZP_CAMERA_CLEAR_MODE_NONE );
	cam->setRenderTarget( 0, m_engine->getBackBufferRenderTarget() );
	cam->setDepthStencilBuffer( ZP_NULL );
}
void zpRenderingPipeline::destroy()
{
	m_debugFont.release();

	m_engine->destroyBuffer( m_cameraBuffer );
	m_engine->destroyBuffer( m_perFrameBuffer );

	m_engine->destroy();
}

void zpRenderingPipeline::update()
{
	zp_bool shouldPop;
	for( zp_uint i = 0; i < zpCameraType_Count; ++i )
	{
		zpCamera* camera = &m_cameras[ i ];
		zpArrayList< zpCameraState* >& stack = m_cameraStack[ i ];
		
		if( !stack.isEmpty() )
		{
			shouldPop = stack.back()->onUpdate( m_application, camera );
			if( shouldPop )
			{
				popCameraState( (zpCameraType)i );
			}
		}
	}
}

void zpRenderingPipeline::beginFrame( zpRenderingContext* i )
{
	i->clearState();

	i->setRasterState( &m_raster );

	zpTime* time = zpTime::getInstance();
	zpFrameBufferData perFrameData;
	perFrameData.deltaTime = time->getDeltaSeconds();
	perFrameData.actualDeltaTime = time->getActualDeltaSeconds();
	perFrameData.fixedDeltaTime = 0.0f;
	perFrameData.timeFromStart = time->getSecondsSinceStart();

	i->update( &m_perFrameBuffer, &perFrameData, sizeof( zpFrameBufferData ) );
	i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_PER_FRAME, &m_perFrameBuffer );

	// clear previous camera
	m_prevCamera = ZP_NULL;
}

void zpRenderingPipeline::submitRendering( zpRenderingContext* i )
{
	zpTexture* t = m_engine->getBackBufferRenderTarget();
	zpDepthStencilBuffer* d = m_engine->getBackBufferDepthStencilBuffer();
	
	// 1) fill buffers
	i->fillBuffers();

	zpCamera* cam;
	// 2) process commands, sorting, etc.
	cam = getCamera( ZP_CAMERA_TYPE_MAIN );
	i->preprocessCommands( cam, cam->getRenderLayers() );
	useCamera( i, cam, &m_cameraBuffer );

	// 3) render opaque commands
	i->setBlendState( ZP_NULL, ZP_NULL, 0xFFFFFFFF );
	i->processCommands( ZP_RENDERING_QUEUE_OPAQUE );
	i->processCommands( ZP_RENDERING_QUEUE_OPAQUE_DEBUG );

	// 4) render skybox commands
	i->processCommands( ZP_RENDERING_QUEUE_SKYBOX );

	// 5) render transparent commands
	i->setBlendState( &m_alphaBlend, ZP_NULL, 0xFFFFFFFF );
	i->processCommands( ZP_RENDERING_QUEUE_TRANSPARENT );
	i->processCommands( ZP_RENDERING_QUEUE_TRANSPARENT_DEBUG );

	// 6) render overlay commands
	i->processCommands( ZP_RENDERING_QUEUE_OVERLAY );

	// 7) render UI commands
	cam = getCamera( ZP_CAMERA_TYPE_UI );
	i->preprocessCommands( cam, cam->getRenderLayers() );
	useCamera( i, cam, &m_cameraBuffer );

	i->processCommands( ZP_RENDERING_QUEUE_UI );
	i->processCommands( ZP_RENDERING_QUEUE_UI_DEBUG );
}

void zpRenderingPipeline::endFrame( zpRenderingContext* i )
{
	i->finalizeCommands();
}
void zpRenderingPipeline::present()
{
	m_engine->present();
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

zpCamera* zpRenderingPipeline::getCamera( zpCameraType type )
{
	return &m_cameras[ type ];
}
void zpRenderingPipeline::useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer )
{
	zpTexture* const* rts = camera->getRenderTargets();
	zpDepthStencilBuffer* d = camera->getDepthStencilBuffer();

	i->setRenderTarget( 0, camera->getNumRenderTargets(), rts, d );
	if( camera->getClearMode().isAllMarked( ZP_CAMERA_CLEAR_MODE_BEFORE_RENDER ) )
	{
		if( camera->getClearMode().isAllMarked( ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL ) )
		{
			i->clearDepthStencilBuffer( d, camera->getDepthClear(), camera->getStencilClear() );
		}
		else if( camera->getClearMode().isAllMarked( ZP_CAMERA_CLEAR_MODE_DEPTH ) )
		{
			i->clearDepthBuffer( d, camera->getDepthClear() );
		}
		else if( camera->getClearMode().isAllMarked( ZP_CAMERA_CLEAR_MODE_STENCIL ) )
		{
			i->clearStencilBuffer( d, camera->getStencilClear() );
		}

		if( camera->getClearMode().isAllMarked( ZP_CAMERA_CLEAR_MODE_COLOR ) )
		{
			i->clearRenderTarget( rts[0], camera->getClearColor() );
		}
	}

	if( camera->update() || m_prevCamera != camera )
	{
		m_prevCamera = camera;
		i->update( cameraBuffer, &camera->getCameraBufferData(), sizeof( zpCameraBufferData ) );
	}
	i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_CAMERA, cameraBuffer );

	i->setViewport( camera->getViewport() );
	i->setScissorRect( camera->getClipRect() );
}

void zpRenderingPipeline::pushCameraState( zpCameraType type, zpCameraState* state )
{
	if( state != ZP_NULL )
	{
		zpArrayList< zpCameraState* >& stack = m_cameraStack[ type ];
		zpCamera* camera = &m_cameras[ type ];

		if( !stack.isEmpty() )
		{
			stack.back()->onLeave( camera );
		}

		m_cameraStack[ type ].pushBackEmpty() = state;

		state->onEnter( camera );
	}
}
void zpRenderingPipeline::popCameraState( zpCameraType type )
{
	zpArrayList< zpCameraState* >& stack = m_cameraStack[ type ];

	if( !stack.isEmpty() )
	{
		zpCamera* camera = &m_cameras[ type ];

		zpCameraState* old = stack.back();
		old->onLeave( camera );
		delete old;
		
		stack.popBack();

		if( !stack.isEmpty() )
		{
			stack.back()->onEnter( camera );
		}
	}
}
