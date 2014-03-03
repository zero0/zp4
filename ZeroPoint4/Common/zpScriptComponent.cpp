#include "zpCommon.h"

zpScriptComponent::zpScriptComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	const zpBison::Value scripts = def[ "Scripts" ];

	if( scripts.isArray() )
	{
		zp_uint count = scripts.size();
		if( count > 0 )
		{
			zpScriptContentManager* scriptContent = getApplication()->getScriptContentManager();

			m_scripts.reserve( count );
			scripts.foreachArray( [ this, scriptContent ]( const zpBison::Value& script )
			{
				const zp_char* scriptFile = script.asCString();
				scriptContent->getResource( scriptFile, m_scripts.pushBackEmpty() );
			} );
		}
	}
}
zpScriptComponent::~zpScriptComponent()
{
}

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

void zpScriptComponent::onUpdate()
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

void zpScriptComponentPool::update()
{
	m_used.foreach( []( zpScriptComponent* o )
	{
		o->update();
	} );

	zpAngelScript::getInstance()->processThreads();
}
void zpScriptComponentPool::simulate()
{
	m_used.foreach( []( zpScriptComponent* o )
	{
		o->simulate();
	} );
}