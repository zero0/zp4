#include "zpCommon.h"

#if 1
zpEditorCameraComponent::zpEditorCameraComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_isActive( false )
{}
zpEditorCameraComponent::~zpEditorCameraComponent() {}

void zpEditorCameraComponent::receiveMessage( const zpMessage& message ) {}

void zpEditorCameraComponent::onKeyDown( zpKeyCode key ) {}
void zpEditorCameraComponent::onKeyRepeat( zpKeyCode key ) {}
void zpEditorCameraComponent::onKeyUp( zpKeyCode key ) {
}

void zpEditorCameraComponent::onMouseButtonDown( zpMouseButton button ) {}
void zpEditorCameraComponent::onMouseButtonRepeat( zpMouseButton button ) {}
void zpEditorCameraComponent::onMouseButtonUp( zpMouseButton button ) {}

void zpEditorCameraComponent::onMouseScroll( zp_int scrollDelta ) {
}

void zpEditorCameraComponent::onMouseMove( const zpVector2i& location ) {}
void zpEditorCameraComponent::onMouseChange( const zpVector2i& delta ) {}

void zpEditorCameraComponent::onCreate()
{
    zp_printfln( "create editor camera component" );
}
void zpEditorCameraComponent::onDestroy() {
    zp_printfln( "destroy editor camera component" );
}

void zpEditorCameraComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
}

void zpEditorCameraComponent::onEnabled() {}
void zpEditorCameraComponent::onDisabled() {}


zpEditorCameraComponentPool::zpEditorCameraComponentPool() {}
zpEditorCameraComponentPool::~zpEditorCameraComponentPool() {}

void zpEditorCameraComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpEditorCameraComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
void zpEditorCameraComponentPool::simulate()
{
    m_used.foreach( []( zpEditorCameraComponent* o )
    {
        o->simulate();
    } );
}

#endif
