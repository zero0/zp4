#pragma once
#ifndef ZP_OCTREE_H
#define ZP_OCTREE_H

enum zpOctreeNodeType
{
	ZP_OCTREE_NODE_TYPE_LINK,
	ZP_OCTREE_NODE_TYPE_LEAF,
};

class zpOctreeNode
{
public:
	zpOctreeNode();
	zpOctreeNode( const zpBoundingAABB& bounds );
	~zpOctreeNode();

	zp_uint update( zpArrayList< zpObject* >& updatedObjects );

	zp_uint getNumObjects() const;

	zp_bool insert( zpObject* obj );

	template<typename Func>
	void query( const zpBoundingAABB& bounds, Func queryFunc ) const;

	template<typename Func>
	void foreach( Func foreachFunc ) const;

	const zpBoundingAABB& getBounds() const;

private:
	enum zpOctreeNodeSide
	{
		TNW,
		TNE,
		TSW,
		TSE,

		BNW,
		BNE,
		BSW,
		BSE,

		zpOctreeNodeSide_Count,
	};

	zp_bool add( zpObject* obj );
	void subdivide();

	zpOctreeNodeType m_type;

	zp_int m_id;
	zp_uint m_maxObjectCount;
	zpBoundingAABB m_bounds;

	zpArrayList< zpObject* > m_objects;
	zpFixedArrayList< zpOctreeNode*, zpOctreeNodeSide_Count > m_children;
};

class zpOctree
{
public:
	zpOctree();
	zpOctree( const zpBoundingAABB& bounds );
	~zpOctree();

	void update();

	zp_uint getNumObjects() const;

	void insert( zpObject* obj );

	template<typename Func>
	void query( const zpBoundingAABB& bounds, Func queryFunc ) const;

	template<typename Func>
	void foreach( Func foreachFunc ) const;

private:
	zpOctreeNode m_root;
};

#include "zpOctree.inl"

#endif
