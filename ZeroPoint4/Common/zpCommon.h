#pragma once
#ifndef ZP_COMMON_H
#define ZP_COMMON_H

#include "Core\zpCore.h"
#include "Input\zpInput.h"
#include "Content\zpContent.h"
#include "Rendering\zpRendering.h"
#include "Scripting\zpScripting.h"
#include "Audio\zpAudio.h"
#include "Physics\zpPhysics.h"

class zpGUI;
class zpApplication;
class zpComponent;
class zpWorld;
class zpObject;

class zpEditorCameraComponent;
class zpScriptComponent;

class zpAudioEmitterComponent;
class zpAudioListenerComponent;

class zpLightComponent;
class zpCameraComponent;
class zpMeshRendererComponent;
class zpParticleEmitterComponent;

class zpRigidBodyComponent;

class zpBreakableComponent;
class zpAttachmentComponent;

class zpAllComponents;

#include "zpEvent.h"
#include "zpGUI.h"

#include "zpComponent.h"

#include "zpEditorCameraComponent.h"
#include "zpScriptComponent.h"

// Audio
#include "zpAudioEmitterComponent.h"
#include "zpAudioListenerComponent.h"

// Rendering
#include "zpLightComponent.h"
#include "zpCameraComponent.h"
#include "zpMeshRendererComponent.h"
#include "zpParticleEmitterComponent.h"

// Physics
#include "zpRigidBodyComponent.h"

#include "zpBreakableComponent.h"
#include "zpAttachmentComponent.h"

#include "zpAllComponents.h"
#include "zpAllGameManagers.h"

#include "zpObject.h"
#include "zpWorld.h"
#include "zpApplication.h"


template< typename T, zp_uint count >
zp_bool zpBisonStringToEnum( const zp_char* (&e)[ count ], const zpBison::Value& str, T& t )
{
	zp_bool ok = false;

	if( str.isString() )
	{
		const zp_char* s = str.asCString();
		for( zp_uint i = 0; i < count; ++i )
		{
			if( zp_strcmp( s, e[ i ] ) == 0 )
			{
				t = (T)i;
				ok = true;
				break;
			}
		}
	}

	return ok;
}

ZP_FORCE_INLINE zpVector4f zpBisonArray2ToVector4( const zpBison::Value& vec, zp_float z, zp_float w )
{
	return zpVector4f( vec[0].asFloat(), vec[1].asFloat(), z, w );
}
ZP_FORCE_INLINE zpVector4f zpBisonArray3ToVector4( const zpBison::Value& vec, zp_float w )
{
	return zpVector4f( vec[0].asFloat(), vec[1].asFloat(), vec[2].asFloat(), w );
}
ZP_FORCE_INLINE zpVector4f zpBisonArray4ToVector4( const zpBison::Value& vec )
{
	return zpVector4f( vec[0].asFloat(), vec[1].asFloat(), vec[2].asFloat(), vec[3].asFloat() );
}
ZP_FORCE_INLINE void zpBisonArray4ToColor4( const zpBison::Value& color, zpColor4f& c )
{
	c.set( color[0].asFloat(), color[1].asFloat(), color[2].asFloat(), color[3].asFloat() );
}
ZP_FORCE_INLINE void zpBisonObjectToColor4( const zpBison::Value& color, zpColor4f& c )
{
	c.set( color["r"].asFloat(), color["g"].asFloat(), color["b"].asFloat(), color["a"].asFloat() );
}

#endif
