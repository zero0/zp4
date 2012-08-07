#pragma once
#ifndef ZP_TWEEN_H
#define ZP_TWEEN_H

enum zpTweenEasing {
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
};

template<typename T>
struct zpTweenStep {
	zpTweenEasing easing;
	zp_float time;
	T value;
};

template<typename T>
class zpTween {
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
	zp_float m_currentTime;
	zpArrayList<zpTweenStep> m_tweenSteps;

	T m_startValue;
	T m_currentValue;
};

#endif