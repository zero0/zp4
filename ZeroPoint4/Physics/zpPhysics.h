#pragma once
#ifndef ZP_PHYSICS_H
#define ZP_PHYSICS_H

#include "Core\zpCore.h"

#include "zpPhysicsLib.inc"

class zpPhysicsManager;
class zpColliderComponent;
class zpRigidBodyComponent;
class zpKinematicComponent;
class zpPhantomComponent;

#include "zpPhysicsManager.h"
#include "zpColliderComponent.h"
#include "zpRigidBodyComponent.h"
#include "zpKinematicComponent.h"
#include "zpPhantomComponent.h"

typedef zpPooledObject<zpColliderComponent, 128> zpColliderComponentPool;
typedef zpPooledObject<zpRigidBodyComponent, 128> zpRigidBodyComponentPool;
typedef zpPooledObject<zpKinematicComponent, 128> zpKinematicComponentPool;
typedef zpPooledObject<zpPhantomComponent, 128> zpPhantomComponentPool;

void zpPhysicsRegisterSerializables();

#endif