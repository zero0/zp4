#include "zpAudio.h"

ZP_SERIALIZE_POOLED_OBJECT( AudioEmitterComponent )
ZP_SERIALIZE_POOLED_OBJECT( AudioListenerComponent )

void zpAudioRegisterSerializables() {
	zpRegisterSerializable::registerSerializable<zpAudioManager>();
	zpRegisterSerializable::registerSerializable<zpAudioResourceCreator>();

	ZP_REGISTER_SERIALZED_POOL( AudioEmitterComponent );
	ZP_REGISTER_SERIALZED_POOL( AudioListenerComponent );
}