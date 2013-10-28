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
	m_materialContent.getResource( "materials/fullscreenNoAlpha.materialb", m_mat );
	
	zpTextureResourceInstance t;
	m_textureContent.getResource( "textures/uv_checker_large.png", t );

	ok = m_meshContent.getResource( "meshes/cube.meshb", m_mesh );
	ZP_ASSERT( ok, "" );

	m_cameras.resize( ZP_RENDERING_MAX_CAMERAS );

	const zpVector2i& size = m_engine->getScreenSize();

	zpViewport viewport;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.width = (zp_float)size.getX();
	viewport.height = (zp_float)size.getY();

	zpRasterStateDesc raster;
	raster.cullMode = ZP_CULL_MODE_BACK;
	raster.frontFace = ZP_FRONT_FACE_CW;
	//raster.fillMode = ZP_FILL_MODE_WIREFRAME;
	//raster.depthClipEnable = true;
	
	m_engine->createRasterState( m_raster, raster );

	//m_cameraBuffer = m_engine->createBuffer( ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpCameraBufferData ) );
	m_engine->createBuffer( m_cameraBuffer, ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, sizeof( zpCameraBufferData ) );

	zpCamera* cam = getCamera( 0 );
	cam->setProjectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE );
	cam->setPosition( zpVector4f( 1, 1, 1, 1 ) );
	cam->setLookAt( zpVector4f( 0, 0, 0, 1 ) );
	cam->setUp( zpVector4f( 0, 1, 0, 0 ) );
	cam->setAspectRatio( viewport.width / viewport.height );
	cam->setFovy( 45.0f );
	cam->setNearFar( 1.0f, 100.0f );
	cam->setOrthoRect( zpRecti( 0, 0, size.getX(), size.getY() ) );
	cam->setViewport( viewport );
	cam->setClipRect( zpRecti( 0, 0, (zp_int)viewport.width, (zp_int)viewport.height ) );
}
void zpRenderingPipeline::destroy()
{
	m_mat.release();
	m_mesh.release();
}

void zpRenderingPipeline::beginFrame()
{
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();
	i->clearState();

	i->setRasterState( &m_raster );
}

void zpRenderingPipeline::submitRendering()
{
	zpTexture* t = m_engine->getBackBufferRenderTarget();
	zpDepthStencilBuffer* d = m_engine->getBackBufferDepthStencilBuffer();
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();

	// 0) render things outside this loop
	i->beginDrawImmediate( ZP_RENDERING_LAYER_UI_OPAQUE, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_UV, &m_mat );
	//i->addQuad(
		i->addVertex( zpVector4f( 0, 0, 0, 1 ), zpVector2f( 0, 1 ) );
		i->addVertex( zpVector4f( 0, 1, 0, 1 ), zpVector2f( 0, 0 ) );
		i->addVertex( zpVector4f( 1, 1, 0, 1 ), zpVector2f( 1, 0 ) );
		i->addVertex( zpVector4f( 1, 0, 0, 1 ), zpVector2f( 1, 1 ) );
		i->addQuadIndex( 0, 1, 2, 3 );

		i->addVertex( zpVector4f( -1, -1, 0, 1 ), zpVector2f( 0, 1 ) );
		i->addVertex( zpVector4f( -1,  0, 0, 1 ), zpVector2f( 0, 0 ) );
		i->addVertex( zpVector4f(  0,  0, 0, 1 ), zpVector2f( 1, 0 ) );
		i->addVertex( zpVector4f(  0, -1, 0, 1 ), zpVector2f( 1, 1 ) );
		i->addQuadIndex( 4, 5, 6, 7 );

		//i->addQuad(
		//	zpVector4f( -1, 0, 0, 1 ), zpVector2f( 0, 1 ),
		//	zpVector4f( -1, 1, 0, 1 ), zpVector2f( 0, 0 ),
		//	zpVector4f(  0, 1, 0, 1 ), zpVector2f( 1, 0 ),
		//	zpVector4f(  0, 0, 0, 1 ), zpVector2f( 1, 1 )
		//	);
	//	);
	i->endDrawImmediate();

	i->drawMesh( ZP_RENDERING_LAYER_OPAQUE, &m_mesh, zpVector4f( 0 ) );
	
	// 1) fill buffers
	i->fillBuffers();

	zpCamera* cam = getCamera( 0 );

	// 2) process commands, sorting, etc.
	i->preprocessCommands( cam );

	// 3) actually render commands
	i->setRenderTarget( 0, 1, &t, d );
	i->clearDepthStencilBuffer( d, 1.0f, 0 );
	i->clearRenderTarget( t, zpColor4f( 1, 0, 0, 1 ) );
	//i->setViewport( m_viewport );
	//i->setScissorRect( zpRecti( 0, 0, (zp_int)m_viewport.width, (zp_int)m_viewport.height ) );
	useCamera( i, cam, &m_cameraBuffer );

	i->processCommands( ZP_RENDERING_LAYER_OPAQUE );

	i->processCommands( ZP_RENDERING_LAYER_UI_OPAQUE );
}
void zpRenderingPipeline::submitDebugRendering()
{

}

void zpRenderingPipeline::endFrame()
{
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();
	i->finalizeCommands();

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
		const zpBison::Value cmpFuncValue = sampler[ "CmpFunc" ];
		const zpBison::Value minFilterValue = sampler[ "MinFilter" ];
		const zpBison::Value magFilterValue = sampler[ "MagFilter" ];
		const zpBison::Value mipFilterValue = sampler[ "MipFilter" ];
		const zpBison::Value texWrapUValue = sampler[ "TextureWrapU" ];
		const zpBison::Value texWrapVValue = sampler[ "TextureWrapV" ];
		const zpBison::Value texWrapWValue = sampler[ "TextureWrapW" ];
		const zpBison::Value maxAnisotrpyValue = sampler[ "MaxAnisotrpy" ];
		const zpBison::Value lodMinValue = sampler[ "LodMin" ];
		const zpBison::Value lodMaxValue = sampler[ "LodMax" ];
		const zpBison::Value lodBiasValue = sampler[ "LodBias" ];
		const zpBison::Value borderColorValue = sampler[ "BorderColor" ];

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
		const zpBison::Value cullModeValue = raster[ "CullMode" ];
		const zpBison::Value fillModeSolidValue = raster[ "FillModeSolid" ];
		const zpBison::Value frontFaceCWValue = raster[ "FrontFaceCW" ];
		const zpBison::Value depthClipEnabledValue = raster[ "DepthClipEnabled" ];
		const zpBison::Value scissorEnabledValue = raster[ "ScissorEnabled" ];
		const zpBison::Value multisampleEnabledValue = raster[ "MultiSampleEnabled" ];
		const zpBison::Value antialiasedLinedEnabledValue = raster[ "AntialiasedLinedEnabled" ];
		const zpBison::Value depthBiasValue = raster[ "DepthBias" ];
		const zpBison::Value depthBiasClampValue = raster[ "DepthBiasClamp" ];
		const zpBison::Value slopeScaledDepthBiasValue = raster[ "SlopeScaledDepthBias" ];

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

zpCamera* zpRenderingPipeline::getCamera( zp_int cameraIndex )
{
	return &m_cameras[ cameraIndex ];
}
void zpRenderingPipeline::useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer )
{
	if( camera->update() )
	{
		i->update( cameraBuffer, &camera->getCameraBufferData(), sizeof( zpCameraBufferData ) );
	}
	i->setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, cameraBuffer );

	i->setViewport( camera->getViewport() );
	i->setScissorRect( camera->getClipRect() );
}
