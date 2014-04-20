#include "zpRendering.h"

const zp_char* g_cmpFuns[] =
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
	ok = m_materialContent.getResource( "materials/fullscreenNoAlpha.materialb", m_mat );
	ZP_ASSERT( ok, "" );
	
	ok = m_meshContent.getResource( "meshes/cube.meshb", m_mesh );
	ZP_ASSERT( ok, "" );

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

	m_engine->createBlendState( m_blend, blend );

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
	cam->setPosition( zpVector4f( -10, 10, 10, 1 ) );
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
	m_mat.release();
	m_mesh.release();
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

	// 0) render things outside this loop
	//i->beginDrawImmediate( ZP_RENDERING_LAYER_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_UV, &m_mat );
	////i->addVertex( zpVector4f( 0, 0, 0, 1 ), zpVector2f( 0, 1 ) );
	////i->addVertex( zpVector4f( 0, 1, 0, 1 ), zpVector2f( 0, 0 ) );
	////i->addVertex( zpVector4f( 1, 1, 0, 1 ), zpVector2f( 1, 0 ) );
	////i->addVertex( zpVector4f( 1, 0, 0, 1 ), zpVector2f( 1, 1 ) );
	////i->addQuadIndex( 0, 1, 2, 3 );
	//
	//i->addVertex( zpVector4f( -1, -1, 0, 1 ), zpVector2f( 0, 1 ) );
	//i->addVertex( zpVector4f( -1,  0, 0, 1 ), zpVector2f( 0, 0 ) );
	//i->addVertex( zpVector4f(  0,  0, 0, 1 ), zpVector2f( 1, 0 ) );
	//i->addVertex( zpVector4f(  0, -1, 0, 1 ), zpVector2f( 1, 1 ) );
	//i->addQuadIndex( 0, 1, 2, 3 );
	//
	////i->addQuad(
	////	zpVector4f( -1, -1, 0, 1 ), zpVector2f( 0, 1 ),
	////	zpVector4f( -1,  0, 0, 1 ), zpVector2f( 0, 0 ),
	////	zpVector4f(  0,  0, 0, 1 ), zpVector2f( 1, 0 ),
	////	zpVector4f(  0, -1, 0, 1 ), zpVector2f( 1, 1 )
	////	);
	//
	//i->addQuad(
	//	zpVector4f( 0, 0, 0, 1 ), zpVector2f( 0, 1 ),
	//	zpVector4f( 0, 1, 0, 1 ), zpVector2f( 0, 0 ),
	//	zpVector4f( 1, 1, 0, 1 ), zpVector2f( 1, 0 ),
	//	zpVector4f( 1, 0, 0, 1 ), zpVector2f( 1, 1 )
	//	);
	//i->endDrawImmediate();
	//
	//i->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_UV, &m_mat );
	//i->addQuad(
	//	zpVector4f( -0.5f, -0.5, 0, 1 ), zpVector2f( 0, 1 ),
	//	zpVector4f( -0.5f,  0.5, 0, 1 ), zpVector2f( 0, 0 ),
	//	zpVector4f(  0.5f,  0.5, 0, 1 ), zpVector2f( 1, 0 ),
	//	zpVector4f(  0.5f, -0.5, 0, 1 ), zpVector2f( 1, 1 )
	//	);
	//i->endDrawImmediate();
	//
	//zpMatrix4f m;
	//m.setIdentity();
	//
	//i->drawMesh( 1 << 0, ZP_RENDERING_QUEUE_OPAQUE, &m_mesh, m );

	i->beginDrawFont( 1 << 4, ZP_RENDERING_QUEUE_UI, &m_debugFont );
	i->addText( "Hello Zero Point Text", 20, zpVector2f( 10, 10 ), ZP_FONT_ALIGNMENT_LEFT, zpColor4f( 1, 1, 1, 1 ), zpColor4f( 0.8f, 0.8f, 0.8f, 1 ) );
	i->addText( "Hello Zero Point Text", 30, zpVector2f( 40, 50 ), ZP_FONT_ALIGNMENT_LEFT, zpColor4f( 1, 1, 1, 1 ), zpColor4f( 0.8f, 0.8f, 0.8f, 1 ) );
	i->addText( "Hello Zero Point Text", 40, zpVector2f( 90, 90 ), ZP_FONT_ALIGNMENT_LEFT, zpColor4f( 1, 1, 1, 1 ), zpColor4f( 0.8f, 0.8f, 0.8f, 1 ) );
	i->endDrawFont();
	
	// 1) fill buffers
	i->fillBuffers();

	zpCamera* cam;
	// 2) process commands, sorting, etc.
	cam = getCamera( ZP_CAMERA_TYPE_MAIN );
	i->preprocessCommands( cam, cam->getRenderLayers() );

	// 3) actually render commands
	useCamera( i, cam, &m_cameraBuffer );

	i->setBlendState( ZP_NULL, ZP_NULL, 0xFFFFFFFF );
	i->processCommands( ZP_RENDERING_QUEUE_OPAQUE );
	i->processCommands( ZP_RENDERING_QUEUE_OPAQUE_DEBUG );

	i->processCommands( ZP_RENDERING_QUEUE_SKYBOX );

	i->setBlendState( &m_blend, ZP_NULL, 0xFFFFFFFF );
	i->processCommands( ZP_RENDERING_QUEUE_TRANSPARENT );
	i->processCommands( ZP_RENDERING_QUEUE_TRANSPARENT_DEBUG );

	i->processCommands( ZP_RENDERING_QUEUE_OVERLAY );

	cam = getCamera( ZP_CAMERA_TYPE_UI );
	i->preprocessCommands( cam, cam->getRenderLayers() );
	useCamera( i, cam, &m_cameraBuffer );

	i->processCommands( ZP_RENDERING_QUEUE_UI );
	i->processCommands( ZP_RENDERING_QUEUE_UI_DEBUG );

	
	//cam = getCamera( ZP_CAMERA_TYPE_UI );
	//
	//// 2) process commands, sorting, etc.
	//i->preprocessCommands( cam, cam->getRenderLayers() );
	//
	//// 3) actually render commands
	//useCamera( i, cam, &m_cameraBuffer );
	//
	//i->processCommands( ZP_RENDERING_QUEUE_OPAQUE );
	//i->processCommands( ZP_RENDERING_QUEUE_OPAQUE_DEBUG );
	//
	//i->processCommands( ZP_RENDERING_QUEUE_SKYBOX );
	//
	//i->processCommands( ZP_RENDERING_QUEUE_TRANSPARENT );
	//i->processCommands( ZP_RENDERING_QUEUE_TRANSPARENT_DEBUG );
	//
	//i->processCommands( ZP_RENDERING_QUEUE_OVERLAY );
	//
	//i->processCommands( ZP_RENDERING_QUEUE_UI );
	//i->processCommands( ZP_RENDERING_QUEUE_UI_DEBUG );
	
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

		if( cmpFuncValue.isString() )
		{
			const zp_char* cmpFunc = cmpFuncValue.asCString();
			for( zp_uint i = 0; i < zpComparisonFunc_Count; ++i )
			{
				if( zp_strcmp( cmpFunc, g_cmpFuns[ i ] ) )
				{
					outSamplerDesc.cmpFunc = (zpComparisonFunc)i;
					break;
				}
			}
		}

		if( minFilterValue.isString() )
		{
			const zp_char* minFilter = minFilterValue.asCString();
			for( zp_uint i = 0; i < zpTextureFilter_Count; ++i )
			{
				if( zp_strcmp( minFilter, g_textureFilter[ i ] ) == 0 )
				{
					outSamplerDesc.minFilter = (zpTextureFilter)i;
					break;
				}
			}
		}

		if( magFilterValue.isString() )
		{
			const zp_char* magFilter = magFilterValue.asCString();
			for( zp_uint i = 0; i < zpTextureFilter_Count; ++i )
			{
				if( zp_strcmp( magFilter, g_textureFilter[ i ] ) == 0 )
				{
					outSamplerDesc.magFilter = (zpTextureFilter)i;
					break;
				}
			}
		}

		if( mipFilterValue.isString() )
		{
			const zp_char* mipFilter = mipFilterValue.asCString();
			for( zp_uint i = 0; i < zpTextureFilter_Count; ++i )
			{
				if( zp_strcmp( mipFilter, g_textureFilter[ i ] ) == 0 )
				{
					outSamplerDesc.mipFilter = (zpTextureFilter)i;
					break;
				}
			}
		}

		if( texWrapUValue.isString() )
		{
			const zp_char* texWrapU = texWrapUValue.asCString();
			for( zp_uint i = 0; i < zpTextureWrap_Count; ++i )
			{
				if( zp_strcmp( texWrapU, g_textureWrap[ i ] ) == 0 )
				{
					outSamplerDesc.texWrapU = (zpTextureWrap)i;
					break;
				}
			}
		}

		if( texWrapUValue.isString() )
		{
			const zp_char* texWrapV = texWrapVValue.asCString();
			for( zp_uint i = 0; i < zpTextureWrap_Count; ++i )
			{
				if( zp_strcmp( texWrapV, g_textureWrap[ i ] ) == 0 )
				{
					outSamplerDesc.texWrapV = (zpTextureWrap)i;
					break;
				}
			}
		}

		if( texWrapWValue.isString() )
		{
			const zp_char* texWrapW = texWrapWValue.asCString();
			for( zp_uint i = 0; i < zpTextureWrap_Count; ++i )
			{
				if( zp_strcmp( texWrapW, g_textureWrap[ i ] ) == 0 )
				{
					outSamplerDesc.texWrapU = (zpTextureWrap)i;
					break;
				}
			}
		}

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

		zpColor4f c;
		if( borderColorValue.isObject() )
		{
			switch( borderColorValue.size() )
			{
			case 4:
				c.setAlpha( borderColorValue[ "a" ].asFloat() );
			case 3:
				c.setRed( borderColorValue[ "r" ].asFloat() );
				c.setGreen( borderColorValue[ "g" ].asFloat() );
				c.setBlue( borderColorValue[ "b" ].asFloat() );
				break;
			default:
				break;
			}
		}
		else if( borderColorValue.isArray() )
		{
			switch( borderColorValue.size() )
			{
			case 4:
				c.setAlpha( borderColorValue[ 3 ].asFloat() );
			case 3:
				c.setRed( borderColorValue[ 0 ].asFloat() );
				c.setGreen( borderColorValue[ 1 ].asFloat() );
				c.setBlue( borderColorValue[ 2 ].asFloat() );
				break;
			default:
				break;
			}
		}
		outSamplerDesc.borderColor = c;
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

		if( cullModeValue.isString() )
		{
			const zp_char* cullMode = cullModeValue.asCString();
			for( zp_uint i = 0; i < zpCullMode_Count; ++i )
			{
				if( zp_strcmp( cullMode, g_cullMode[ i ] ) == 0 )
				{
					outRasterDesc.cullMode = (zpCullMode)i;
					break;
				}
			}
		}

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
