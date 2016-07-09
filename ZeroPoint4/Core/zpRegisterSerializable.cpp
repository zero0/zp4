#include "zpCore.h"

#if 0
zpHashMap<zpString, zpRegisterSerializable::CreateSerializableFunction> zpRegisterSerializable::s_creators;

zpRegisterSerializable::zpRegisterSerializable() {}
zpRegisterSerializable::zpRegisterSerializable( const zpRegisterSerializable& ) {}
zpRegisterSerializable::~zpRegisterSerializable() {}

void zpRegisterSerializable::operator=( const zpRegisterSerializable& ) {}

void zpRegisterSerializable::registerSerializable( const zpString& name, zpRegisterSerializable::CreateSerializableFunction createFunc ) {
    s_creators[ name ] = createFunc;
}

zpSerializable* zpRegisterSerializable::createSerializable( const zpString& name ) {
    return s_creators[ name ]();
}
#endif
