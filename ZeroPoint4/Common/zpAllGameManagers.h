#pragma once
#ifndef ZP_ALL_GAME_MANAGERS_H
#define ZP_ALL_GAME_MANAGERS_H

#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) class zp##mng;
#include "zpAllGameManagers.inl"

class zpAllGameManagers : public zpSerializable
{
public:
	zpAllGameManagers();
	~zpAllGameManagers();

#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) zp##mng* get##mng();
#include "zpAllGameManagers.inl"

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void update();
	void simulate();

	void create();
	void destroy();

	//void setGame( zpGame* game );

private:
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) zp##mng* m_##mng;
#include "zpAllGameManagers.inl"

};

#endif