#pragma once
#ifndef ZP_GAME_OBJECT_H
#define ZP_GAME_OBJECT_H

class zpGameObject {
public:
	zpGameObject();
	~zpGameObject();

private:
	zpVector4 m_vector;
};

#endif