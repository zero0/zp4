#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

class zpTransformComponent : public zpComponent
{
public:
	zpTransformComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpTransformComponent();

	void setLocalTransform( zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation );
	void setLocalTransform( zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation, zpVector4fParamF localScale );

	void setLocalPosition( zpVector4fParamF localPosition );
	void setLocalScale( zpVector4fParamF localScale );
	void setLocalRotation( zpQuaternion4fParamF localRotation );

	zpVector4f getLocalPosition() const;
	zpQuaternion4f getLocalRotation() const;
	zpVector4f getLocalScale() const;

	zpVector4f getWorldPosition() const;

	zpMatrix4f getWorldTransform() const;
	zpMatrix4f getLocalTransform() const;

	const zpArrayList< zpTransformComponent* >& getChildren() const;

	void setParent( zpTransformComponent* parent );
	zpTransformComponent* getParent() const;

	zp_uint getChildCount() const;
	zpTransformComponent* getChild( zp_uint index ) const;

	zpTransformComponent* removeChild( zp_uint index );

	void addChild( zpTransformComponent* child );
	void addChild( zpTransformComponent* child, zpVector4fParamF localPosition );
	void addChild( zpTransformComponent* child, zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation );
	void addChild( zpTransformComponent* child, zpVector4fParamF localPosition, zpQuaternion4fParamF localRotation, zpVector4fParamF localScale );

	void translate( zpVector4fParamF dir );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_int findChildDirect( zpTransformComponent* child );

	zpVector4f m_localPosition;
	zpQuaternion4f m_localRotation;
	zpVector4f m_localScale;

	zpMatrix4f m_localTransform;
	zpMatrix4f m_worldTransform;

	zpTransformComponent* m_parent;
	zpArrayList< zpTransformComponent* > m_children;
};

class zpTransformOctree;

class zpTransformOctreeNode
{
public:
	zpTransformOctreeNode();
	~zpTransformOctreeNode();

	zp_bool insert( zpTransformComponent* obj );
	zp_bool remove( zpTransformComponent* obj );

	void setup( const zpBoundingAABB& bounds, zpTransformOctree* tree, zpTransformOctreeNode* parent );
	void teardown();

	const zpBoundingAABB& getBounts() const;

	zp_int getObjectCount() const;

	template<typename Func>
	void foreachNode( Func func )
	{
		func( this );
	
		if( !m_isLeaf )
		{
			zpTransformOctreeNode** b = m_children.begin();
			zpTransformOctreeNode** e = m_children.end();
			for( ; b != e; ++b )
			{
				zpTransformOctreeNode* node = *b;
				node->foreachNode( func );
			}
		}
	}

private:
	enum zpTransformOctreeNodeSide
	{
		TNW,
		TNE,
		TSW,
		TSE,

		BNW,
		BNE,
		BSW,
		BSE,

		zpTransformOctreeNodeSide_Count,
	};

	zp_bool add( zpTransformComponent* obj );
	void subdivide();

	zp_bool m_isLeaf;
	zp_uint m_maxObjectCount;

	zpBoundingAABB m_bounds;

	zpTransformOctree* m_tree;
	zpTransformOctreeNode* m_parent;
	zpFixedArrayList< zpTransformOctreeNode*, zpTransformOctreeNodeSide_Count > m_children;

	zpFixedArrayList< zpTransformComponent*, 32 > m_objects;
};

class zpTransformOctree
{
public:
	void insert( zpTransformComponent* obj );
	void remove( zpTransformComponent* obj );

	void setup();
	void teardown();

	const zpBoundingAABB& getBounts() const;

	zp_int getObjectCount() const;

	zpTransformOctreeNode* createNode();
	void destroyNode( zpTransformOctreeNode* node );

	void update();

	template<typename Func>
	void foreachNode( Func func )
	{
		m_root->foreachNode( func );
	}

private:
	zpTransformOctreeNode* m_root;

	zpFixedArrayList< zpTransformOctreeNode, 128 > m_nodes;

	zpFixedArrayList< zpTransformOctreeNode*, 128 > m_usedNodes;
	zpFixedArrayList< zpTransformOctreeNode*, 128 > m_freeNodes;
};

class zpTransformComponentPool : public zpContentPool< zpTransformComponent, 64 >
{
public:
	zpTransformComponentPool();
	virtual ~zpTransformComponentPool();

	void update( zp_float deltaTime, zp_float realTime );

	zpTransformOctree* getTree() { return &m_octree; }

protected:
	void onCreate( zpTransformComponent* t );
	void onDestroy( zpTransformComponent* t );

private:
	zpTransformOctree m_octree;
};

#endif
