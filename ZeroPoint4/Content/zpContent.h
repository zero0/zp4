#pragma once
#ifndef ZP_CONTENT_H
#define ZP_CONTENT_H

#include "Core\zpCore.h"

class zpXmlParser;

class zpContentManager;
template<typename R> class zpResourceInstance;
ZP_ABSTRACT_CLASS zpResource;
ZP_ABSTRACT_CLASS zpResourceCreator;
class zpPrefabs;

class zpXmlSerializedInput;
class zpXmlSerializedOutput;

class zpLoadContentComponent;

#include "zpXmlParser.h"

#include "zpContentManager.h"
#include "zpResource.h"
#include "zpResourceCreator.h"
#include "zpPrefabs.h"

#include "zpXmlSerializedInput.h"
#include "zpXmlSerializedOutput.h"

#include "zpLoadContentComponent.h"

void zpContentRegisterSerializables();

#endif