#pragma once
#ifndef ZP_CORE_H
#define ZP_CORE_H

#define ZP_USE_SIMD		0

#define ZP_INLINE		inline
#define ZP_FORCE_INLINE	__forceinline

#include "zpBaseTypes.h"
#include "zpMath.h"

class zpTime;
class zpGameObject;
class zpGameObjectComponent;

#include "zpTime.h"

#include "zpLink.h"
#include "zpFlag.h"

#include "zpGameObject.h"
#include "zpGameObjectComponent.h"


#endif