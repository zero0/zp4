#include "zpCommon.h"

zpScriptComponent::zpScriptComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
{
    const zpBison::Value scripts = def[ "Scripts" ];
    zpScriptContentManager* scriptContent = getApplication()->getScriptContentManager();
    zp_bool ok;

    if( scripts.isArray() )
    {
        zp_uint count = scripts.size();
        if( count > 0 )
        {
            m_scripts.reserve( count );
            scripts.foreachArray( [ this, scriptContent, &ok ]( const zpBison::Value& script )
            {
                const zp_char* scriptFile = script.asCString();
                ok = scriptContent->getResource( scriptFile, m_scripts.pushBackEmpty() );
                ZP_ASSERT( ok, "Failed to build script %s", scriptFile );
            } );
        }
    }
    else if( scripts.isString() )
    {
        m_scripts.reserve( 1 );
        const zp_char* scriptFile = scripts.asCString();
        ok = scriptContent->getResource( scriptFile, m_scripts.pushBackEmpty() );
        ZP_ASSERT( ok, "Failed to build script %s", scriptFile );
    }
}
zpScriptComponent::~zpScriptComponent()
{}

void zpScriptComponent::onCreate()
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "create" );
    } );
}
void zpScriptComponent::onInitialize()
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "initialize" );
    } );
}
void zpScriptComponent::onDestroy()
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "onDestroy" );
    } );
}

void zpScriptComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "update" );
    } );
}
void zpScriptComponent::onSimulate()
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "simulate" );
    } );
}

void zpScriptComponent::onEnabled()
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "onEnabled" );
    } );
}
void zpScriptComponent::onDisabled()
{
    m_scripts.foreach( []( zpScriptResourceInstance& script )
    {
        script.callMethodImmidiate( "onDisabled" );
    } );
}


zpScriptComponentPool::zpScriptComponentPool() {}
zpScriptComponentPool::~zpScriptComponentPool() {}

void zpScriptComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpScriptComponent* o )
    {
        o->update( deltaTime, realTime );
    } );

    zpAngelScript::getInstance()->processThreads( 0 );
}
void zpScriptComponentPool::simulate()
{
    m_used.foreach( []( zpScriptComponent* o )
    {
        o->simulate();
    } );
}