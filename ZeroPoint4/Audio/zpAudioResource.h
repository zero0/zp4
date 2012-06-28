#pragma once
#ifndef ZP_AUDIO_RESOURCE_H
#define ZP_AUDIO_RESOURCE_H

#define ZP_AUDIO_LOOP_POINT_NONE	(zp_uint)-1

class zpAudioResource : public zpResource {
public:
	zpAudioResource();
	virtual ~zpAudioResource();

	zp_bool load();
	void unload();

	zp_bool isStreaming() const;
	zp_bool isLooping() const;
	zp_bool is3DSound() const;
	zpAudioChannelGroup getChannelGroup() const;
	zp_uint getLoopFrom() const;
	zp_uint getLoopTo() const;

private:
	void* getSound() const;

	void setIsStreaming( zp_bool isStreaming );
	void setIsLooping( zp_bool isLooping );
	void setIs3DSound( zp_bool is3DSound );
	void setChannelGroup( zpAudioChannelGroup group );
	void setLoopFromTo( zp_uint loopFrom, zp_uint loopTo );

	zp_bool m_isStreaming;
	zp_bool m_isLooping;
	zp_bool m_is3DSound;

	void* m_sound;
	zpAudioChannelGroup m_group;

	zp_uint m_loopFrom;
	zp_uint m_loopTo;

	friend class zpAudioResourceCreator;
	friend class zpAudioManager;
};

#endif