#pragma once
#ifndef ZP_ATLAS_H
#define ZP_ATLAS_H

class zpSprite
{
public:
	zpSprite();
	~zpSprite();

private:
	zpRecti m_rect;
	zpRectf m_uv;
};

class zpSpriteNode
{
public:
	zpSpriteNode( const zpRecti& rect );
	~zpSpriteNode();

	zpSpriteNode* insert( const zpTextureResourceInstance& texture, zp_uint padding );
	void clearChildren();

private:
	zp_bool m_isUsed;

	zpSpriteNode* m_left;
	zpSpriteNode* m_right;

	zpRecti m_rect;
	zpString m_name;
};

class zpAtlas
{
public:
	zpAtlas();
	~zpAtlas();

	void create( zp_uint width, zp_uint height, zp_uint padding, zpDisplayFormat format, zpRenderingPipeline* pipeline );

	void loadSprites( const zpArrayList< zpString >& textureNames );
	const zpSprite* getSprite( const zp_char* textureName );
	void removeSprite( const zp_char* textureName );

private:
	zpTextureResourceInstance m_atlas;
	zp_int m_padding;
	zpSpriteNode m_root;
};


class zpAtlasContentManager;

class zpAtlasResource : public zpResource< zpAtlas >
{
private:
	zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline ) { return true; }
	void unload( zpRenderingEngine* engine ) {}

	friend class zpAtlasContentManager;
};

class zpAtlasResourceInstance : public zpResourceInstance< zpAtlasResource >
{
};

class zpAtlasContentManager : public zpContentManager< zpAtlasResource, zpAtlasResourceInstance, zpAtlasContentManager, 2 >
{
public:
	zp_bool getResourceWithoutLoadAtlas( zpAtlasResourceInstance& outInstance, zp_uint width, zp_uint height, zpDisplayFormat format );

private:
	zp_bool createResource( zpAtlasResource* res, const zp_char* filename );
	void destroyResource( zpAtlasResource* res );
	void initializeInstance( zpAtlasResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
