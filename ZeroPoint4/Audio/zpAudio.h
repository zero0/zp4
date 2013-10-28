#pragma once
#ifndef ZP_AUDIO_H
#define ZP_AUDIO_H

#include "Core\zpCore.h"
#include "Content\zpContent.h"

struct zpAudioBuffer
{
	zp_handle soundBuffer;
	zp_handle soundBuffer3D;
};

enum zpAudioChannelGroup
{
	ZP_AUDIO_CHANNEL_GROUP_MASTER,
	ZP_AUDIO_CHANNEL_GROUP_MUSIC,
	ZP_AUDIO_CHANNEL_GROUP_SFX,
	ZP_AUDIO_CHANNEL_GROUP_DIALOG,
};

enum zpAudioSpeakerMode
{
	ZP_AUDIO_SPEAKER_MODE_MONO,
	ZP_AUDIO_SPEAKER_MODE_HEADPHONE,
	ZP_AUDIO_SPEAKER_MODE_STEREO,
	ZP_AUDIO_SPEAKER_MODE_51_SURROUND,
	ZP_AUDIO_SPEAKER_MODE_71_SURROUND,
};

enum zpAudioType
{
	ZP_AUDIO_TYPE_2D,
	ZP_AUDIO_TYPE_3D,
};

class zpAudioEngine;

#include "zpAudioResource.h"
#include "zpAudioEngine.h"

#endif
