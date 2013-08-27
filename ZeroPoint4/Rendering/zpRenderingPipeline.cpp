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
{
}
zpRenderingPipeline::~zpRenderingPipeline()
{
	m_engine = ZP_NULL;
}

zpRenderingEngine* zpRenderingPipeline::getRenderingEngine() const
{
	return m_engine;
}

void zpRenderingPipeline::initialize()
{
	m_materialContent.getResource( "materials/simple.materialb", m_mat );
	const zpVector2i& size = m_engine->getWindow()->getScreenSize();

	m_viewport.width = size.getX();
	m_viewport.height = size.getY();
}
void zpRenderingPipeline::destroy()
{
	m_mat.release();
}

void zpRenderingPipeline::beginFrame()
{
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();
	i->clearState();
}

void zpRenderingPipeline::submitRendering()
{
	zpTexture* t = m_engine->getBackBufferRenderTarget();
	zpDepthStencilBuffer* d = m_engine->getBackBufferDepthStencilBuffer();

	zpRenderingContext* i = m_engine->getImmediateRenderingContext();

	i->setRenderTarget( 0, 1, &t, d );
	i->clearRenderTarget( t, zpColor4f( 1, 0, 0, 1 ) );
	i->setViewport( m_viewport );

	//i->clearDepthStencilBuffer( 1.0f, 0 );

	zpRandom* r = zpRandom::getInstance();
	const zp_int numDrawCalls = 10; //r->randomInt( 1, 10 );
	const zp_int numTris = 1000; //r->randomInt( 1, 10 ) * 3;

	zpVector4f p0, p1, p2;
	zpColor4f c0, c1, c2;
	zp_int index = 0;
	for( zp_int d = 0; d < numDrawCalls; ++d )
	{
		i->beginDrawImmediate( ZP_RENDERING_LAYER_OPAQUE, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_mat );

		for( zp_int n = 0; n < numTris; ++n )
		{
			r->randomUnitCircle( p0 );
			r->randomUnitCircle( p1 );
			r->randomUnitCircle( p2 );

			r->randomColor( c0 );
			r->randomColor( c1 );
			r->randomColor( c2 );

			i->addVertex( p0, c0 );
			i->addVertex( p1, c1 );
			i->addVertex( p2, c2 );
			i->addTriangleIndex( index + 0, index + 2, index + 1 );
			index += 3;
		}

		i->endDrawImmediate();
	}
	
	i->preprocessCommands();
	i->processCommands( ZP_RENDERING_LAYER_OPAQUE );
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

	if( sampler.isEmpty() )
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

	if( raster.isEmpty() )
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
