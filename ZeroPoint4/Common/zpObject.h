#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

class zpObject;
class zpObjectContentManager;
class zpObjectResource;

typedef zpComponent* (*zpComponentFactoryCreate)( zpObject* obj, const zpBison::Value& data );
typedef void (*zpComponentFactoryDestroy)( zpComponent* cmp );

struct zpComponentFactoryFunctions
{
    zpComponentFactoryCreate createFunc;
    zpComponentFactoryDestroy destroyFunc;
};

enum zpObjectFlag : zp_uint
{
    ZP_OBJECT_FLAG_ENABLED,
    ZP_OBJECT_FLAG_CREATED,
    ZP_OBJECT_FLAG_INITIALIZED,
    ZP_OBJECT_FLAG_SHOULD_DESTROY,
    ZP_OBJECT_FLAG_DONT_DESTROY_ON_UNLOAD,
    ZP_OBJECT_FLAG_STATIC,
    ZP_OBJECT_FLAG_TRANSFORM_DIRTY,
    zpObjectFlag_Count,

    ZP_OBJECT_FLAG_USER0,
    zpObjectFlagUser_Count = 31,

    ZP_OBJECT_FLAG_CAN_UPDATE = 1 << ZP_OBJECT_FLAG_ENABLED | 1 << ZP_OBJECT_FLAG_CREATED | 1 << ZP_OBJECT_FLAG_INITIALIZED,
};

class zpObjectResource : public zpResource< zpBison >
{
private:
    zp_bool load( const zp_char* filename );
    void unload();

    friend class zpObjectContentManager;
};

class zpObjectResourceInstance : public zpResourceInstance< zpObjectResource >
{};


class zpObject
{
    friend class zpObjectContentManager;

public:
    zpObject( zpApplication* application );
    zpObject( zpApplication* application, const zpObjectResourceInstance& res );
    zpObject( zpApplication* application, const zpBison::Value& root );
    ~zpObject();

    zpAllComponents* getComponents();

    zp_uint getPrefabID() const;
    zp_uint getID() const;
    void setID( zp_uint id );

    void setFlag( zpObjectFlag flag );
    void unsetFlag( zpObjectFlag flag );
    zp_bool isFlagSet( zpObjectFlag flag ) const;

    void setEnabled( zp_bool enabled );
    zp_bool isEnabled() const;

    const zpString& getName() const;
    void setName( const zpString& name );

    const zpFlag64& getTags() const;
    void clearTags();

    zp_bool hasTag( const zp_char* tag ) const;
    void addTag( const zp_char* tag );
    void removeTag( const zp_char* tag );

    zp_bool hasTag( const zpString& tag ) const;
    void addTag( const zpString& tag );
    void removeTag( const zpString& tag );

    const zpFlag32& getLayers() const;
    void setLayers( const zpFlag32& layers );
    zp_bool isOnLayer( zp_uint layer ) const;

    zpApplication* getApplication() const;

    zpWorld* getWorld() const;
    void setWorld( zpWorld* world );

    void initialize();
    void update();
    void destroy();

private:
    void load( zp_bool isInitialLoad, const zpBison::Value& root );
    void unload();

    zpString m_name;
    zpFlag64 m_tags;
    zpFlag32 m_layers;
    zpFlag32 m_flags;
    zp_long m_lastLoadTime;
    zp_uint m_prefabId;
    zp_uint m_instanceId;

    zpApplication* m_application;
    zpWorld* m_world;

    zpAllComponents m_components;

    zpObjectResourceInstance m_object;
};

class zpObjectContentManager : public zpContentManager< zpObjectResource, zpObjectResourceInstance, zpObjectContentManager, 16 >, public zpContentPool< zpObject, 64 >
{
public:
    zpObjectContentManager();
    virtual ~zpObjectContentManager();

    zpObject* createObject();
    zpObject* createObject( const zp_char* filename );
    zpObject* createObject( const zpBison::Value& def );

    void initializeAllObjectsInWorld( zpWorld* world );

    void destroyAllObjects();
    void destroyAllObjectsInWorld( zpWorld* world );

    void getAllObjectsInLayer( zp_uint layer, zpArrayList< zpObject* >& objects ) const;
    void getAllObjectsWithTag( zp_int tag, zpArrayList< zpObject* >& objects ) const;

    void registerComponent( const zp_char* componentType, zpComponentFactoryCreate factoryCreate, zpComponentFactoryDestroy factoryDestroy );
    void unregisterComponent( const zp_char* componentType );
    void unregisterAllComponents();

    zpComponent* createComponent( zpObject* obj, const zp_char* componentType, const zpBison::Value& data );
    void destroyComponent( const zp_char* componentType, zpComponent* component );

    void update();

private:
    zp_bool createResource( zpObjectResource* res, const zp_char* filename );
    void destroyResource( zpObjectResource* res );
    void initializeInstance( zpObjectResourceInstance& instance ) {}

    zp_uint m_numObjects;

    zpArrayList< zpComponentFactoryFunctions > m_componentFactoryFunctions;
    zpArrayList< zp_hash > m_componentFactoryHashes;

    template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
    friend class zpContentManager;
};

#endif