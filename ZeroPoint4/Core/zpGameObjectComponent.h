#pragma once
#ifndef ZP_GAME_OBJECT_COMPONENT_H
#define ZP_GAME_OBJECT_COMPONENT_H

class zpGameObjectComponent : public zpIntrusiveListNode<zpGameObjectComponent> {
public:
	zpGameObjectComponent() {}
	virtual ~zpGameObjectComponent() {}

private:
};

#endif