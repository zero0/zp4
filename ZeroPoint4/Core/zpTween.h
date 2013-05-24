#pragma once
#ifndef ZP_TWEEN_H
#define ZP_TWEEN_H

enum zpTweenEasing
{
	ZP_TWEEN_EASING_LINEAR,

	ZP_TWEEN_EASING_QUADRATIC_IN,
	ZP_TWEEN_EASING_QUADRATIC_OUT,
	ZP_TWEEN_EASING_QUADRATIC_IN_OUT,

	ZP_TWEEN_EASING_CUBIC_IN,
	ZP_TWEEN_EASING_CUBIC_OUT,
	ZP_TWEEN_EASING_CUBIC_IN_OUT,

	ZP_TWEEN_EASING_QUARTIC_IN,
	ZP_TWEEN_EASING_QUARTIC_OUT,
	ZP_TWEEN_EASING_QUARTIC_IN_OUT,

	ZP_TWEEN_EASING_QUINTIC_IN,
	ZP_TWEEN_EASING_QUINTIC_OUT,
	ZP_TWEEN_EASING_QUINTIC_IN_OUT,

	ZP_TWEEN_EASING_SIN_IN,
	ZP_TWEEN_EASING_SIN_OUT,
	ZP_TWEEN_EASING_SIN_IN_OUT,

	ZP_TWEEN_EASING_EXP_IN,
	ZP_TWEEN_EASING_EXP_OUT,
	ZP_TWEEN_EASING_EXP_IN_OUT,

	ZP_TWEEN_EASING_CIRCULAR_IN,
	ZP_TWEEN_EASING_CIRCULAR_OUT,
	ZP_TWEEN_EASING_CIRCULAR_IN_OUT,

	zpTweenEasing_Count,
	zpTweenEasing_Force32Bit = ZP_FORECE_32BIT,
};

template<typename T>
class zpTween
{
public:
	zpTween( const T& start = T() );
	~zpTween();

	void update( zp_float deltaTime );

	void queue( zp_float time, const T& val, zpTweenEasing func = ZP_TWEEN_EASING_LINEAR );
	void clear();

	const T& getCurrentValue() const;
	zp_bool isComplete() const;

	void setStartValue( const T& start );

private:
	struct zpTweenStep
	{
		zpTweenEasing easing;
		zp_float time;
		T value;
	};

	struct zpTweenFunctions {
		template<int>
		static T easing( const T& b, const T& c, zp_float t, zp_float d );

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_LINEAR>( const T& b, const T& c, zp_float t, zp_float d ) {
			return c * ( t / d ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUADRATIC_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			return c * t * t + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUADRATIC_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			return -c * t * ( t - 2 ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUADRATIC_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d * 0.5f;
			if( t < 1.f ) return c * 0.5f * t * t + b;
			t--;
			return -c * 0.5f * ( t * ( t - 2 ) - 1 ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_CUBIC_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			return c * t * t * t + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_CUBIC_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			t--;
			return c * ( t * t * t + 1 ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_CUBIC_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d * 0.5f;
			if( t < 1.f ) return c * 0.5f * t * t * t + b;
			t -= 2.f;
			return c * 0.5f * ( t * t * t + 2 ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUARTIC_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			return c * t * t * t * t + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUARTIC_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			t--;
			return -c * ( t * t * t * t - 1.f ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUARTIC_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d * 0.5f;
			if( t < 1.f ) return c * 0.5f * t * t * t * t + b;
			t -= 2.f;
			return c * 0.5f * ( t * t * t * t - 2 ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUINTIC_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			return c * t * t * t * t * t + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUINTIC_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			t--;
			return -c * ( t * t * t * t * t - 1.f ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_QUINTIC_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d * 0.5f;
			if( t < 1.f ) return c * 0.5f * t * t * t * t * t + b;
			t -= 2.f;
			return c * 0.5f * ( t * t * t * t * t + 2 ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_SIN_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			return -c * zp_cos( ( t / d ) * ZP_PIOVER2 ) + c + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_SIN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			return c * zp_sin( ( t / d ) * ZP_PIOVER2 ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_SIN_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			return -c * 0.5f * ( zp_cos( ZP_PI * t / d ) - 1 ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_EXP_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			return c * zp_pow( 2, 10 * ( t / d - 1 ) ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_EXP_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			return c * ( -zp_pow( 2, -10 * t / d ) + 1 ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_EXP_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d * 0.5f;
			if( t < 1.f ) return c * 0.5f * zp_pow( 2, 10 * ( t - 1 ) ) + b;
			t--;
			return c * 0.5f * ( zp_pow( 2, -10 * t ) + 2 ) + b;
		}

		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_CIRCULAR_IN>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			return -c * ( zp_sqrt( 1 - t * t ) - 1 ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_CIRCULAR_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d;
			t--;
			return c * zp_sqrt( 1 - t * t ) + b;
		}
		template<>
		ZP_FORCE_INLINE static T easing<ZP_TWEEN_EASING_CIRCULAR_IN_OUT>( const T& b, const T& c, zp_float t, zp_float d ) {
			t /= d * 0.5f;
			if( t < 1.f ) return -c * 0.5f * ( zp_sqrt( 1 - t * t ) - 1 ) + b;
			t -= 2.f;
			return c * 0.5f * ( zp_sqrt( 1 - t * t ) + 1 ) + b;
		}
	};

	zp_float m_currentTime;
	zpArrayList<zpTweenStep> m_tweenSteps;

	T m_startValue;
	T m_currentValue;
};

#include "zpTween.inl"

#endif