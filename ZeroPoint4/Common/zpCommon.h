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

class zpCameraComponent;
class zpMeshRendererComponent;
class zpParticleEmitterComponent;

class zpRigidBodyComponent;

class zpBreakableComponent;
class zpAttachmentComponent;

#include "zpEvent.h"
#include "zpGUI.h"

#include "zpComponent.h"

#include "zpEditorCameraComponent.h"
#include "zpScriptComponent.h"

// Audio
#include "zpAudioEmitterComponent.h"
#include "zpAudioListenerComponent.h"

// Rendering
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


#endif
