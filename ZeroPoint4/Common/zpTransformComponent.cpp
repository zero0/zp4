#include "zpCommon.h"

zpTransformComponent::zpTransformComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_localPosition( 0, 0, 0, 1 )
	, m_localRotation( 0, 0, 0, 1 )
	, m_localScale( 1, 1, 1, 1 )
	, m_parent( ZP_NULL )
{
	m_localTransform.setIdentity();
	m_worldTransform.setIdentity();

	if( !def.isNull() )
	{
		const zpBison::Value& position = def[ "Position" ];
		const zpBison::Value& rotation = def[ "Rotation" ];
		const zpBison::Value& scale = def[ "Scale" ];

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
	
		// build world matrix so it's correct on the same frame of creation
		onUpdate();

		// add children
		const zpBison::Value& children = def[ "Children" ];
		if( children.isArray() && !children.isEmpty() )
		{
			zpObjectContentManager* manager = getApplication()->getObjectContentManager();
	
			m_children.reserve( children.size() );
			children.foreachArray( [ this, manager ]( const zpBison::Value& child )
			{
				zpObject* o = ZP_NULL;
				if( child.isString() )
				{
					const zp_char* objName = child.asCString();
					o = manager->createObject( objName );
				}
				else if( child.isObject() )
				{
					o = manager->createObject( child );
				}

				o->initialize();

				ZP_ASSERT( o != ZP_NULL, "Failed to create child object" );
				addChild( o->getComponents()->getTransformComponent() );
			} );
		}
	}
}
zpTransformComponent::~zpTransformComponent()
{
}

void zpTransformComponent::setLocalTransform( const zpVector4f& localPosition, const zpQuaternion4f& localRotation )
{
	m_localPosition = localPosition;
	m_localRotation = localRotation;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
	getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );

}
void zpTransformComponent::setLocalTransform( const zpVector4f& localPosition, const zpQuaternion4f& localRotation, const zpVector4f& localScale )
{
	m_localPosition = localPosition;
	m_localRotation = localRotation;
	m_localScale = localScale;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
	getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}

void zpTransformComponent::setLocalPosition( const zpVector4f& localPosition )
{
	m_localPosition = localPosition;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
	getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}
void zpTransformComponent::setLocalScale( const zpVector4f& localScale )
{
	m_localScale = localScale;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
	getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}
void zpTransformComponent::setLocalRotation( const zpQuaternion4f& localRotation )
{
	m_localRotation = localRotation;

	zpMath::TRS( m_localTransform, m_localPosition, m_localRotation, m_localScale );
	getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}

const zpVector4f& zpTransformComponent::getLocalPosition() const
{
	return m_localPosition;
}
const zpQuaternion4f& zpTransformComponent::getLocalRotation() const
{
	return m_localRotation;
}
const zpVector4f& zpTransformComponent::getLocalScale() const
{
	return m_localScale;
}

const zpVector4f& zpTransformComponent::getWorldPosition() const
{
	return m_worldTransform.getRow( 3 );
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

void zpTransformComponent::setParent( zpTransformComponent* parent )
{
	// if parent defined, add this as a child (handles old parent removal)
	if( parent != ZP_NULL )
	{
		parent->addChild( this );
	}
	// otherwise, remove child of old parent and clear parent
	else if( m_parent != ZP_NULL )
	{
		zp_int i = m_parent->findChildDirect( this );
		m_parent->removeChild( i );

		m_parent = ZP_NULL;
	}

	// mark that the world transform changed
	getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
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

zpTransformComponent* zpTransformComponent::removeChild( zp_uint index )
{
	zpTransformComponent* child = m_children[ index ];
	m_children.erase( index );

	child->m_parent = ZP_NULL;

	return child;
}

void zpTransformComponent::addChild( zpTransformComponent* child )
{
	if( child->m_parent != ZP_NULL )
	{
		zp_int i = child->m_parent->findChildDirect( child );
		child->m_parent->removeChild( i );
	}

	m_children.pushBack( child );

	child->m_parent = this;
}
void zpTransformComponent::addChild( zpTransformComponent* child, const zpVector4f& localPosition )
{
	addChild( child );
	child->setLocalPosition( localPosition );
}
void zpTransformComponent::addChild( zpTransformComponent* child, const zpVector4f& localPosition, const zpQuaternion4f& localRotation )
{
	addChild( child );
	child->setLocalTransform( localPosition, localRotation );
}
void zpTransformComponent::addChild( zpTransformComponent* child, const zpVector4f& localPosition, const zpQuaternion4f& localRotation, const zpVector4f& localScale )
{
	addChild( child );
	child->setLocalTransform( localPosition, localRotation, localScale );
}

void zpTransformComponent::translate( const zpVector4f& dir )
{
	zpVector4f pos;
	zpMath::Add( pos, m_localPosition, dir );
	setLocalPosition( pos );
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
	//if( getParentObject()->isFlagSet( ZP_OBJECT_FLAG_TRANSFORM_DIRTY ) )
	{
		m_worldTransform = m_localTransform;

		// calculate world position based on parents
		zpTransformComponent* p = m_parent;
		while( p != ZP_NULL )
		{
			zpMath::Mul( m_worldTransform, m_worldTransform, p->getLocalTransform() );

			zpTransformComponent* a = p->getParentObject()->getComponents()->getTransformComponent();
			p = a == ZP_NULL ? ZP_NULL : a->m_parent;
		}
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

zp_int zpTransformComponent::findChildDirect( zpTransformComponent* child )
{
	zp_int i = m_children.indexOf( child );
	return i;
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
