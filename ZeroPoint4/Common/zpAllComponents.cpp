#include "zpCommon.h"

zpAllComponents::zpAllComponents()
    : m_object( ZP_NULL )
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            , m_##cmp( ZP_NULL )
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
{}
zpAllComponents::~zpAllComponents()
{
    unload();

    m_components.destroy();
}

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )    \
zp##cmp##Component* zpAllComponents::get##cmp##Component() const { return m_##cmp; }
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )    \
zp##cmp##Component* zpAllComponents::add##cmp##Component( const zpBison::Value& def ) \
{ \
    if( m_##cmp == ZP_NULL ) \
    { \
        m_##cmp = m_object->getApplication()->get##cmp##ComponentPool()->create( m_object, def ); \
        m_##cmp->create(); \
    } \
    return m_##cmp; \
}
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )    \
void zpAllComponents::remove##cmp##Component() \
{ \
    if( m_##cmp != ZP_NULL ) \
    { \
        m_##cmp->destroy(); \
        m_object->getApplication()->get##cmp##ComponentPool()->destroy( m_##cmp ); \
        m_##cmp = ZP_NULL; \
    } \
}
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

zpComponent* zpAllComponents::getComponent( const zp_char* componentType ) const
{
    zpComponent* cmp = ZP_NULL;

    zp_hash typeHash = zp_fnv1_32_string( componentType, 0 );

    zp_size_t index;
    if( m_componentHashes.findIndexIf( [ typeHash ]( zp_hash h ) { return h == typeHash; }, index ) )
    {
        cmp = m_components[ index ];
    }

    return cmp;
}
zpComponent* zpAllComponents::addComponent( const zp_char* componentType, const zpBison::Value& data )
{
    zpComponent* cmp = ZP_NULL;

    zp_hash typeHash = zp_fnv1_32_string( componentType, 0 );

    zp_size_t index;
    if( m_componentHashes.findIndexIf( [ typeHash ]( zp_hash h ) { return h == typeHash; }, index ) )
    {
        cmp = m_components[ index ];
    }
    else
    {
        cmp = m_object->getApplication()->getObjectContentManager()->createComponent( m_object, componentType, data );
        ZP_ASSERT( cmp != ZP_NULL, "Unable to create component %s!", componentType );

        m_components.pushBack( cmp );
        m_componentHashes.pushBack( typeHash );
    }

    return cmp;
}
void zpAllComponents::removeComponent( const zp_char* componentType )
{
    zp_hash typeHash = zp_fnv1_32_string( componentType, 0 );

    zp_size_t index;
    if( m_componentHashes.findIndexIf( [ typeHash ]( zp_hash h ) { return h == typeHash; }, index ) )
    {
        zpComponent* cmp = m_components[ index ];

        cmp->destroy();
        m_object->getApplication()->getObjectContentManager()->destroyComponent( componentType, cmp );

        m_components.erase( index );
        m_componentHashes.erase( index );
    }
}

void zpAllComponents::create( const zp_char* componentName, const zpBison::Value& def )
{
    zpApplication* app = m_object->getApplication();

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )    \
    if( zp_strcmp( componentName, #cmp ) == 0 ) { \
        m_##cmp = app->get##cmp##ComponentPool()->create( m_object, def ); \
        m_##cmp->create(); \
        return; }
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    

    ZP_ASSERT( false, "Unknown component name %s", componentName );
}

void zpAllComponents::initialize()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            if( m_##cmp ) { m_##cmp->initialize(); }
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    zpComponent** b = m_components.begin();
    zpComponent** e = m_components.end();
    for( ; b != e; ++b )
    {
        (*b)->initialize();
    }
}

void zpAllComponents::unload()
{
    zpApplication* app = m_object->getApplication();

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )    \
    if( m_##cmp ) { \
        m_##cmp->destroy(); \
        app->get##cmp##ComponentPool()->destroy( m_##cmp ); \
        m_##cmp = ZP_NULL; }
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    zpComponent** b = m_components.begin();
    zpComponent** e = m_components.end();
    for( ; b != e; ++b )
    {
        (*b)->destroy();
    }

    m_components.clear();

    m_object = ZP_NULL;
}

void zpAllComponents::setEnabled( zp_bool enabled )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            if( m_##cmp ) { m_##cmp->setEnabled( enabled ); }
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    zpComponent** b = m_components.begin();
    zpComponent** e = m_components.end();
    for( ; b != e; ++b )
    {
        (*b)->setEnabled( enabled );
    }
}

void zpAllComponents::setup( zpObject* obj )
{
    m_object = obj;
}
