#include "zpRendering.h"

zp_bool zpMaterialResource::load( const zp_char* filename )
{
	m_filename = filename;

	zpBison material;
	material.readFromFile( m_filename );

	/*zpProperties material( getFilename() );

	if( !material.hasProperty( "material.shader" ) ) return false;
	m_material.shader = getContentManager()->createInstanceOfResource<zpShaderResource>( material[ "material.shader" ] );

	zpProperties textures = material.getSubProperties( "material.texture" );
	if( !textures.isEmpty() ) {
		textures.foreach( [ this ]( const zpString& key, const zpString& value ) {
			if( key == "diffuse" ) {
				m_material.textures[ ZP_MATERIAL_TEXTURE_SLOT_DIFFUSE ] = getContentManager()->createInstanceOfResource<zpTextureResource>( value );
			} else if( key == "normal" ) {
				m_material.textures[ ZP_MATERIAL_TEXTURE_SLOT_NORMAL ] = getContentManager()->createInstanceOfResource<zpTextureResource>( value );
			} else if( key == "specular" ) {
				m_material.textures[ ZP_MATERIAL_TEXTURE_SLOT_SPECULAR ] = getContentManager()->createInstanceOfResource<zpTextureResource>( value );
			} else if( key == "opacity" ) {
				m_material.textures[ ZP_MATERIAL_TEXTURE_SLOT_OPACITY ] = getContentManager()->createInstanceOfResource<zpTextureResource>( value );
			} else if( key == "other" ) {
				m_material.textures[ ZP_MATERIAL_TEXTURE_SLOT_OTHER ] = getContentManager()->createInstanceOfResource<zpTextureResource>( value );
			}
		} );
	}*/

	return true;
}
void zpMaterialResource::unload()
{
	/*m_material.shader = zpResourceInstance<zpShaderResource>();
	m_material.textures.map( []( const zpResourceInstance<zpTextureResource>& ) {
		return zpResourceInstance<zpTextureResource>();
	} );*/
}

zpMaterialResourceInstance::zpMaterialResourceInstance()
{
	while( m_textureOverides.size() != zpMaterialTextureSlot_Count )
	{
		m_textureOverides.pushBackEmpty();
	}
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

}

void zpMaterialResourceInstance::setBuffer( zpResourceBindSlot slot, zp_uint index, zpBuffer* buffer )
{

}

zp_bool zpMaterialContentManager::createResource( zpMaterialResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpMaterialContentManager::destroyResource( zpMaterialResource* res )
{
	res->unload();
}
