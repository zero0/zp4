#include "zpCommon.h"

zpObject::zpObject( const zp_char* filename )
	: m_transform()
	, m_name()
	, m_flags( ZP_OBJECT_FLAG_ENABLED )
	, m_components()
	, m_world( ZP_NULL )
	, m_application( ZP_NULL )
{}
zpObject::~zpObject()
{}

zpAllComponents* zpObject::getComponents()
{
	return &m_components;
}

void zpObject::setFlag( zpObjectFlag flag )
{
	m_flags.mark( flag );
	if( flag == ZP_OBJECT_FLAG_SHOULD_DESTROY )
	{
		m_flags.unmark( ZP_OBJECT_FLAG_ENABLED );
	}
}
void zpObject::unsetFlag( zpObjectFlag flag )
{
	m_flags.unmark( flag );
}
zp_bool zpObject::isFlagSet( zpObjectFlag flag ) const
{
	return m_flags.isMarked( flag );
}

const zpString& zpObject::getName() const
{
	return m_name;
}
void zpObject::setName( const zpString& name )
{
	m_name = name;
}

const zpMatrix4f& zpObject::getTransform() const
{
	return m_transform;
}
void zpObject::setTransform( const zpMatrix4f& transform )
{
	m_transform = transform;
}

zpWorld* zpObject::getWorld() const
{
	return m_world;
}

void zpObject::setWorld( zpWorld* world )
{
	m_world = world;
}

zpApplication* zpObject::getApplication() const
{
	return m_application;
}
void zpObject::setApplication( zpApplication* application )
{
	m_application = application;
}


zpObjectPooledContent::zpObjectPooledContent() {}
zpObjectPooledContent::~zpObjectPooledContent() {}

void zpObjectPooledContent::update()
{
	for( zp_uint i = 0; i < size(); ++i )
	{
		if( isUsed( i ) )
		{
			zpObject* o = at( i );
			if( o->isFlagSet( ZP_OBJECT_FLAG_SHOULD_DESTROY ) )
			{
				destroy( o );
			}
		}
	}
}