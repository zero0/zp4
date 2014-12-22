#include "zpCommon.h"

zpAttachmentComponent::zpAttachmentComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	const zpBison::Value& children = def[ "Children" ];
	if( children.isArray() && !children.isEmpty() )
	{
		zpObjectContentManager* manager = getApplication()->getObjectContentManager();

		m_children.reserve( children.size() );
		children.foreachArray( [ this, manager ]( const zpBison::Value& child )
		{
			const zp_char* objName = child.asCString();

			zpObject* o = manager->createObject( objName );
			addChild( o );
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
		o->destroy();
	} );
	m_children.clear();
}
	
void zpAttachmentComponent::onUpdate()
{
	m_worldTransform = getParentObject()->getTransform();

	zpObject* p = m_parent;
	while( p != ZP_NULL )
	{
		zpMath::Mul( m_worldTransform, m_worldTransform, p->getTransform() );

		zpAttachmentComponent* a = p->getComponents()->getAttachmentComponent();
		p = a == ZP_NULL ? ZP_NULL : a->m_parent;
	}
}
void zpAttachmentComponent::onSimulate()
{
}

void zpAttachmentComponent::onEnabled()
{
	m_children.foreach( []( zpObject* o )
	{
		o->setEnabled( true );
	} );
}
void zpAttachmentComponent::onDisabled()
{
	m_children.foreach( []( zpObject* o )
	{
		o->setEnabled( false );
	} );
}

zpObject* zpAttachmentComponent::getParent() const
{
	return m_parent;
}

const zpArrayList< zpObject* >& zpAttachmentComponent::getChildren() const
{
	return m_children;
}

zp_uint zpAttachmentComponent::getChildCount() const
{
	return m_children.size();
}
zpObject* zpAttachmentComponent::getChild( zp_uint index ) const
{
	return m_children[ index ];
}

zpObject* zpAttachmentComponent::removeChild( zp_uint index, zp_bool shouldDestroy )
{
	zpObject* o = m_children[ index ];
	m_children.erase( index );

	zpAttachmentComponent* attachment = o->getComponents()->getAttachmentComponent();
	if( attachment != ZP_NULL )
	{
		attachment->m_parent = ZP_NULL;
	}

	if( shouldDestroy )
	{
		o->destroy();
	}
	return o;
}
void zpAttachmentComponent::addChild( zpObject* child )
{
	m_children.pushBack( child );

	// add attachment component to child to set the parent
	zpAttachmentComponent* attachment = ZP_NULL;
	attachment = child->getComponents()->getAttachmentComponent();
	if( attachment == ZP_NULL )
	{
		attachment = child->getComponents()->addAttachmentComponent( zpBison::null );
	}

	attachment->m_parent = getParentObject();
}
void zpAttachmentComponent::addChild( zpObject* child, const zpMatrix4f& localTransform )
{
	child->setTransform( localTransform );
	addChild( child );
}

const zpMatrix4f& zpAttachmentComponent::getWorldTransform() const
{
	return m_worldTransform;
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
