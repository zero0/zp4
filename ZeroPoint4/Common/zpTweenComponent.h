#pragma once
#ifndef ZP_TWEEN_COMPONENT_H
#define ZP_TWEEN_COMPONENT_H

enum zpTweenMethod : zp_int
{
	ZP_TWEEN_METHOD_LINEAR = 0,

	ZP_TWEEN_METHOD_SMOOTHSTEP,
	ZP_TWEEN_METHOD_SMOOTHERSTEP,

	ZP_TWEEN_METHOD_QUAD_IN,
	ZP_TWEEN_METHOD_QUAD_OUT,
	ZP_TWEEN_METHOD_QUAD_IN_OUT,

	ZP_TWEEN_METHOD_CUBIC_IN,
	ZP_TWEEN_METHOD_CUBIC_OUT,
	ZP_TWEEN_METHOD_CUBIC_IN_OUT,

	ZP_TWEEN_METHOD_QUAR_IN,
	ZP_TWEEN_METHOD_QUAR_OUT,
	ZP_TWEEN_METHOD_QUAR_IN_OUT,

	ZP_TWEEN_METHOD_QUIN_IN,
	ZP_TWEEN_METHOD_QUIN_OUT,
	ZP_TWEEN_METHOD_QUIN_IN_OUT,

	ZP_TWEEN_METHOD_SIN_IN,
	ZP_TWEEN_METHOD_SIN_OUT,
	ZP_TWEEN_METHOD_SIN_IN_OUT,

	ZP_TWEEN_METHOD_EXP_IN,
	ZP_TWEEN_METHOD_EXP_OUT,
	ZP_TWEEN_METHOD_EXP_IN_OUT,

	ZP_TWEEN_METHOD_CIRCULAR_IN,
	ZP_TWEEN_METHOD_CIRCULAR_OUT,
	ZP_TWEEN_METHOD_CIRCULAR_IN_OUT,

	zpTweenMethod_Count
};

enum zpTweenFlags : zp_int
{
	ZP_TWEEN_FLAGS_IS_ACTIVE = 0,
	ZP_TWEEN_FLAGS_USE_DT,
	ZP_TWEEN_FLAGS_USE_WORLD,
	ZP_TWEEN_FLAGS_REPEAT,
	ZP_TWEEN_FLAGS_PING_PONG,
	ZP_TWEEN_FLAGS_SUPPRESS_FINISH_CALLBACK,
	ZP_TWEEN_FLAGS_FLIP_DIRECTION,

	ZP_TWEEN_FLAGS_SHOULD_DESTROY,
};

struct zpTweenPart
{
	zpVector4f position;
	zpQuaternion4f rotation;
	zpVector4f scale;
};

struct zpTweenAction
{
	zpTweenPart start;
	zpTweenPart current;
	zpTweenPart end;

	zpTweenMethod method;
	zp_float currentTime;
	zp_float maxTime;
	zp_int id;
	zpFlag32 flags;

	zpString messageOnStart;
	zpString messageOnFinish;

	zpString startOnEvent;
};

class zpTweenComponent : public zpComponent, zpEventListener
{
public:
	zpTweenComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpTweenComponent();

	void handleEvent( const zpEvent& e, zpObject* sender );

	zp_bool allComplete() const;
	zp_bool isComplete( zp_int id ) const;

	void cancelTween( zp_int id );
	void cancelAllTweens();

	zpTweenAction* tweenToPosition( zpVector4fParamF endPostion, zp_float duration, zp_float delay, zp_bool useDT, zp_bool useWorld );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	void setupTween( zpTweenAction* a, const zpTweenPart& start, const zpTweenPart& end, zp_float duration, zp_float delay, zp_bool useDT );
	
	zp_int m_tweedId;

	zpFixedArrayList< zpTweenAction*, 16 > m_freeTweens;
	zpFixedArrayList< zpTweenAction*, 16 > m_usedTweens;

	zpFixedArrayList< zpTweenAction, 16 > m_tweens;
};

class zpTweenComponentPool : public zpContentPool< zpTweenComponent, 16 >
{
public:
	zpTweenComponentPool();
	virtual ~zpTweenComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
