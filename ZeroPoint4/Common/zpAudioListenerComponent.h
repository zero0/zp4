#pragma once
#ifndef ZP_AUDIO_LISTENER_COMPONENT_H
#define ZP_AUDIO_LISTENER_COMPONENT_H

class zpAudioListenerComponent : public zpComponent
{
public:
	zpAudioListenerComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpAudioListenerComponent();

	void setRolloffFactor( zp_float rolloff );
	void setDistanceFactor( zp_float dist );
	void setDopplerFactor( zp_float doppler );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy(); 

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zpAudioEngine* m_engine;
	zpVector4f m_oldPosition;
};


class zpAudioListenerComponentPool : public zpContentPool< zpAudioListenerComponent, 1 >
{
public:
	zpAudioListenerComponentPool();
	virtual ~zpAudioListenerComponentPool();

	void update();
	void simulate();
};

#endif