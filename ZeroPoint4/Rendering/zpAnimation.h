#pragma once
#ifndef ZP_ANIMATION_H
#define ZP_ANIMATION_H

struct zpAnimationClip
{
	zpString animationName;
	zp_float frameRate;

	zpArrayList< zpString > boneNames;
	zpArrayList< zpArrayList< zpMatrix4f > > keyFrames;
};

struct zpAnimation
{
	zpArrayList< zpAnimationClip > clips;
};

class zpAnimationResource : public zpResource< zpAnimation >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpAnimationContentManager;
};

class zpAnimationResourceInstance : public zpResourceInstance< zpAnimationResource >
{
};

class zpAnimationContentManager : public zpContentManager< zpAnimationResource, zpAnimationResourceInstance, zpAnimationContentManager, 8 >
{
private:
	zp_bool createResource( zpAnimationResource* res, const zp_char* filename );
	void destroyResource( zpAnimationResource* res );
	void initializeInstance( zpAnimationResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
