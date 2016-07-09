
template< typename Component, typename ComponentManager, zp_uint Count >
zpComponentManager< Component, ComponentManager, Count >::zpComponentManager()
    : m_firstFree( 0 )
{
    zp_zero_memory_array( m_isUsed );
}
template< typename Component, typename ComponentManager, zp_uint Count >
zpComponentManager< Component, ComponentManager, Count >::~zpComponentManager()
{

}

template< typename Component, typename ComponentManager, zp_uint Count >
void zpComponentManager< Component, ComponentManager, Count >::update()
{
    Component* ptr = (Component*)m_pool;
    for( zp_uint i = 0; i < Count; ++i, ++ptr )
    {
        if( m_isUsed[ i ] )
        {
            ptr->update();
        }
    }
}
template< typename Component, typename ComponentManager, zp_uint Count >
void zpComponentManager< Component, ComponentManager, Count >::simlate()
{
    Component* ptr = (Component*)m_pool;
    for( zp_uint i = 0; i < Count; ++i, ++ptr )
    {
        if( m_isUsed[ i ] )
        {
            ptr->simlate();
        }
    }
}

template< typename Component, typename ComponentManager, zp_uint Count >
Component* zpComponentManager< Component, ComponentManager, Count >::createComponent( const zpBison::Value& componentData )
{
    ComponentManager *impl = (ComponentManager*)this; //dynamic_cast<ComponentManager*>( this );
    for( zp_uint i = m_firstFree; i < Count; ++i )
    {
        if( !m_isUsed[ i ] )
        {
            m_firstFree = i + 1;
            m_isUsed[ i ] = true;

            Component* ptr = ( ( (Component*)m_pool ) + i );

            new (ptr) Component( componentData );
            impl->componentCreated( ptr );

            return ptr;
        }
    }
    return ZP_NULL;
}
template< typename Component, typename ComponentManager, zp_uint Count >
void zpComponentManager< Component, ComponentManager, Count >::destroyComponent( Component* component )
{
    ComponentManager *impl = (ComponentManager*)this; //dynamic_cast<ComponentManager*>( this );
    for( zp_uint i = 0; i < Count; ++i )
    {
        Component* ptr = ( ( (Component*)m_pool ) + i );
        if( ptr == obj )
        {
            impl->componentDestroyed( ptr );
            ptr->~T();
            zp_zero_memory( ptr );

            m_isUsed[ i ] = false;
            m_firstFree = i;
            break;
        }
    }
}

#if 0
zpGameManager::zpGameManager() 
    : m_flags( 0 )
    , m_game( ZP_NULL )
{}
zpGameManager::~zpGameManager() {}

void zpGameManager::create() {
    if( m_flags.isMarked( ZP_GAME_MANAGER_FLAG_CREATED ) ) return;
    m_flags.mark( ZP_GAME_MANAGER_FLAG_CREATED );

    onCreate();
}
void zpGameManager::destroy() {
    if( !m_flags.isMarked( ZP_GAME_MANAGER_FLAG_CREATED ) ) return;
    m_flags.unmark( ZP_GAME_MANAGER_FLAG_CREATED );

    onDestroy();
}

void zpGameManager::update() {
    if( m_flags.isAllMarked( 1 << ZP_GAME_MANAGER_FLAG_CREATED | 1 << ZP_GAME_MANAGER_FLAG_ENABLED ) ) onUpdate();
}
void zpGameManager::simulate() {
    if( m_flags.isAllMarked( 1 << ZP_GAME_MANAGER_FLAG_CREATED | 1 << ZP_GAME_MANAGER_FLAG_ENABLED ) ) onSimulate();
}

void zpGameManager::setEnabled( zp_bool enabled ) {
    bool wasEnabled = m_flags.isMarked( ZP_GAME_MANAGER_FLAG_ENABLED );
    m_flags.setMarked( ZP_GAME_MANAGER_FLAG_ENABLED, enabled );

    if( wasEnabled && !enabled ) {
        onDisabled();
    } else if( !wasEnabled && enabled ) {
        onEnabled();
    }
}
zp_bool zpGameManager::isEnabled() const {
    return m_flags.isMarked( ZP_GAME_MANAGER_FLAG_ENABLED );
}

void zpGameManager::setGame( zpGame* game ) {
    m_game = game;
}
zpGame* zpGameManager::getGame() const {
    return m_game;
}
#endif
