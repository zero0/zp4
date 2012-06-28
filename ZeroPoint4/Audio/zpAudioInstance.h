#pragma once
#ifndef ZP_AUDIO_INSTANCE_H
#define ZP_AUDIO_INSTANCE_H

class zpAudioInstance {
public:
	zpAudioInstance( zpAudioResource* resource );
	~zpAudioInstance();

	zpAudioResource* getAudioResource() const;

	zp_bool isPlaying() const;

	void setPosition( const zpVector4& position );
	const zpVector4& getPosition() const;

	void setVelocity( const zpVector4& velocity );
	const zpVector4& getVelocity() const;

	void setConeOrientation( const zpVector4& orientation );
	const zpVector4& getConeOrientation() const;

	void setVolume( zp_float volume );
	zp_float getVolume() const;

	void setMinMaxDistance( zp_float minDistance, zp_float maxDistance );
	zp_float getMinDistance() const;
	zp_float getMaxDistance() const;
	
	void update();
	void stop();

	void setChannel( void* channel );

private:
	zpAudioInstance();

	zpAudioResource* m_resource;
	void* m_channel;

	zp_float m_minDistance;
	zp_float m_maxDistance;

	zpVector4 m_position;
	zpVector4 m_velocity;
	zpVector4 m_coneOrientation;
};

#endif