#pragma once
#ifndef ZP_AUDIO_RESOURCE_H
#define ZP_AUDIO_RESOURCE_H

class zpAudioResource;
class zpAudioResourceInstance;
class zpAudioContentManager;

class zpAudioResource : public zpResource< zpAudioBuffer >
{
public:

private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpAudioContentManager;
};

class zpAudioResourceInstance : public zpResourceInstance< zpAudioResource >
{
public:
	zpAudioResourceInstance();
	virtual ~zpAudioResourceInstance();

	void setPosition( const zpVector4f& pos );
	void setVelocity( const zpVector4f& vel );
	void setVolume( zp_float volume );
	void setPan( zp_float pan );
	void setFrequency( zp_float frequency );

	void play( zp_bool repeat = false );
	void stop();

	zp_bool isPlaying() const;
	zp_bool is3DSound() const;

protected:
	void initialized();

private:
	zpAudioBuffer m_instanceAudioBuffer;

	zpAudioEngine* m_engine;

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

class zpAudioContentManager : public zpContentManager< zpAudioResource, zpAudioResourceInstance, zpAudioContentManager, 64 >
{
public:
	zpAudioContentManager();
	virtual ~zpAudioContentManager();

private:
	zp_bool createResource( zpAudioResource* res, const zp_char* filename );
	void destroyResource( zpAudioResource* res );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};


#endif
