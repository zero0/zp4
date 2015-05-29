#pragma once
#ifndef ZP_SKELETON_H
#define ZP_SKELETON_H

class zpSkeletonContentManager;

struct zpSkeletonBone
{
	zp_int indexStart;
	zp_int indexCount;
	zp_int weightStart;
	zp_int weightCount;
	zp_int parent;
	zpMatrix4f bindPose;
};

struct zpSkeleton
{
	zpArrayList< zp_int > indecies;
	zpArrayList< zp_float > weights;
	zpArrayList< zpString > boneNames;
	zpArrayList< zpSkeletonBone > bones;
};

class zpSkeletonResource : public zpResource< zpSkeleton >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpMeshContentManager;
};

class zpSkeletonResourceInstance : public zpResourceInstance< zpSkeletonResource >
{
};

class zpSkeletonContentManager : public zpContentManager< zpSkeletonResource, zpSkeletonResourceInstance, zpSkeletonContentManager, 8 >
{
private:
	zp_bool createResource( zpSkeletonResource* res, const zp_char* filename );
	void destroyResource( zpSkeletonResource* res );
	void initializeInstance( zpSkeletonResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
