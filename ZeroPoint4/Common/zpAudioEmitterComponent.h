#pragma once
#ifndef ZP_AUDIO_EMITTER_COMPONENT_H
#define ZP_AUDIO_EMITTER_COMPONENT_H

class zpAudioEmitterComponent : public zpComponent
{
public:
	zpAudioEmitterComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpAudioEmitterComponent();

	void playSound( const zpString& filename );
	void stopSound( const zpString& filename );
	void stopAllSounds();

	void clearSoundCache();

	zp_bool isMovingSound() const;

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_bool m_isMoving;
	zpVector4f m_prevPosition;
	zpHashMap< zpString, zpAudioResourceInstance > m_sounds;
};


class zpAudioEmitterComponentPool : public zpContentPool< zpAudioEmitterComponent, 16 >
{
public:
	zpAudioEmitterComponentPool();
	virtual ~zpAudioEmitterComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
	void simulate();
};

#endif
