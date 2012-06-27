#include "zpAudio.h"
#include <stdio.h>

zpAudioResourceCreator::zpAudioResourceCreator() {}
zpAudioResourceCreator::~zpAudioResourceCreator() {}

zpResource* zpAudioResourceCreator::createResource( const zpString& filename ) {

	zpAudioChannelGroup group = ZP_AUDIO_CHANNEL_GROUP_SFX;
	zp_bool is2DSound = filename.indexOf( "_2d" ) != zpString::npos;
	zp_bool is3DSound = filename.indexOf( "_3d" ) != zpString::npos;
	zp_bool isLooping = filename.indexOf( "_loop" ) != zpString::npos;
	zp_bool isStreaming = filename.indexOf( "_stream" ) != zpString::npos;
	zp_uint loopFromIndex = filename.indexOf( "_from" );
	zp_uint loopToIndex = filename.indexOf( "_to" );
	zp_uint loopFrom = ZP_AUDIO_LOOP_POINT_NONE;
	zp_uint loopTo = ZP_AUDIO_LOOP_POINT_NONE;

	if( isLooping ) {
		if( loopFromIndex != zpString::npos ) {
			zp_uint end = filename.findFirstOf( "_.", loopFromIndex + 1 );
			if( end != zpString::npos ) {
				zpString sub = filename.substring( loopFromIndex, end - loopFromIndex );
				sscanf_s( sub.c_str(), "_from%d", &loopFrom );
			}
		}
		if( loopToIndex != zpString::npos ) {
			zp_uint end = filename.findFirstOf( "_.", loopToIndex + 1 );
			if( end != zpString::npos ) {
				zpString sub = filename.substring( loopToIndex, end - loopToIndex );
				sscanf_s( sub.c_str(), "_to%d", &loopTo );
			}
		}
	}

	if( filename.indexOf( "sfx/" ) != zpString::npos ) {
		group = ZP_AUDIO_CHANNEL_GROUP_SFX;
	} else if( filename.indexOf( "music/" ) != zpString::npos ) {
		group = ZP_AUDIO_CHANNEL_GROUP_MUSIC;
	} else if( filename.indexOf( "dialog/" ) != zpString::npos ) {
		group = ZP_AUDIO_CHANNEL_GROUP_DIALOG;
	}

	zpAudioResource* resource = new zpAudioResource();
	resource->setIsStreaming( isStreaming );
	resource->setIsLooping( isLooping );
	resource->setIs3DSound( is3DSound );
	resource->setChannelGroup( group );
	resource->setLoopFromTo( loopFrom, loopTo );
	
	return resource;
}
