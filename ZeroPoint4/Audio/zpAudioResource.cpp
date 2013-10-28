#include "zpAudio.h"

#define CHECK_WAVE_ID( id, a, b, c, d )		( (id)[0] == (a) && (id)[1] == (b) && (id)[2] == (c) && (id)[3] == (d) )

struct zpWaveHeader
{
	zp_char chunkId[4];
	zp_uint chunkSize;
	zp_char format[4];
	zp_char subChunkId[4];
	zp_uint subChunkSize;
	zp_ushort audioFormat;
	zp_ushort numChannels;
	zp_uint sampleRate;
	zp_uint bytesPerSecond;
	zp_ushort blockAlign;
	zp_ushort bitsPerSample;
	zp_char dataChunkId[4];
	zp_uint dataSize;
};

zp_bool decodeWave( const zpString& filename, zpWaveHeader& header, zpDataBuffer& data )
{
	zpFile waveFile( filename );
	zpDataBuffer waveFileData;

	if( waveFile.open( ZP_FILE_MODE_BINARY_READ ) )
	{
		waveFile.readFileBinary( waveFileData );
		waveFile.close();

		waveFileData.readAt( header, 0 );

		if( !CHECK_WAVE_ID( header.chunkId, 'R', 'I', 'F', 'F' ) )
		{
			return false;
		}

		if( !CHECK_WAVE_ID( header.format, 'W', 'A', 'V', 'E' ) )
		{
			return false;
		}

		if( !CHECK_WAVE_ID( header.subChunkId, 'f', 'm', 't', ' ' ) )
		{
			return false;
		}

		if( !CHECK_WAVE_ID( header.dataChunkId, 'd', 'a', 't', 'a' ) )
		{
			return false;
		}

		data.reserve( (zp_uint)header.dataSize );
		data.writeBulk( waveFileData.getData() + sizeof( zpWaveHeader ), (zp_uint)header.dataSize );

		return true;
	}

	return false;
}

zp_bool zpAudioResource::load( const zp_char* filename )
{
	m_filename = filename;

	zpAudioEngine* engine = zpAudioEngine::getInstance();

	zp_bool ok = false;
	zpWaveHeader header;
	zpDataBuffer buffer;
	if( decodeWave( m_filename, header, buffer ) )
	{
		ok = true;
		engine->createSoundBuffer( m_resource, ZP_AUDIO_TYPE_2D, (zp_uint)header.dataSize, (zp_uint)header.sampleRate, header.bitsPerSample, header.numChannels );
		engine->fillSoundBuffer( m_resource, buffer.getData(), buffer.size() );
	}

	return ok;
}
void zpAudioResource::unload()
{
	zpAudioEngine::getInstance()->destroySoundBuffer( m_resource );
}


zpAudioResourceInstance::zpAudioResourceInstance()
	: m_engine( zpAudioEngine::getInstance() )
{}
zpAudioResourceInstance::~zpAudioResourceInstance()
{
	stop();
	m_engine = ZP_NULL;
}
void zpAudioResourceInstance::setPosition( const zpVector4f& pos )
{
	m_engine->setPosition( m_instanceAudioBuffer, pos );
}
void zpAudioResourceInstance::setVolume( zp_float volume )
{
	m_engine->setVolume( m_instanceAudioBuffer, volume );
}
void zpAudioResourceInstance::setPan( zp_float pan )
{
	m_engine->setPan( m_instanceAudioBuffer, pan );
}
void zpAudioResourceInstance::setFrequency( zp_float frequency )
{
}

void zpAudioResourceInstance::play( zp_bool repeat )
{
	m_engine->play( m_instanceAudioBuffer, repeat );
}
void zpAudioResourceInstance::stop()
{
	m_engine->stop( m_instanceAudioBuffer );
}

zp_bool zpAudioResourceInstance::isPlaying() const
{
	return m_engine->isPlaying( m_instanceAudioBuffer );
}

void zpAudioResourceInstance::initialized()
{
	m_engine->cloneSoundBuffer( *getResource()->getData(), m_instanceAudioBuffer );
}


zpAudioContentManager::zpAudioContentManager()
{}
zpAudioContentManager::~zpAudioContentManager()
{}
zp_bool zpAudioContentManager::createResource( zpAudioResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpAudioContentManager::destroyResource( zpAudioResource* res )
{
	res->unload();
}
