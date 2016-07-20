#include "zpCommon.h"

zpTransformComponent::zpTransformComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_localPosition( zpMath::Vector4( 0, 0, 0, 1 ) )
    , m_localRotation( zpMath::Vector4( 0, 0, 0, 1 ) )
    , m_localScale( zpMath::Vector4( 1, 1, 1, 1 ) )
    , m_parent( ZP_NULL )
{
    m_localTransform = zpMath::MatrixIdentity();
    m_worldTransform = zpMath::MatrixIdentity();

    if( !def.isNull() )
    {
        const zpBison::Value& position = def[ "Position" ];
        const zpBison::Value& rotation = def[ "Rotation" ];
        const zpBison::Value& scale = def[ "Scale" ];

        zpVector4f pos = zpMath::Vector4( 0, 0, 0, 1 );
        zpVector4f scl = zpMath::Vector4( 1, 1, 1, 0 );
        zpQuaternion4f rot = zpMath::Quaternion( 0, 0, 0, 1 );

        // position
        if( position.isArray() && !position.isEmpty() )
        {
            pos = zpMath::Vector4( position[ 0 ].asFloat(), position[ 1 ].asFloat(), position[ 2 ].asFloat(), 1.f );
        }

        // rotation
        if( rotation.isArray() && !rotation.isEmpty() )
        {
            zpScalar yaw   = zpMath::Scalar( rotation[ 0 ].asFloat() );
            zpScalar pitch = zpMath::Scalar( rotation[ 1 ].asFloat() );
            zpScalar roll  = zpMath::Scalar( rotation[ 2 ].asFloat() );
            rot = zpMath::QuaternionFromEulerAngle( yaw, pitch, roll );
        }

        // scale
        if( scale.isArray() && !scale.isEmpty() )
        {
            scl = zpMath::Vector4( scale[ 0 ].asFloat(), scale[ 1 ].asFloat(), scale[ 2 ].asFloat(), 1.f );
        }

        // build local matrix
        setLocalTransform( pos, rot, scl );
    
        // build world matrix so it's correct on the same frame of creation
        onUpdate( 0.f, 0.f );

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
                ZP_ASSERT( o != ZP_NULL, "Failed to create child object" );

                o->initialize();

                addChild( o->getComponents()->getTransformComponent() );
            } );
        }
    }
}
zpTransformComponent::~zpTransformComponent()
{
}

void zpTransformComponent::setLocalTransform( zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation )
{
    m_localPosition = localPosition;
    m_localRotation = localRotation;

    m_localTransform = zpMath::TRS( m_localPosition, m_localRotation, m_localScale );
    getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );

}
void zpTransformComponent::setLocalTransform( zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation, zpVector4fParamF localScale )
{
    m_localPosition = localPosition;
    m_localRotation = localRotation;
    m_localScale = localScale;

    m_localTransform = zpMath::TRS( m_localPosition, m_localRotation, m_localScale );
    getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}

void zpTransformComponent::setLocalPosition( zpVector4fParamF localPosition )
{
    m_localPosition = localPosition;

    m_localTransform = zpMath::TRS( m_localPosition, m_localRotation, m_localScale );
    getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}
void zpTransformComponent::setLocalScale( zpVector4fParamF localScale )
{
    m_localScale = localScale;

    m_localTransform = zpMath::TRS( m_localPosition, m_localRotation, m_localScale );
    getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}
void zpTransformComponent::setLocalRotation( zpQuaternion4fParamF localRotation )
{
    m_localRotation = localRotation;

    m_localTransform = zpMath::TRS( m_localPosition, m_localRotation, m_localScale );
    getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}

zpVector4f zpTransformComponent::getLocalPosition() const
{
    return m_localPosition;
}
zpQuaternion4f zpTransformComponent::getLocalRotation() const
{
    return m_localRotation;
}
zpVector4f zpTransformComponent::getLocalScale() const
{
    return m_localScale;
}

zpVector4f zpTransformComponent::getWorldPosition() const
{
    return m_worldTransform.m_m4;
}

zpMatrix4f zpTransformComponent::getWorldTransform() const
{
    return m_worldTransform;
}
zpMatrix4f zpTransformComponent::getLocalTransform() const
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
        zp_size_t i = m_parent->findChildDirect( this );
        m_parent->removeChild( i );
    }

    m_parent = parent;

    if( m_parent != ZP_NULL )
    {
        m_parent->addChild( this );
    }

    // mark that the world transform changed
    getParentObject()->setFlag( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
}
zpTransformComponent* zpTransformComponent::getParent() const
{
    return m_parent;
}

zp_size_t zpTransformComponent::getChildCount() const
{
    return m_children.size();
}
zpTransformComponent* zpTransformComponent::getChild( zp_size_t index ) const
{
    return m_children[ index ];
}

zpTransformComponent* zpTransformComponent::removeChild( zp_size_t index )
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
        zp_size_t i = child->m_parent->findChildDirect( child );
        child->m_parent->removeChild( i );
    }

    m_children.pushBack( child );

    child->m_parent = this;
}
void zpTransformComponent::addChild( zpTransformComponent* child, zpVector4fParamF localPosition )
{
    addChild( child );
    child->setLocalPosition( localPosition );
}
void zpTransformComponent::addChild( zpTransformComponent* child, zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation )
{
    addChild( child );
    child->setLocalTransform( localPosition, localRotation );
}
void zpTransformComponent::addChild( zpTransformComponent* child, zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation, zpVector4fParamF localScale )
{
    addChild( child );
    child->setLocalTransform( localPosition, localRotation, localScale );
}

void zpTransformComponent::translate( zpVector4fParamF dir )
{
    zpVector4f pos;
    pos = zpMath::Vector4Add( m_localPosition, dir );
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
    zpTransformComponent** b = m_children.begin();
    zpTransformComponent** e = m_children.end();
    for( ; b != e; ++b )
    {
        (*b)->getParentObject()->destroy();
    }

    m_children.clear();
}

void zpTransformComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    zp_bool dirty = getParentObject()->isFlagSet( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
    if( dirty )
    {
        m_worldTransform = m_localTransform;

        // calculate world position based on parents
        zpTransformComponent* p = m_parent;
        while( p != ZP_NULL )
        {
            m_worldTransform = zpMath::MatrixMul( m_worldTransform, p->getLocalTransform() );

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
    zpTransformComponent** b = m_children.begin();
    zpTransformComponent** e = m_children.end();
    for( ; b != e; ++b )
    {
        (*b)->setEnabled( true );
    }
}
void zpTransformComponent::onDisabled()
{
    zpTransformComponent** b = m_children.begin();
    zpTransformComponent** e = m_children.end();
    for( ; b != e; ++b )
    {
        (*b)->setEnabled( false );
    }
}

zp_size_t zpTransformComponent::findChildDirect( zpTransformComponent* child )
{
    zp_size_t i = m_children.indexOf( child );
    return i;
}


zpTransformOctreeNode::zpTransformOctreeNode()
    : m_isLeaf( true )
    , m_maxObjectCount( ZP_TRANSFORM_OCTREE_MAX_OBJECTS )
    , m_tree( ZP_NULL )
    , m_parent( ZP_NULL )
{}
zpTransformOctreeNode::~zpTransformOctreeNode()
{}
zp_bool zpTransformOctreeNode::insert( zpTransformComponent* obj )
{
    zpVector4f pos = obj->getWorldPosition();

    // if there is no collision in with the node, return false
    if( !ZP_IS_COLLISION( m_bounds, pos ) )
    {
        return false;
    }

    // if the node is a leaf and it was successfully added, return true
    if( m_isLeaf && add( obj ) )
    {
        return true;
    }

    // if there are no children, subdivide the node since it failed to add
    if( m_children.isEmpty() )
    {
        subdivide();
    }

    // if the node is a leaf and there is no room left of objects, convert to a link
    if( m_isLeaf && m_objects.size() == m_maxObjectCount )
    {
        m_isLeaf = false;
    }

    // if the node is a link and there are objects, push all objects to children
    if( !m_isLeaf && m_objects.size() > 0 )
    {
        zpTransformComponent** b = m_objects.begin();
        zpTransformComponent** e = m_objects.begin();
        for( ; b != e; ++b )
        {
            zpTransformComponent* o = *b;

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
zp_bool zpTransformOctreeNode::remove( zpTransformComponent* obj )
{
    zp_bool removed = false;

    // if leaf, remove from objects
    if( m_isLeaf )
    {
        zp_size_t index = m_objects.indexOf( obj );
        if( index != zpArrayList< zpTransformComponent* >::npos )
        {
            m_objects.erase( index );
            removed = true;
        }
    }
    // otherwise, try removing from children
    else
    {
        zpTransformOctreeNode** b = m_children.begin();
        zpTransformOctreeNode** e = m_children.end();
        for( zp_int i = 0; b != e && !removed; ++b, ++i )
        {
            removed = (*b)->remove( obj );
        }

        // if the object was removed, see if there are any objects left in the tree
        if( removed )
        {
            // if the count is zero, remove children and become a leaf again
            zp_size_t count = getObjectCount();
            if( count == 0 )
            {
                // convert back to a leaf
                m_isLeaf = true;

                // destroy child nodes
                zpTransformOctreeNode** b = m_children.begin();
                zpTransformOctreeNode** e = m_children.end();
                for( zp_int i = 0; b != e && !removed; ++b, ++i )
                {
                    zpTransformOctreeNode* node = *b;
                    node->teardown();
                    m_tree->destroyNode( node );
                }

                m_children.clear();
            }
        }
    }

    return removed;
}

void zpTransformOctreeNode::setup( const zpBoundingAABB& bounds, zpTransformOctree* tree, zpTransformOctreeNode* parent )
{
    m_bounds = bounds;
    m_tree = tree;
    m_parent = parent;
}
void zpTransformOctreeNode::teardown()
{

}

const zpBoundingAABB& zpTransformOctreeNode::getBounts() const
{
    return m_bounds;
}

zp_size_t zpTransformOctreeNode::getObjectCount() const
{
    zp_size_t count = 0;

    if( m_isLeaf )
    {
        count = m_objects.size();
    }
    else
    {
        const zpTransformOctreeNode* const* b = m_children.begin();
        const zpTransformOctreeNode* const* e = m_children.end();
        for( ; b != e; ++b )
        {
            count += (*b)->getObjectCount();
        }
    }

    return count;
}

zp_bool zpTransformOctreeNode::add( zpTransformComponent* obj )
{
    if( m_objects.size() < m_maxObjectCount )
    {
        m_objects.pushBack( obj );
        return true;
    }

    return false;
}
void zpTransformOctreeNode::subdivide()
{
    for( zp_uint i = 0; i < zpTransformOctreeNodeSide_Count; ++i )
    {
        zpTransformOctreeNode* node = m_tree->createNode();
        m_children.pushBack( node );
    }

    zpVector4f center = m_bounds.getCenter();
    zpVector4f halfSize = m_bounds.getExtents();
    zpVector4f quartSize;
    zpVector4f c;

    static const zpVector4f offsets[] =
    {
        zpMath::Vector4(  1,  1,  1, 0 ),
        zpMath::Vector4( -1,  1,  1, 0 ),
        zpMath::Vector4(  1,  1, -1, 0 ),
        zpMath::Vector4( -1,  1, -1, 0 ),
        zpMath::Vector4(  1, -1,  1, 0 ),
        zpMath::Vector4( -1, -1,  1, 0 ),
        zpMath::Vector4(  1, -1, -1, 0 ),
        zpMath::Vector4( -1, -1, -1, 0 ),
    };

    quartSize = zpMath::Vector4Scale( halfSize, zpMath::Scalar( 0.5f ) );

    zpTransformOctreeNode** b = m_children.begin();
    zpTransformOctreeNode** e = m_children.end();
    for( zp_size_t i = 0; b != e; ++b, ++i )
    {
        c = zpMath::Vector4Mul( offsets[ i ], quartSize );
        c = zpMath::Vector4Add( c, center );

        zpBoundingAABB bounds;
        bounds.setCenter( c );
        bounds.setExtents( quartSize );

        zpTransformOctreeNode* n = *b;
        n->setup( bounds, m_tree, this );
    }
}


void zpTransformOctree::insert( zpTransformComponent* obj )
{
    // if there is no root, create one around the first object
    if( m_root == ZP_NULL )
    {
        m_root = createNode();

        zpBoundingAABB bounds;
        bounds.setCenter( obj->getWorldPosition() );
        bounds.setExtents( zpMath::Vector4( 100.f, 100.f, 100.f, 0 ) );

        m_root->setup( bounds, this, ZP_NULL );
    }

    zp_bool insertedIntoTree = m_root->insert( obj );

    // if not inserted into tree, expand tree to surround the item
    if( !insertedIntoTree )
    {

    }
}
void zpTransformOctree::remove( zpTransformComponent* obj )
{
    m_root->remove( obj );
}

void zpTransformOctree::setup()
{
    m_nodes.resize( ZP_TRANSFORM_OCTREE_MAX_NODES );

    zpTransformOctreeNode* b = m_nodes.begin();
    zpTransformOctreeNode* e = m_nodes.end();
    for( ; b != e; ++b )
    {
        m_freeNodes.pushBack( b );
    }
}
void zpTransformOctree::teardown()
{
    m_root->teardown();

    destroyNode( m_root );
    m_root = ZP_NULL;
}

const zpBoundingAABB& zpTransformOctree::getBounts() const
{
    return m_root->getBounts();
}

zp_size_t zpTransformOctree::getObjectCount() const
{
    return m_root->getObjectCount();
}

zpTransformOctreeNode* zpTransformOctree::createNode()
{
    zpTransformOctreeNode* node = m_freeNodes.back();
    m_freeNodes.popBack();
    m_usedNodes.pushBack( node );

    return node;
}
void zpTransformOctree::destroyNode( zpTransformOctreeNode* node )
{
    zp_size_t index = m_usedNodes.indexOf( node );
    m_usedNodes.erase( index );
    m_freeNodes.pushBack( node );
}

void zpTransformOctree::update()
{

}


zpTransformComponentPool::zpTransformComponentPool()
{
    m_octree.setup();
}
zpTransformComponentPool::~zpTransformComponentPool()
{
    m_octree.teardown();
}

void zpTransformComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    // update transforms
    zpTransformComponent** b = m_used.begin();
    zpTransformComponent** e = m_used.end();
    for( ; b != e; ++b )
    {
        (*b)->update( deltaTime, realTime );
    }

    // update octree
    m_octree.update();
}

void zpTransformComponentPool::onCreate( zpTransformComponent* t )
{
    m_octree.insert( t );
}
void zpTransformComponentPool::onDestroy( zpTransformComponent* t )
{
    m_octree.remove( t );
}
