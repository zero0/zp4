#include "zpRendering.h"

zpAnimationController::zpAnimationController()
	: m_useRealTime( true )
	, m_currentKeyFrame( 0 )
	, m_nextKeyFrame( 1 )
	, m_keyFrameDelta( 0.f )
	, m_crossFadeTime( 0.f )
	, m_animationSpeed( 1.f )
	, m_playDirection( 1 )
	, m_currentAnimationClip( -1 )
	, m_nextAnimationClip( -1 )
	, m_playMode( ZP_ANIMATION_CONTROLLER_PLAY_MODE_NORMAL )
{

}
zpAnimationController::~zpAnimationController()
{

}

void zpAnimationController::setAnimation( const zpAnimationResourceInstance& anim )
{
	m_animation = anim;
}

void zpAnimationController::play( const zpString& animationName )
{
	if( m_animation.isVaild() )
	{
		zp_size_t index;
		zp_bool found = m_animation.getResource()->getData()->clips.findIndexIf( [ &animationName ]( const zpAnimationClip& c ) {
			return c.animationName == animationName;
		}, index );

		if( found )
		{
			m_currentAnimationClip = index;
			m_crossFadeTime = 0.f;

			m_currentKeyFrame = 0;
			m_nextKeyFrame = 1;
		}
	}
}
void zpAnimationController::crossFade( const zpString& animationName, zp_float crossFadeTime )
{
	if( m_animation.isVaild() )
	{
		zp_size_t index;
		zp_bool found = m_animation.getResource()->getData()->clips.findIndexIf( [ &animationName ]( const zpAnimationClip& c ) {
			return c.animationName == animationName;
		}, index );

		if( found && m_nextAnimationClip != m_currentAnimationClip )
		{
			m_nextAnimationClip = index;
			m_crossFadeTime = crossFadeTime;

			m_crossFadeCurrentKeyFrame = 0;
			m_crossFadeNextKeyFrame = 1;
		}
	}
}

void zpAnimationController::update( zp_float dt, zp_float rt )
{
	const zpAnimationClip& currentClip = m_animation.getResource()->getData()->clips[ m_currentAnimationClip ];
	
	zp_float tt = m_useRealTime ? rt : dt;
	zp_float t = currentClip.frameRate * m_animationSpeed * tt;

	m_keyFrameDelta += t;

	if( m_keyFrameDelta > 1.f )
	{
		m_currentKeyFrame = m_nextKeyFrame;

		switch( m_playMode )
		{
		case ZP_ANIMATION_CONTROLLER_PLAY_MODE_NORMAL:
			if( ( m_currentKeyFrame + m_playDirection ) < 0 )
			{
				m_nextKeyFrame = 0;
			}
			else if( ( m_currentKeyFrame + m_playDirection ) > currentClip.maxFrames )
			{
				m_nextKeyFrame = currentClip.maxFrames - 1;
			}
			break;

		case ZP_ANIMATION_CONTROLLER_PLAY_MODE_REPEAT:
			if( ( m_currentKeyFrame + m_playDirection ) < 0 )
			{
				m_nextKeyFrame = currentClip.maxFrames - 1;
			}
			else if( ( m_currentKeyFrame + m_playDirection ) > currentClip.maxFrames )
			{
				m_nextKeyFrame = 0;
			}
			break;

		case ZP_ANIMATION_CONTROLLER_PLAY_MODE_REPEAT_PING_PONG:
			if( ( m_currentKeyFrame + m_playDirection ) < 0 )
			{
				m_nextKeyFrame = 0;
				m_playDirection = 1;
			}
			else if( ( m_currentKeyFrame + m_playDirection ) > currentClip.maxFrames )
			{
				m_nextKeyFrame = currentClip.maxFrames - 1;
				m_playDirection = -1;
			}
			break;
		}

		m_keyFrameDelta -= 1.f;
	}

	if( m_nextAnimationClip >= 0 )
	{
		const zpAnimationClip& nextClip = m_animation.getResource()->getData()->clips[ m_nextAnimationClip ];

		m_crossFadeKeyFrameDelta += t;

		if( m_crossFadeKeyFrameDelta > 1.f )
		{
			m_crossFadeCurrentKeyFrame = m_crossFadeNextKeyFrame;
			m_crossFadeNextKeyFrame = ( m_crossFadeNextKeyFrame + 1 ) % nextClip.maxFrames;
		}

		m_crossFadeTime -= tt;

		if( m_crossFadeTime < 0.f )
		{
			m_nextAnimationClip = -1;

			m_currentKeyFrame = m_crossFadeCurrentKeyFrame;
			m_nextKeyFrame = m_crossFadeNextKeyFrame;
			m_keyFrameDelta = m_crossFadeKeyFrameDelta;
		}
	}
}



zpAnimationEngine::zpAnimationEngine()
	: m_maxBones( 4 )
{

}
zpAnimationEngine::~zpAnimationEngine()
{

}

void zpAnimationEngine::queueAnimation( zpMesh* mesh, zpSkeleton* skeleton, zpAnimationController* animController )
{
	zpAnimationEngineProcess& proc = m_animationQueue.pushBackEmpty();
	proc.mesh = mesh;
	proc.skeleton = skeleton;
	proc.controller = animController;
}

void zpAnimationEngine::processAnimations( zp_float dt, zp_float rt )
{
	zpAnimationEngineProcess* animB = m_animationQueue.begin();
	zpAnimationEngineProcess* animE = m_animationQueue.end();
	for( ; animB != animE; ++animB )
	{
		animB->controller->update( dt, rt );

		for( zp_size_t i = 0, imax = animB->skeleton->bones.size(); i < imax; ++i )
		{
			zpSkeletonBone* bone = &animB->skeleton->bones[ i ];

			zpMatrix4f bindPose = bone->bindPose;
			const zpString& boneName = animB->skeleton->boneNames[ i ];

			const zpAnimationClip& currentClip = animB->controller->m_animation.getResource()->getData()->clips[ animB->controller->m_currentAnimationClip ];

			const zpArrayList< zpMatrix4f >& frames = currentClip.keyFrames[ i ];
		}
	}
}

void zpAnimationEngine::flushAnimationQueue()
{
	m_animationQueue.reset();
}
