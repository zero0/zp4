#include "zpAudio.h"

void zpAudioRegisterSerializables() {
	zpRegisterSerializable::registerSerializable<zpAudioManager>();
	zpRegisterSerializable::registerSerializable<zpAudioResourceCreator>();
	zpRegisterSerializable::registerSerializable<zpAudioEmitterComponent>();
}