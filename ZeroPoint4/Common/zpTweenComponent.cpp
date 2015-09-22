#include "zpCommon.h"

zpTweenComponent::zpTweenComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	m_tweens.resize( 16 );

	zpTweenAction* b = m_tweens.begin();
	zpTweenAction* e = m_tweens.end();
	for( ; b != e; ++b )
	{
		m_freeTweens.pushBack( b );
	}
}
zpTweenComponent::~zpTweenComponent()
{

}

void zpTweenComponent::handleEvent( const zpEvent& e, zpObject* sender )
{

}


zp_bool zpTweenComponent::allComplete() const
{
	zp_bool allComplete = true;

	const zpTweenAction* const* b = m_usedTweens.begin();
	const zpTweenAction* const* e = m_usedTweens.end();
	for( ; b != e; ++b )
	{
		const zpTweenAction* a = *b;
		allComplete &= a->currentTime >= a->maxTime;
	}

	return allComplete;
}
zp_bool zpTweenComponent::isComplete( zp_int id ) const
{
	zp_bool complete = true;

	zp_uint index;
	zp_bool found = m_usedTweens.findIndexIf( [ &id ]( zpTweenAction* a ) {
		return a->id == id;
	}, index );

	if( found )
	{
		const zpTweenAction* a = m_usedTweens[ index ];
		complete = a->currentTime >= a->maxTime;
	}

	return complete;
}

void zpTweenComponent::cancelTween( zp_int id )
{
	zp_uint index;
	zp_bool found = m_usedTweens.findIndexIf( [ &id ]( zpTweenAction* a ) {
		return a->id == id;
	}, index );

	if( found )
	{
		zpTweenAction* a = m_usedTweens[ index ];
		a->currentTime = a->maxTime + 1.f;
	}
}
void zpTweenComponent::cancelAllTweens()
{
	zpTweenAction** b = m_usedTweens.begin();
	zpTweenAction** e = m_usedTweens.end();
	for( ; b != e; ++b )
	{
		zpTweenAction* a = *b;
		a->currentTime = a->maxTime + 1.f;
	}
}

zpTweenAction* zpTweenComponent::tweenToPosition( zpVector4fParamF endPosition, zp_float duration, zp_float delay, zp_bool useDT, zp_bool useWorld )
{
	zpTweenAction* a = m_freeTweens.back();
	m_freeTweens.popBack();
	m_usedTweens.pushBack( a );

	zpTweenPart start;
	zpTweenPart end;

	zpTransformComponent* t = getParentObject()->getComponents()->getTransformComponent();
	
	if( useWorld )
	{
		start.position = t->getWorldPosition();
		start.rotation = zpMath::Quaternion( 0, 0, 0, 1 );
		start.scale = zpMath::Vector4( 1, 1, 1, 0 );
	}
	else
	{
		start.position = t->getLocalPosition();
		start.rotation = t->getLocalRotation();
		start.scale = t->getLocalScale();
	}

	end.position = endPosition;
	end.rotation = start.rotation;
	end.scale = start.scale;

	setupTween( a, start, end, duration, delay, useDT );

	a->flags.setMarked( ZP_TWEEN_FLAGS_USE_WORLD, useWorld );

	return a;
}

void zpTweenComponent::onCreate()
{
	int i = 0; i++;
}
void zpTweenComponent::onInitialize()
{
	int i = 0; i++;
}
void zpTweenComponent::onDestroy()
{

}

void zpTweenComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	zpTweenAction** b = m_usedTweens.begin();
	zpTweenAction** e = m_usedTweens.end();
	for( ; b != e; ++b )
	{
		zpTweenAction* a = *b;
		if( !a->flags.isMarked( ZP_TWEEN_FLAGS_IS_ACTIVE ) ) continue;

		zp_float dt = a->flags.isMarked( ZP_TWEEN_FLAGS_USE_DT ) ? deltaTime : realTime;

		zp_bool wasOnDelay = a->currentTime <= 0.f;

		a->currentTime += dt;

		if( a->currentTime > a->maxTime )
		{
			if( a->flags.isMarked( ZP_TWEEN_FLAGS_REPEAT ) )
			{
				a->currentTime = a->currentTime - a->maxTime;
			}
			else
			{
				a->flags.mark( ZP_TWEEN_FLAGS_SHOULD_DESTROY );
				a->currentTime = -1.f;
			}
		}
		
		if( a->currentTime > 0.f )
		{
			// if was on delay and not past delay, trigger start
			if( wasOnDelay )
			{
				// TODO: send message on start
			}

			zp_float time = zp_min( a->currentTime, a->maxTime );
			zpScalar t = zpMath::Scalar( time / a->maxTime );

			zpVector4f sp = a->start.position;
			zpVector4f ep = a->end.position;

			// update the t based on the tween method
			switch( a->method )
			{
			case ZP_TWEEN_METHOD_LINEAR:
				break;

			case ZP_TWEEN_METHOD_SMOOTHSTEP:
				{
					zpScalar x;

					x = zpMath::ScalarSub( zpMath::Scalar( 3 ), zpMath::ScalarAdd( t, t ) );
					t = zpMath::ScalarMul( zpMath::ScalarMul( t, t ), x );
				}
				break;

			case ZP_TWEEN_METHOD_SMOOTHERSTEP:
				{
					zpScalar x;

					x = zpMath::ScalarSub( zpMath::ScalarMul( t, zpMath::Scalar( 6 ) ), zpMath::Scalar( 15 ) );
					x = zpMath::ScalarAdd( zpMath::ScalarMul( t, x ), zpMath::Scalar( 10 ) );
					t = zpMath::ScalarMul( t, zpMath::ScalarMul( t, zpMath::ScalarMul( t, x ) ) );
				}
				break;

			case ZP_TWEEN_METHOD_QUAD_IN:
				t = zpMath::ScalarMul( t, t );
				break;

			case ZP_TWEEN_METHOD_QUAD_OUT:
				t = zpMath::ScalarMul( t, zpMath::ScalarSub( t, zpMath::Scalar( 2 ) ) );
				t = zpMath::ScalarNeg( t );
				break;
			}

			// tween methods boil down to lerp
			a->current.position = zpMath::Vector4Lerp( sp, ep, t );
		}
	}

	// remove tweens that should be destroyed
	for( zp_int i = 0, imax = m_usedTweens.size(); i < imax; ++i )
	{
		zpTweenAction* a = m_usedTweens[ i ];
		zp_bool shouldDestroy = a->flags.isMarked( ZP_TWEEN_FLAGS_SHOULD_DESTROY );
		if( shouldDestroy )
		{
			// TODO: send message on finish

			m_usedTweens.erase( i );
			m_freeTweens.pushBack( a );
			--i;
			--imax;
		}
	}

	// update transform component based on current tweens
	zpTransformComponent* t = getParentObject()->getComponents()->getTransformComponent();

	b = m_usedTweens.begin();
	e = m_usedTweens.end();
	for( ; b != e; ++b )
	{
		zpTweenAction* a = *b;
		t->setLocalTransform( a->current.position, a->current.rotation, a->current.scale );
	}
}
void zpTweenComponent::onSimulate()
{

}

void zpTweenComponent::onEnabled()
{

}
void zpTweenComponent::onDisabled()
{

}

void zpTweenComponent::setupTween( zpTweenAction* a, const zpTweenPart& start, const zpTweenPart& end, zp_float duration, zp_float delay, zp_bool useDT )
{
	a->method = ZP_TWEEN_METHOD_LINEAR;
	a->currentTime = -delay;
	a->maxTime = duration;
	a->id = m_tweedId++;

	a->start = start;
	a->current = start;
	a->end = end;

	a->flags.clear();
	a->flags.mark( ZP_TWEEN_FLAGS_IS_ACTIVE );
	a->flags.setMarked( ZP_TWEEN_FLAGS_USE_DT, useDT );
}



zpTweenComponentPool::zpTweenComponentPool()
{

}
zpTweenComponentPool::~zpTweenComponentPool()
{

}

void zpTweenComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ deltaTime, realTime ]( zpTweenComponent* o ) {
		o->update( deltaTime, realTime );
	} );
}
