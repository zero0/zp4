#include "zpCommon.h"

zpPlayerComponent::zpPlayerComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	m_player = (zpPlayerNumber)def[ "Player" ].asInt();
	m_controllerIndex = (zpControllerNumber)m_player;
}
zpPlayerComponent::~zpPlayerComponent()
{

}

void zpPlayerComponent::onCreate()
{
	m_controller = getApplication()->getInputManager()->getController( m_controllerIndex );
}
void zpPlayerComponent::onInitialize()
{

}
void zpPlayerComponent::onDestroy()
{

}

void zpPlayerComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	zpKinematicBodyComponent* kinematic = getParentObject()->getComponents()->getKinematicBodyComponent();
	
	
}
void zpPlayerComponent::onSimulate()
{

}

void zpPlayerComponent::onEnabled()
{

}
void zpPlayerComponent::onDisabled()
{

}
