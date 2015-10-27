#pragma once
#ifndef ZP_ANIMATION_ENGINE_H
#define ZP_ANIMATION_ENGINE_H

class zpAnimationEngine;

enum zpAnimationControllerPlayMode
{
	ZP_ANIMATION_CONTROLLER_PLAY_MODE_NORMAL,
	ZP_ANIMATION_CONTROLLER_PLAY_MODE_REPEAT,
	ZP_ANIMATION_CONTROLLER_PLAY_MODE_REPEAT_PING_PONG,
};

class zpAnimationController
{
public:
	zpAnimationController();
	~zpAnimationController();

	void setAnimation( const zpAnimationResourceInstance& anim );

	void play( const zpString& animationName );
	void crossFade( const zpString& animationName, zp_float crossFadeTime );

	void update( zp_float dt );

private:
	zp_int m_currentKeyFrame;
	zp_int m_nextKeyFrame;
	zp_float m_keyFrameDelta;

	zp_int m_crossFadeCurrentKeyFrame;
	zp_int m_crossFadeNextKeyFrame;
	zp_float m_crossFadeKeyFrameDelta;

	zp_float m_crossFadeTime;

	zp_float m_animationSpeed;
	zp_int m_playDirection;

	zp_size_t m_currentAnimationClip;
	zp_size_t m_nextAnimationClip;

	zpAnimationControllerPlayMode m_playMode;

	zpAnimationResourceInstance m_animation;

	friend class zpAnimationEngine;
};

struct zpAnimationEngineProcess
{
	zpMesh* mesh;
	zpSkeleton* skeleton;
	zpAnimationController* controller;
};

class zpAnimationEngine
{
public:
	zpAnimationEngine();
	~zpAnimationEngine();

	void queueAnimation( zpMesh* mesh, zpSkeleton* skeleton, zpAnimationController* animController );

	void processAnimations( zp_float dt );

	void flushAnimationQueue();

private:
	zp_uint m_maxBones;

	zpArrayList< zpAnimationEngineProcess > m_animationQueue;
};

#endif
