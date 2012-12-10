#include "zpPhysics.h"
#include "zpPhysicsLib.inc"

ZP_SERIALIZE_POOLED_OBJECT( ColliderComponent )
ZP_SERIALIZE_POOLED_OBJECT( KinematicComponent )
ZP_SERIALIZE_POOLED_OBJECT( PhantomComponent )
ZP_SERIALIZE_POOLED_OBJECT( RigidBodyComponent )

void zpPhysicsRegisterSerializables() {
	zpRegisterSerializable::registerSerializable<zpPhysicsManager>();
	ZP_REGISTER_SERIALZED_POOL( ColliderComponent );
	ZP_REGISTER_SERIALZED_POOL( KinematicComponent );
	ZP_REGISTER_SERIALZED_POOL( PhantomComponent );
	ZP_REGISTER_SERIALZED_POOL( RigidBodyComponent );
}