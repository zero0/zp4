#pragma once
#ifndef ZP_ATLAS_H
#define ZP_ATLAS_H

class zpAtlasContentManager;
class zpAtlasResourceInstance;
class zpAtlasResource;

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

//////////////////////////////////////////////////////////////////////////

struct zpAtlasSprite
{
    zpString name;
    zpRectf outer;
    zpRectf inner;
    zpVector4f padding;
    zp_bool rotated;
};

class zpAtlas
{
public:
    zpAtlas();
    ~zpAtlas();

    const zpMaterialResourceInstance& getMaterial() const;
    const zpAtlasSprite* getSprite( const zp_char* spriteName ) const;

private:
    zpBison m_atlasData;
    zpMaterialResourceInstance m_materialAtlas;
    zpArrayList< zpAtlasSprite > m_sprites;

    friend class zpAtlasResource;
};

//////////////////////////////////////////////////////////////////////////

class zpAtlasResource : public zpResource< zpAtlas >
{
private:
    zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline );
    void unload( zpRenderingPipeline* pipeline );

    friend class zpAtlasContentManager;
};


class zpAtlasResourceInstance : public zpResourceInstance< zpAtlasResource >
{
};


class zpAtlasContentManager : public zpContentManager< zpAtlasResource, zpAtlasResourceInstance, zpAtlasContentManager, 2 >
{
private:
    zp_bool createResource( zpAtlasResource* res, const zp_char* filename );
    void destroyResource( zpAtlasResource* res );
    void initializeInstance( zpAtlasResourceInstance& instance ) {}

    template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
    friend class zpContentManager;
};

#endif
