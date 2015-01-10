#include "zpCommon.h"

zpTransformComponent::zpTransformComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	const zpBison::Value& position = def[ "Position" ];
	const zpBison::Value& rotation = def[ "Rotation" ];
	const zpBison::Value& scale = def[ "Scale" ];

	m_localTransform.setIdentity();

	zpVector4f pos( 0, 0, 0, 1 );
	zpVector4f scl( 1, 1, 1, 0 );
	zpQuaternion4f rot( 0, 0, 0, 1 );

	// position
	if( position.isArray() && !position.isEmpty() )
	{
		pos = zpVector4f( position[ 0 ].asFloat(), position[ 1 ].asFloat(), position[ 2 ].asFloat(), 1.f );
	}

	// rotation
	if( rotation.isArray() && !rotation.isEmpty() )
	{
		zpScalar yaw(   rotation[ 0 ].asFloat() );
		zpScalar pitch( rotation[ 1 ].asFloat() );
		zpScalar roll(  rotation[ 2 ].asFloat() );
		zpMath::SetEulerAngle( rot, yaw, pitch, roll );
	}

	// scale
	if( scale.isArray() && !scale.isEmpty() )
	{
		scl = zpVector4f( scale[ 0 ].asFloat(), scale[ 1 ].asFloat(), scale[ 2 ].asFloat(), 1.f );
	}

	// build local matrix
	setLocalTransform( pos, rot, scl );
	
	// build world matrix
	onUpdate();

	// add children
	const zpBison::Value& children = def[ "Children" ];
	if( children.isArray() && !children.isEmpty() )
	{
		zpObjectContentManager* manager = getApplication()->getObjectContentManager();
	
		m_children.reserve( children.size() );
		children.foreachArray( [ this, manager ]( const zpBison::Value& child )
		{
			const zp_char* objName = child.asCString();
	
			zpObject* o = manager->createObject( objName );
			addChild( o->getComponents()->getTransformComponent() );
		} );
	}
}
zpTransformComponent::~zpTransformComponent()
{
}

//void zpTransformComponent::setLocalTransform( const zpMatrix4f& localTransform )
//{
//	m_localTransform = localTransform;
//}
void zpTransformComponent::setLocalTransform( const zpVector4f& localPosition, const zpQuaternion4f& localRotation, const zpVector4f& localScale )
{
	m_localPosition = localPosition;
	m_localRotation = localRotation;
	m_localScale = localScale;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
}

void zpTransformComponent::setLocalPosition( const zpVector4f& localPosition )
{
	m_localPosition = localPosition;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
}
void zpTransformComponent::setLocalScale( const zpVector4f& localScale )
{
	m_localScale = localScale;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
}
void zpTransformComponent::setLocalRotation( const zpQuaternion4f& localRotation )
{
	m_localRotation = localRotation;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
}

const zpMatrix4f& zpTransformComponent::getWorldTransform() const
{
	return m_worldTransform;
}
const zpMatrix4f& zpTransformComponent::getLocalTransform() const
{
	return m_localTransform;
}

const zpArrayList< zpTransformComponent* >& zpTransformComponent::getChildren() const
{
	return m_children;
}

zpTransformComponent* zpTransformComponent::getParent() const
{
	return m_parent;
}

zp_uint zpTransformComponent::getChildCount() const
{
	return m_children.size();
}
zpTransformComponent* zpTransformComponent::getChild( zp_uint index ) const
{
	return m_children[ index ];
}

zpTransformComponent* zpTransformComponent::removeChild( zp_uint index, zp_bool shouldDestroy )
{
	zpTransformComponent* o = m_children[ index ];
	m_children.erase( index );

	zpTransformComponent* attachment = o->getParentObject()->getComponents()->getTransformComponent();
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

void zpTransformComponent::addChild( zpTransformComponent* child )
{
	m_children.pushBack( child );

	// add attachment component to child to set the parent
	zpTransformComponent* attachment = ZP_NULL;
	attachment = child->getParentObject()->getComponents()->getTransformComponent();
	if( attachment == ZP_NULL )
	{
		attachment = child->getParentObject()->getComponents()->addTransformComponent( zpBison::null );
	}

	attachment->m_parent = this;
}
void zpTransformComponent::addChild( zpTransformComponent* child, const zpMatrix4f& localTransform )
{
	m_localTransform = localTransform;
	addChild( child );
}

void zpTransformComponent::translate( const zpVector4f& dir )
{
	zpVector4f pos( m_localTransform.getRow( 3 ) );
	zpMath::Add( pos, pos, dir );
	m_localTransform.setRow( 3, pos );
}

const zpVector4f& zpTransformComponent::getLocalPosition() const
{
	return m_localTransform.getRow( 3 );
}
const zpVector4f& zpTransformComponent::getWorldPosition() const
{
	return m_worldTransform.getRow( 3 );
}

void zpTransformComponent::onCreate()
{

}
void zpTransformComponent::onInitialize()
{

}
void zpTransformComponent::onDestroy()
{
	m_children.foreach( []( zpTransformComponent* o )
	{
		o->getParentObject()->destroy();
	} );
	m_children.clear();
}

void zpTransformComponent::onUpdate()
{
	m_worldTransform = m_localTransform;

	zpTransformComponent* p = m_parent;
	while( p != ZP_NULL )
	{
		zpMath::Mul( m_worldTransform, m_worldTransform, p->getLocalTransform() );

		zpTransformComponent* a = p->getParentObject()->getComponents()->getTransformComponent();
		p = a == ZP_NULL ? ZP_NULL : a->m_parent;
	}
}
void zpTransformComponent::onSimulate()
{
}

void zpTransformComponent::onEnabled()
{
	m_children.foreach( []( zpTransformComponent* o )
	{
		o->getParentObject()->setEnabled( true );
	} );
}
void zpTransformComponent::onDisabled()
{
	m_children.foreach( []( zpTransformComponent* o )
	{
		o->getParentObject()->setEnabled( false );
	} );
}

zpTransformComponentPool::zpTransformComponentPool()
{
}
zpTransformComponentPool::~zpTransformComponentPool()
{
}

void zpTransformComponentPool::update()
{
	m_used.foreach( []( zpTransformComponent* o )
	{
		o->update();
	} );
}
