#pragma once
#ifndef ZP_AUDIO_H
#define ZP_AUDIO_H

#include "Core\zpCore.h"
#include "Content\zpContent.h"

enum zpAudioChannelGroup {
	ZP_AUDIO_CHANNEL_GROUP_MASTER,
	ZP_AUDIO_CHANNEL_GROUP_MUSIC,
	ZP_AUDIO_CHANNEL_GROUP_SFX,
	ZP_AUDIO_CHANNEL_GROUP_DIALOG,
};

enum zpAudioSpeakerMode {
	ZP_AUDIO_SPEAKER_MODE_MONO,
	ZP_AUDIO_SPEAKER_MODE_STEREO,
	ZP_AUDIO_SPEAKER_MODE_51_SURROUND,
	ZP_AUDIO_SPEAKER_MODE_71_SURROUND,
};

class zpAudioManager;
class zpAudioResourceCreator;
class zpAudioResource;
template<> class zpResourceInstance<zpAudioResource>;
class zpAudioEmitterComponent;

#include "zpAudioManager.h"
#include "zpAudioResourceCreator.h"
#include "zpAudioResource.h"
#include "zpAudioInstance.h"
#include "zpAudioEmitterComponent.h"

void zpAudioRegisterSerializables();

#endif