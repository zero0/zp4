#pragma once
#ifndef ZP_PARTICLE_EMITTER_COMPONENT_H
#define ZP_PARTICLE_EMITTER_COMPONENT_H

class zpParticleEmitterComponent : public zpComponent
{
public:
	zpParticleEmitterComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpParticleEmitterComponent();

	void render( zpRenderingContext* i, const zpCamera* camera );

	void setRenderLayer( zp_uint layer );
	zp_uint getRenderLayer() const;

	void setRenderingQueue( zpRenderingQueue queue );
	zpRenderingQueue getRenderingQueue() const;

	void play( const zp_char* effectName, zp_bool force = false );
	void stop( const zp_char* effectName );
	void stopAll();

	zp_bool isPlaying( const zp_char* effectName ) const;
	zp_bool isAnyPlaying() const;
	zp_bool isPaused() const;
	void pause( zp_bool isPaused );

protected:
	void onRender( zpRenderingContext* i, const zpCamera* camera );

	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	struct zpParticleEffect;
	
	zp_bool createEffect( const zp_char* name, const zpBison::Value& effectDef );

	void playEffect( zpParticleEffect* effect );

	void emitParticle( zpParticleEffect* effect );

	zp_bool m_isPaused;
	zp_uint m_layer;
	zpRenderingQueue m_queue;

	zp_uint m_maxParticles;

	zpRandom* m_random;

	zpVector4f m_prevPosition;

	struct zpParticle
	{
		zpVector4f position;
		zpVector4f velocity;
		zpVector4f rotation;
		zpVector4f angularVelocity;
		zpVector4f scale;
		zpVector4f normal;
		zpColor4f color;
		zp_float life;
		zp_float maxLife;
		zp_float maxSpeed;
	};

	enum zpParticleEffectShape
	{
		ZP_PARTICLE_EFFECT_SHAPE_SPHERE = 0,
		ZP_PARTICLE_EFFECT_SHAPE_HEMISPHERE,
		ZP_PARTICLE_EFFECT_SHAPE_CONE,
		ZP_PARTICLE_EFFECT_SHAPE_BOX,

		zpParticleEffectShape_Count
	};

	enum zpParticleEffectRange
	{
		ZP_PARTICLE_EFFECT_RANGE_CONSTANT = 0,
		ZP_PARTICLE_EFFECT_RANGE_RANDOM,
		ZP_PARTICLE_EFFECT_RANGE_LIFETIME,
		ZP_PARTICLE_EFFECT_RANGE_SPEED,

		zpParticleEffectRange_Count
	};

	enum zpParticleEffectState
	{
		ZP_PARTICLE_EFFECT_STATE_DISABLED = 0,
		ZP_PARTICLE_EFFECT_STATE_STARTING,
		ZP_PARTICLE_EFFECT_STATE_PLAYING,
		ZP_PARTICLE_EFFECT_STATE_FINISHING,

		zpParticleEffectState_Count
	};

	enum zpParticleEffectFlag
	{
		ZP_PARTICLE_EFFECT_FLAG_EMIT_FROM_SHELL = 0,
		ZP_PARTICLE_EFFECT_FLAG_RANDOM_DIRECTION,
		ZP_PARTICLE_EFFECT_FLAG_LOOPING,
		ZP_PARTICLE_EFFECT_FLAG_PRE_WARM,
		ZP_PARTICLE_EFFECT_FLAG_USE_REAL_TIME,
		ZP_PARTICLE_EFFECT_FLAG_WORLD_SPACE,
		ZP_PARTICLE_EFFECT_FLAG_IS_BILLBOARD,
		ZP_PARTICLE_EFFECT_FLAG_PLAY_ON_CREATE,
		ZP_PARTICLE_EFFECT_FLAG_USE_DEPTH_SORT,

		zpParticleEffectFlag_Count
	};

	struct zpParticleEffect
	{
		zpString name;
		zpVector4f gravity;
		zpVector4f shapeSize;

		zpVector4f startScale;
		zpVector4f endScale;

		zpColor4f startColor;
		zpColor4f endColor;

		zp_float delay;
		zp_float duration;
		zp_float time;

		zp_float minSpeed;
		zp_float maxSpeed;

		zp_float inheritVelocityScale;

		zp_float minLife;
		zp_float maxLife;

		zp_float emitTime;
		zp_float minEmitRate;
		zp_float maxEmitRate;

		zp_float flipbookSpeed;
		zp_uint flipbookX;
		zp_uint flipbookY;

		zpParticleEffectRange scaleRange;
		zpParticleEffectRange colorRange;

		zpParticleEffectShape shape;
		zp_bool emitFromShell;
		zp_bool randomDirection;
		zp_bool isLooping;
		zp_bool preWarm;
		zp_bool useRealTime;
		zp_bool isWorldSpace;
		zp_bool isBillboard;
		zp_bool playOnCreate;
		zp_bool useDepthSort;

		zpParticleEffectState state;

		zpMaterialResourceInstance material;
		zpArrayList< zpParticle* > usedParticles;

		zp_ushort sortBias;
	};

	zpArrayList< zpParticle > m_allParticles;
	zpArrayList< zpParticleEffect > m_effects;

	zpArrayList< zpParticle* > m_freeParticles;
};


class zpParticleEmitterComponentPool : public zpContentPool< zpParticleEmitterComponent, 64 >
{
public:
	zpParticleEmitterComponentPool();
	virtual ~zpParticleEmitterComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
	void simulate();

	void render( zpRenderingContext* i, const zpCamera* camera );
};

#endif
