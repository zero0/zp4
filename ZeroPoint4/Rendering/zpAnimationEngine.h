#pragma once
#ifndef ZP_ANIMATION_ENGINE_H
#define ZP_ANIMATION_ENGINE_H

class zpAnimationController
{
public:
	zpAnimationController();
	~zpAnimationController();

private:
	zp_int m_currentAnimation;
	zp_int m_nextAnimation;
};

class zpAnimationEngine
{
public:
	void queueAnimation( zpMesh* mesh, zpSkeleton* skeleton, zpAnimationController* animController );

	void processAnimations( zp_float dt, zp_float rt );

private:

};

#endif
