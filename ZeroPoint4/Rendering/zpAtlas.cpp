#include "zpRendering.h"
#include "Common/zpCommon.h"

zpAtlas::zpAtlas()
{

}
zpAtlas::~zpAtlas()
{

}

const zpMaterialResourceInstance& zpAtlas::getMaterial() const
{
	return m_materialAtlas;
}
const zpAtlasSprite* zpAtlas::getSprite( const zp_char* spriteName ) const
{
	const zpAtlasSprite* sprite = ZP_NULL;

	m_sprites.findIf( [ spriteName ]( const zpAtlasSprite& s ) { return s.name == spriteName; }, &sprite );

	return sprite;
}


zp_bool zpAtlasContentManager::createResource( zpAtlasResource* res, const zp_char* filename )
{
	zpRenderingPipeline* pipeline = getApplication()->getRenderPipeline();
	return res->load( filename, pipeline );
}
void zpAtlasContentManager::destroyResource( zpAtlasResource* res )
{
	zpRenderingPipeline* pipeline = getApplication()->getRenderPipeline();
	res->unload( pipeline );
}



zp_bool zpAtlasResource::load( const zp_char* filename, zpRenderingPipeline* pipeline )
{
	zp_bool ok = false;
	if( m_resource.m_atlasData.readFromFile( zpString( filename ) ) )
	{
		const zpBison::Value& root = m_resource.m_atlasData.root();

		const zpBison::Value& materialFilename = root[ "Material" ];
		ok = pipeline->getMaterialContentManager()->getResource( materialFilename.asCString(), m_resource.m_materialAtlas );

		const zpBison::Value& sprites = root[ "Sprites" ];
		m_resource.m_sprites.reserve( sprites.size() );
		sprites.foreachObject( [ this ]( const zpBison::Value& k, const zpBison::Value& v ) {
			const zpBison::Value& outer = v[ "Outer" ];
			const zpBison::Value& inner = v[ "Inner" ];
			const zpBison::Value& padding = v[ "Padding" ];

			zpAtlasSprite& sprite = m_resource.m_sprites.pushBackEmpty();
			sprite.name = k.asCString();
			sprite.outer = zpRectf( outer[ 0 ].asFloat(), outer[ 1 ].asFloat(), outer[ 2 ].asFloat(), outer[ 3 ].asFloat() );
			sprite.inner = zpRectf( inner[ 0 ].asFloat(), inner[ 1 ].asFloat(), inner[ 2 ].asFloat(), inner[ 3 ].asFloat() );
			sprite.padding = zpMath::Vector4( padding[ 0 ].asFloat(), padding[ 1 ].asFloat(), padding[ 2 ].asFloat(), padding[ 3 ].asFloat() );
			sprite.rotated = v[ "Rotated" ].asBool();
		} );
	}

	return ok;
}
void zpAtlasResource::unload( zpRenderingPipeline* pipeline )
{
	m_resource.m_materialAtlas.release();
	m_resource.m_atlasData.destroy();
	m_resource.m_sprites.clear();
}
