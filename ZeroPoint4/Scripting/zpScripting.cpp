#include "zpScripting.h"

void zpScriptingRegisterSerializables() {
	zpRegisterSerializable::registerSerializable<zpScriptingManager>();
	zpRegisterSerializable::registerSerializable<zpScriptingResourceCreator>();
	zpRegisterSerializable::registerSerializable<zpScriptingComponent>();
}