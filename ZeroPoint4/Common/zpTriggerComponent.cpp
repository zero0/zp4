#include "zpCommon.h"

zpTriggerComponent::zpTriggerComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{

}
zpTriggerComponent::~zpTriggerComponent()
{

}

void zpTriggerComponent::onCreate()
{

}
void zpTriggerComponent::onInitialize()
{

}
void zpTriggerComponent::onDestroy()
{

}

void zpTriggerComponent::onUpdate()
{
	const zpVector4f& center = getParentObject()->getTransform().getRow( 3 );
	m_bounds.setCenter( center );
}
void zpTriggerComponent::onSimulate()
{

}

void zpTriggerComponent::onEnabled()
{

}
void zpTriggerComponent::onDisabled()
{

}


zpTriggerComponentPool::zpTriggerComponentPool()
{

}
zpTriggerComponentPool::~zpTriggerComponentPool()
{

}

void zpTriggerComponentPool::update()
{
	m_used.foreach( []( zpTriggerComponent* o )
	{
		o->update();
	} );
}
