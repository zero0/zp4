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
		const zpBison::Value& root = material.root();

		const zp_char* shaderFile = root[ "Shader" ].asCString();
		pipeline->getShaderContentManager()->getResource( shaderFile, m_resource.shader );
	}

	return ok;
}
void zpMaterialResource::unload()
{
	m_resource.shader.release();
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

void zpMaterialResourceInstance::setBuffer( zpResourceBindSlot slot, zp_uint index, zpBuffer* buffer )
{

}

zp_bool zpMaterialContentManager::createResource( zpMaterialResource* res, const zp_char* filename )
{
	return res->load( filename, getApplication()->getRenderPipeline() );
}
void zpMaterialContentManager::destroyResource( zpMaterialResource* res )
{
	res->unload();
}
