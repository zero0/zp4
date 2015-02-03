#include "zpRendering.h"
#include "Common/zpCommon.h"

zp_bool zpMaterialResource::load( const zp_char* filename, zpRenderingPipeline* pipeline, zp_ushort materialId )
{
	zp_bool ok;
	m_filename = filename;

	zpBison material;
	ok = material.readFromFile( m_filename );

	if( ok )
	{
		m_resource.materialId = materialId;

		const zpBison::Value& root = material.root();

		// load shader for the material
		const zp_char* shaderFile = root[ "Shader" ].asCString();
		ok = pipeline->getShaderContentManager()->getResource( shaderFile, m_resource.shader );
		ZP_ASSERT( ok, "Failed to load shader '%s' for material '%s'", shaderFile, getFilename().str() );

		const zpBison::Value& shaderData = m_resource.shader.getResource()->getData()->getShaderData().root();

		// load sort bias
		zp_ushort sortBias = root[ "SortBias" ].asInt();
		m_resource.sortBias = sortBias;

		// load textures for the material
		const zpBison::Value& textures = root[ "Textures" ];
		m_resource.textures.reserve( textures.size() );
		textures.foreachArray( [ this, pipeline, &ok ]( const zpBison::Value& v )
		{
			const zp_char* tex = v.asCString();

			zpMaterial::zpMaterialTextureSampler& t = m_resource.textures.pushBackEmpty();
			t.bindSlots = ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER;

			ok = pipeline->getTextureContentManager()->getResource( tex, t.texture );
			ZP_ASSERT( ok, "Failed to load texture '%s' for material '%s'", tex, getFilename().str() );

			zpSamplerStateDesc sampler;
			pipeline->getRenderingEngine()->createSamplerState( t.sampler, sampler );
		} );

		// load constants from the material
		const zpBison::Value& constants = root[ "Constants" ];

		const zpBison::Value& shaderGlobals = shaderData[ "Globals" ];
		if( !shaderGlobals.isEmpty() )
		{
			zp_uint globalVariablesSize = shaderData[ "GlobalsSize" ].asInt();

			pipeline->getRenderingEngine()->createBuffer( m_resource.globalVariables, ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DYNAMIC, globalVariablesSize );
		}

		// load texture samplers for the material
		const zpBison::Value& samples = root[ "Samplers" ];
		samples.foreachArrayIndexed( [ this, pipeline ]( zp_uint i, const zpBison::Value& v )
		{
			zpMaterial::zpMaterialTextureSampler& t = m_resource.textures[ i ];

			zpSamplerStateDesc sampler;
			pipeline->generateSamplerStateDesc( v, sampler );
			pipeline->getRenderingEngine()->createSamplerState( t.sampler, sampler );
		} );

		// load blend mode for the material
		const zpBison::Value& blendMode = root[ "BlendMode" ];
		if( !blendMode.isEmpty() )
		{
			zpBlendStateDesc blendDesc;
			pipeline->generateBlendStateDesc( blendMode, blendDesc );
			pipeline->getRenderingEngine()->createBlendState( m_resource.blend, blendDesc );
		}

		// load depth stencil for material
		const zpBison::Value& depth = root[ "Depth" ];
		if( !depth.isEmpty() )
		{
			zpDepthStencilStateDesc depthDesc;
			pipeline->generateDepthStencilStateDesc( depth, depthDesc );
			pipeline->getRenderingEngine()->createDepthStencilState( m_resource.depth, depthDesc );
		}
	}

	return ok;
}
void zpMaterialResource::unload()
{
	m_resource.shader.release();
	m_resource.textures.clear();
}

zpMaterialContentManager::zpMaterialContentManager()
	: m_currentMaterialId( 0 )
{}
zp_bool zpMaterialContentManager::createResource( zpMaterialResource* res, const zp_char* filename )
{
	return res->load( filename, getApplication()->getRenderPipeline(), m_currentMaterialId++ );
}
void zpMaterialContentManager::destroyResource( zpMaterialResource* res )
{
	res->unload();
}
