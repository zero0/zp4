#include "zpRendering.h"
#include "Common/zpCommon.h"

zp_bool zpMaterialResource::load( const zp_char* filename, zpRenderingPipeline* pipeline )
{
	zp_bool ok;
	m_filename = filename;

	zpBison material;
	ok = material.readFromFile( m_filename );

	if( ok )
	{
		m_resource.textures.resize( zpMaterialTextureSlot_Count );
		const zpBison::Value& root = material.root();

		const zp_char* shaderFile = root[ "Shader" ].asCString();
		ok = pipeline->getShaderContentManager()->getResource( shaderFile, m_resource.shader );
		ZP_ASSERT( ok, "Failed to load shader '%s' for material '%s'", shaderFile, getFilename().str() );

		const zpBison::Value textures = root[ "Textures" ];
		textures.foreachArrayIndexed( [ this, pipeline, &ok ]( zp_uint i, const zpBison::Value& v )
		{
			const zp_char* tex = v.asCString();

			zpMaterial::zpMaterialTextureSampler& t = m_resource.textures[ i ];
			ok = pipeline->getTextureContentManager()->getResource( tex, t.texture );
			ZP_ASSERT( ok, "Failed to load texture '%s' for material '%s'", tex, getFilename().str() );
		} );

		const zpBison::Value constants = root[ "Constants" ];


		const zpBison::Value samples = root[ "Samplers" ];
		samples.foreachArrayIndexed( [ this, pipeline ]( zp_uint i, const zpBison::Value& v )
		{
			zpMaterial::zpMaterialTextureSampler& t = m_resource.textures[ i ];

			zpSamplerStateDesc sampler;
			pipeline->generateSamplerStateDesc( v, sampler );
			t.sampler = pipeline->getRenderingEngine()->createSamplerState( sampler );
		} );
	}

	return ok;
}
void zpMaterialResource::unload()
{
	m_resource.shader.release();
	m_resource.textures.clear();
}

zpMaterialResourceInstance::zpMaterialResourceInstance()
{
	m_textureOverides.resize( zpMaterialTextureSlot_Count );
}
zpMaterialResourceInstance::~zpMaterialResourceInstance()
{
	m_textureOverides.clear();
}

void zpMaterialResourceInstance::setTextureOverride( zpMaterialTextureSlot slot, const zpTextureResourceInstance& texture )
{
	m_textureOverides[ slot ] = texture;
}
void zpMaterialResourceInstance::resetTexture( zpMaterialTextureSlot slot )
{
	m_textureOverides[ slot ].release();
}

const zpTexture* zpMaterialResourceInstance::getTexture( zpMaterialTextureSlot slot ) const
{
	const zpTextureResourceInstance& o = m_textureOverides[ slot ];
	const zpTextureResourceInstance& t = getResource()->getData()->textures[ slot ].texture;
	
	const zpTexture* tex = o.isVaild() ? o.getResource()->getData() : t.isVaild() ? t.getResource()->getData() : ZP_NULL;
	return tex;
}
const zpSamplerState* zpMaterialResourceInstance::getSampler( zpMaterialTextureSlot slot ) const
{
	const zpSamplerState* sampler = getResource()->getData()->textures[ slot ].sampler;
	return sampler;
}

zp_bool zpMaterialContentManager::createResource( zpMaterialResource* res, const zp_char* filename )
{
	return res->load( filename, getApplication()->getRenderPipeline() );
}
void zpMaterialContentManager::destroyResource( zpMaterialResource* res )
{
	res->unload();
}
