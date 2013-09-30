#include "zpCommon.h"

zpScriptComponent::zpScriptComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	zp_uint count = def.size();
	m_scripts.reserve( count );
	def.foreachArray( [ this ]( const zpBison::Value& script )
	{
		const zp_char* scriptFile = script.asCString();
		getApplication()->getScriptContentManager()->getResource( scriptFile, m_scripts.pushBackEmpty() );
	} );
}
zpScriptComponent::~zpScriptComponent()
{
	m_scripts.clear();
}

void zpScriptComponent::onCreate()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "create" );
	} );
}
void zpScriptComponent::onInitialize()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "onInitialize" );
	} );
}
void zpScriptComponent::onDestroy()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "onDestroy" );
	} );
}

void zpScriptComponent::onUpdate()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "update" );
	} );
}
void zpScriptComponent::onSimulate()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "simulate" );
	} );
}

void zpScriptComponent::onEnabled()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "onEnabled" );
	} );
}
void zpScriptComponent::onDisabled()
{
	m_scripts.foreach( []( zpScriptResourceInstance& script )
	{
		script.callMethod( "onDisabled" );
	} );
}


zpScriptComponentPool::zpScriptComponentPool() {}
zpScriptComponentPool::~zpScriptComponentPool() {}

void zpScriptComponentPool::update()
{
	m_used.foreach( []( zpScriptComponent* o )
	{
		o->update();
	} );
}
void zpScriptComponentPool::simulate()
{
	m_used.foreach( []( zpScriptComponent* o )
	{
		o->simulate();
	} );
}