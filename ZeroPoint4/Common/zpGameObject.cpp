#include "zpCommon.h"

zpGameObject::zpGameObject()
	: m_transform()
	, m_world( ZP_NULL )
	, m_components()	
	, m_flags( 0 )
	, m_name()
{}
zpGameObject::~zpGameObject()
{
	destroy();
}

zpAllComponents* zpGameObject::getComponents()
{
	return &m_components;
}

void zpGameObject::setFlag( zpGameObjectFlag flag )
{
	m_flags.mark( (zp_uint)flag );
}

void zpGameObject::unsetFlag( zpGameObjectFlag flag )
{
	m_flags.unmark( (zp_uint)flag );
}

zp_bool zpGameObject::isFlagSet( zpGameObjectFlag flag ) const
{
	return m_flags.isMarked( (zp_uint)flag );
}

void zpGameObject::setWorld( zpWorld* world )
{
	m_world = world;
}
zpWorld* zpGameObject::getWorld() const
{
	return m_world;
}

void zpGameObject::update()
{
	m_components.update();
}
void zpGameObject::simulate()
{
	m_components.simulate();
}

void zpGameObject::create()
{
	m_components.create();
}
void zpGameObject::destroy()
{
	m_components.destroy();
}

const zpString& zpGameObject::getName() const
{
	return m_name;
}
void zpGameObject::setName( const zpString& name )
{
	m_name = name;
}

const zpMatrix4f& zpGameObject::getTransform() const
{
	return m_transform;
}
void zpGameObject::setTransform( const zpMatrix4f& transform )
{
	m_transform = transform;
}

void zpGameObject::receiveMessage( const zpMessage& message )
{
	sendMessageToComponents( message );
	//sendMessageToChildGameObjects( message );
}
void zpGameObject::sendMessageToComponents( const zpMessage& message )
{
	
}
//void zpGameObject::sendMessageToChildGameObjects( const zpMessage& message )
//{
//	
//}
//void zpGameObject::sendMessageToParentGameObject( const zpMessage& message )
//{
//}
