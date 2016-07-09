#pragma once
#ifndef ZP_DYNAMIC_ATLAS_H
#define ZP_DYNAMIC_ATLAS_H

struct zpDynamicAtlasSprite
{
    zpString name;
    zpRecti outer;
    zpRecti inner;
    zpVector4f padding;
    zp_bool rotated;

    zpTextureResourceInstance texture;
};

class zpDynamicAtlasSpriteResource : public zpResource< zpDynamicAtlasSprite >
{
};

struct zpDynamicSpriteNode
{
    zpDynamicSpriteNode* left;
    zpDynamicSpriteNode* right;
    zpDynamicAtlasSpriteResource* image;
    zpRecti rect;

    zpDynamicSpriteNode() : left( ZP_NULL ), right( ZP_NULL ), image( ZP_NULL ) {}

    zpDynamicSpriteNode* insert( zpDynamicAtlasSpriteResource* sprite )
    {
        if( right != ZP_NULL )
        {
            zpDynamicSpriteNode* node;

            node = right->insert( image );
            if( node != ZP_NULL ) return node;

            node = left->insert( image );
            return node;
        }
        else
        {
            if( image != ZP_NULL ) return ZP_NULL;

            const zpRecti& r = sprite->getData()->outer;
            if( r.getSize().getX() > rect.getSize().getX() || r.getSize().getY() > rect.getSize().getY() ) return ZP_NULL;

            if( r.getSize().getX() == rect.getSize().getX() || r.getSize().getY() == rect.getSize().getY() ) return this;

        }
    }
};

class zpDynamicAtlasSpriteResourceInstance : public zpResourceInstance< zpDynamicAtlasSpriteResource >
{
};

class zpDynamicAtlas
{
public:
    zp_bool getSprite( const zp_char* spriteName, zpDynamicAtlasSpriteResourceInstance& sprite );

    const zpMaterialResourceInstance& getMaterial() const;

private:
    zpMaterialResourceInstance m_material;

    zpTextureResourceInstance m_atlas0;
    zpTextureResourceInstance m_atlas1;

    zpFixedArrayList< zpDynamicAtlasSpriteResource, 64 > m_sprites;
    zpFixedArrayList< zpDynamicAtlasSpriteResource*, 64 > m_freeSprites;
    zpFixedArrayList< zpDynamicAtlasSpriteResource*, 64 > m_usedSprites;
};


class zpDynamicAtlasResource : public zpResource< zpDynamicAtlas >
{
private:
    zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline );
    void unload( zpRenderingPipeline* pipeline );

    friend class zpDynamicAtlasContentManager;
};


class zpDynamicAtlasResourceInstance : public zpResourceInstance< zpDynamicAtlasResource >
{
};


class zpDynamicAtlasContentManager : public zpContentManager< zpDynamicAtlasResource, zpDynamicAtlasResourceInstance, zpDynamicAtlasContentManager, 2 >
{
private:
    zp_bool createResource( zpDynamicAtlasResource* res, const zp_char* filename );
    void destroyResource( zpDynamicAtlasResource* res );
    void initializeInstance( zpDynamicAtlasResourceInstance& instance ) {}

    template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
    friend class zpContentManager;
};

#endif
