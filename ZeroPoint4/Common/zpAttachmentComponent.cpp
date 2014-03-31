#include "zpCommon.h"

zpAttachmentComponent::zpAttachmentComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	const zpBison::Value& children = def[ "Children" ];
	if( children.isArray() && !children.isEmpty() )
	{
		m_children.reserve( children.size() );
		children.foreachArray( [ this ]( const zpBison::Value& child )
		{
			const zp_char* objName = child.asCString();
			zpObject* o = getApplication()->getObjectContentManager()->createObject( getApplication(), objName );
			m_children.pushBack( o );
		} );
	}
}
zpAttachmentComponent::~zpAttachmentComponent()
{
}

void zpAttachmentComponent::onCreate()
{

}
void zpAttachmentComponent::onInitialize()
{

}
void zpAttachmentComponent::onDestroy()
{
	m_children.foreach( []( zpObject* o )
	{
		o->setFlag( ZP_OBJECT_FLAG_SHOULD_DESTROY );
	} );
	m_children.clear();
}
	
void zpAttachmentComponent::onUpdate()
{
	zpMatrix4f local;
	m_children.foreach( [ &local, this ]( zpObject* o )
	{
		zpMath::Mul( local, getParentObject()->getTransform(), o->getTransform() );
		o->setTransform( local );
	} );
}
void zpAttachmentComponent::onSimulate()
{
}

void zpAttachmentComponent::onEnabled()
{
}
void zpAttachmentComponent::onDisabled()
{
}

const zpArrayList< zpObject* >& zpAttachmentComponent::getChildren() const
{
	return m_children;
}

zp_uint zpAttachmentComponent::getChildCount() const
{
	return m_children.size();
}
zpObject* zpAttachmentComponent::getChildAt( zp_uint index ) const
{
	return m_children[ index ];
}

zpObject* zpAttachmentComponent::removeChild( zp_uint index )
{
	zpObject* o = m_children[ index ];
	m_children.erase( index );
	return o;
}
void zpAttachmentComponent::addChild( zpObject* child )
{
	m_children.pushBack( child );
}


zpAttachmentComponentPool::zpAttachmentComponentPool()
{
}
zpAttachmentComponentPool::~zpAttachmentComponentPool()
{
}

void zpAttachmentComponentPool::update()
{
	m_used.foreach( []( zpAttachmentComponent* o )
	{
		o->update();
	} );
}
