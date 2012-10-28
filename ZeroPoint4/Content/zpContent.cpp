#include "zpContent.h"

void zpContentRegisterSerializables() {
	zpRegisterSerializable::registerSerializable<zpContentManager>();
	zpRegisterSerializable::registerSerializable<zpLoadContentComponent>();
}