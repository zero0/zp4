#pragma once
#ifndef ZP_AUDIO_INSTANCE_H
#define ZP_AUDIO_INSTANCE_H

ZP_RESOURCE_INSTANCE_TEMPLATE_START_COPY( zpAudioResource, copy )
public:
	zp_bool isPlaying() const;

	void setPosition( const zpVector4f& position );
	const zpVector4f& getPosition() const;

	void setVelocity( const zpVector4f& velocity );
	const zpVector4f& getVelocity() const;

	void setConeOrientation( const zpVector4f& orientation );
	const zpVector4f& getConeOrientation() const;

	void setVolume( zp_float volume );
	zp_float getVolume() const;

	void setMinMaxDistance( zp_float minDistance, zp_float maxDistance );
	zp_float getMinDistance() const;
	zp_float getMaxDistance() const;

	void update();
	void stop();

	void setChannel( void* channel );

private:
	void copy( const zpResourceInstance<zpAudioResource>& c );

	void* m_channel;

	zp_float m_minDistance;
	zp_float m_maxDistance;

	zpVector4f m_position;
	zpVector4f m_velocity;
	zpVector4f m_coneOrientation;

ZP_RESOURCE_INSTANCE_TEMPLATE_END

/*
template<>
class zpResourceInstance<zpAudioResource> {
public:
	zpResourceInstance();
	zpResourceInstance( zpAudioResource* resource );
	~zpResourceInstance();

	void operator=( const zpResourceInstance<zpAudioResource>& instance );
	void operator=( zpResourceInstance<zpAudioResource>&& instance );

	operator zp_bool() const;

	zpAudioResource* getAudioResource() const;

	zp_bool isPlaying() const;

	void setPosition( const zpVector4f& position );
	const zpVector4f& getPosition() const;

	void setVelocity( const zpVector4f& velocity );
	const zpVector4f& getVelocity() const;

	void setConeOrientation( const zpVector4f& orientation );
	const zpVector4f& getConeOrientation() const;

	void setVolume( zp_float volume );
	zp_float getVolume() const;

	void setMinMaxDistance( zp_float minDistance, zp_float maxDistance );
	zp_float getMinDistance() const;
	zp_float getMaxDistance() const;

	void update();
	void stop();

	void setChannel( void* channel );

private:
	zpAudioResource* m_resource;
	void* m_channel;

	zp_float m_minDistance;
	zp_float m_maxDistance;

	zpVector4f m_position;
	zpVector4f m_velocity;
	zpVector4f m_coneOrientation;
};
*/
#endif