#pragma once
#ifndef ZP_CONTENT_H
#define ZP_CONTENT_H

#include "Core\zpCore.h"

class zpContentManager;
template<typename R> class zpResourceInstance;
ZP_ABSTRACT_CLASS zpResource;
ZP_ABSTRACT_CLASS zpResourceCreator;
class zpPrefabs;
class zpXmlParser;
class zpXmlSerializedInput;

#include "zpContentManager.h"
#include "zpResource.h"
#include "zpResourceCreator.h"
#include "zpPrefabs.h"
#include "zpXmlParser.h"
#include "zpXmlSerializedInput.h"

#endif