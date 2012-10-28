#pragma once
#ifndef ZP_INPUT_H
#define ZP_INPUT_H

#include "Core\zpCore.h"

class zpMouse;
class zpKeyboard;
class zpController;
class zpInputManager;

#include "zpMouse.h"
#include "zpKeyboard.h"
#include "zpController.h"
#include "zpInputManager.h"

void zpInputRegisterSerializables();

#endif