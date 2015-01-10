#include "zpCommon.h"

zpOctree::zpOctree()
{
}
zpOctree::zpOctree( const zpBoundingAABB& bounds )
	: m_root( bounds )
{
}
zpOctree::~zpOctree()
{
}

void zpOctree::update()
{
	zpArrayList< zpObject* > updatedObjects;

	// update the root
	m_root.update( updatedObjects );

	// add back any moved objects
	updatedObjects.foreach( [ this ]( zpObject* o ) {
		insert( o );
	} );
}

zp_uint zpOctree::getNumObjects() const
{
	return m_root.getNumObjects();
}

void zpOctree::insert( zpObject* obj )
{
	m_root.insert( obj );
}


zpOctreeNode::zpOctreeNode()
	: m_type( ZP_OCTREE_NODE_TYPE_LEAF )
	, m_id( 0 )
	, m_maxObjectCount( 32 )
{
}
zpOctreeNode::zpOctreeNode( const zpBoundingAABB& bounds )
	: m_type( ZP_OCTREE_NODE_TYPE_LEAF )
	, m_id( 0 )
	, m_maxObjectCount( 32 )
	, m_bounds( bounds )
{
}
zpOctreeNode::~zpOctreeNode()
{
	m_children.foreach( []( zpOctreeNode* node ) {
		delete node;
	} );
}

zp_uint zpOctreeNode::update( zpArrayList< zpObject* >& updatedObjects )
{
	if( m_type == ZP_OCTREE_NODE_TYPE_LEAF )
	{
		for( zp_uint i = 0; i < m_objects.size(); )
		{
			zpObject* obj = m_objects[ i ];

			const zpVector4f& pos = obj->getComponents()->getTransformComponent()->getWorldPosition();

			// if the object should be destroyed, remove it from being tracked
			if( obj->isFlagSet( ZP_OBJECT_FLAG_SHOULD_DESTROY ) )
			{
				m_objects.erase( i );
				continue;
			}
			// otherwise, if the object is no longer in the bounds, remove it so it could be re-added
			else if( !ZP_IS_COLLISION( m_bounds, pos ) )
			{
				m_objects.erase( i );
				updatedObjects.pushBack( obj );
				continue;
			}

			++i;
		}
	}
	else
	{
		zp_uint numChildObjects = 0;

		// update children
		m_children.foreach( [ &numChildObjects, &updatedObjects ]( zpOctreeNode* node ) {
			numChildObjects += node->update( updatedObjects );
		} );

		// if there are no more objects in the child nodes, become a leaf again
		if( numChildObjects == 0 )
		{
			m_children.foreach( []( zpOctreeNode* node ) {
				delete node;
			} );
			m_children.destroy();

			m_type = ZP_OCTREE_NODE_TYPE_LEAF;
		}
	}

	return m_objects.size();
}

zp_uint zpOctreeNode::getNumObjects() const
{
	zp_uint count = m_objects.size();
	return count;
}

zp_bool zpOctreeNode::insert( zpObject* obj )
{
	const zpVector4f& pos = obj->getComponents()->getTransformComponent()->getWorldPosition();

	// if there is no collision in with the node, return false
	if( !ZP_IS_COLLISION( m_bounds, pos ) )
	{
		return false;
	}

	// if the node is a leaf and it was successfully added, return true
	if( m_type == ZP_OCTREE_NODE_TYPE_LEAF && add( obj ) )
	{
		return true;
	}

	// if there are no children, subdivide the node since it failed to add
	if( m_children.isEmpty() )
	{
		subdivide();
	}

	// if the node is a leaf and there is no room left of objects, convert to a link
	if( m_type == ZP_OCTREE_NODE_TYPE_LEAF && m_objects.size() == m_maxObjectCount )
	{
		m_type = ZP_OCTREE_NODE_TYPE_LINK;
	}

	// if the node is a link and there are objects, push all objects to children
	if( m_type == ZP_OCTREE_NODE_TYPE_LINK && m_objects.size() > 0 )
	{
		zpObject** b = m_objects.begin();
		zpObject** e = m_objects.begin();
		for( ; b != e; ++b )
		{
			zpObject* o = *b;

			if( m_children[ TNW ]->insert( o ) ) continue;
			if( m_children[ TNE ]->insert( o ) ) continue;
			if( m_children[ TSW ]->insert( o ) ) continue;
			if( m_children[ TSE ]->insert( o ) ) continue;

			if( m_children[ BNW ]->insert( o ) ) continue;
			if( m_children[ BNE ]->insert( o ) ) continue;
			if( m_children[ BSW ]->insert( o ) ) continue;
			if( m_children[ BSE ]->insert( o ) ) continue;
		}

		// destroy the objects list to not leak memory
		m_objects.destroy();
	}

	if( m_children[ TNW ]->insert( obj ) ) return true;
	if( m_children[ TNE ]->insert( obj ) ) return true;
	if( m_children[ TSW ]->insert( obj ) ) return true;
	if( m_children[ TSE ]->insert( obj ) ) return true;

	if( m_children[ BNW ]->insert( obj ) ) return true;
	if( m_children[ BNE ]->insert( obj ) ) return true;
	if( m_children[ BSW ]->insert( obj ) ) return true;
	if( m_children[ BSE ]->insert( obj ) ) return true;

	return false;
}

const zpBoundingAABB& zpOctreeNode::getBounds() const
{
	return m_bounds;
}

zp_bool zpOctreeNode::add( zpObject* obj )
{
	if( m_objects.size() < m_maxObjectCount )
	{
		m_objects.pushBack( obj );
		return true;
	}

	return false;
}
void zpOctreeNode::subdivide()
{
	// create child nodes
	m_children.reserve( zpOctreeNodeSide_Count );
	for( zp_uint i = 0; i < zpOctreeNodeSide_Count; ++i )
	{
		m_children.pushBack( new zpOctreeNode );
	}

	zpVector4f center = m_bounds.getCenter();
	zpVector4f halfSize = m_bounds.getExtents();
	zpVector4f quartSize;
	zpVector4f c;

	static const zpVector4f offsets[] =
	{
		zpVector4f(  1,  1,  1 ),
		zpVector4f( -1,  1,  1 ),
		zpVector4f(  1,  1, -1 ),
		zpVector4f( -1,  1, -1 ),
		zpVector4f(  1, -1,  1 ),
		zpVector4f( -1, -1,  1 ),
		zpVector4f(  1, -1, -1 ),
		zpVector4f( -1, -1, -1 ),
	};

	zpMath::Mul( quartSize, halfSize, zpScalar( 0.5f ) );

	for( zp_int i = 0; i < zpOctreeNodeSide_Count; ++i )
	{
		zpMath::Mul( c, offsets[ i ], quartSize );
		zpMath::Add( c, c, center );

		zpOctreeNode* n = m_children[ i ];
		n->m_bounds.setCenter( c );
		n->m_bounds.setExtents( halfSize );
	}
}
