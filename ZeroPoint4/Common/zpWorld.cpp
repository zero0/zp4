#include "zpCommon.h"

zpWorld::zpWorld( const zp_char* filename )
{}
zpWorld::~zpWorld()
{}

void zpWorld::addObject( zpObject* go )
{
	m_objects.pushBack( go );
	go->setWorld( this );
}
void zpWorld::removeObject( zpObject* go )
{
	go->setWorld( ZP_NULL );
	m_objects.eraseAll( go );
}
void zpWorld::removeObjectAtIndex( zp_uint index )
{
	m_objects[ index ]->setWorld( ZP_NULL );
	m_objects.erase( index );
}
zpObject* zpWorld::getObject( zp_uint index ) const
{
	return m_objects[ index ];
}
zp_uint zpWorld::getNumObjects() const
{
	return m_objects.size();
}

void zpWorld::create()
{

}
void zpWorld::destroy()
{

}

const zpString& zpWorld::getName() const
{
	return m_name;
}

void zpWorld::setApplication( zpApplication* application )
{
	m_application = application;
}
zpApplication* zpWorld::getApplication() const
{
	return m_application;
}


zpWorldPooledContent::zpWorldPooledContent() {}
zpWorldPooledContent::~zpWorldPooledContent() {}
