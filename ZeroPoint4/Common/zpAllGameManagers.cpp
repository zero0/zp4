#include "zpCommon.h"
//#include "Audio/zpAudio.h"
//#include "Content/zpContent.h"
//#include "Physics/zpPhysics.h"
//#include "Rendering/zpRendering.h"
//#include "Input/zpInput.h"
//#include "Scripting/zpScripting.h"

zpAllGameManagers::zpAllGameManagers()
	: zpSerializable()
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) , m_##mng( new zp##mng )
	#include "zpAllGameManagers.inl"
{}
zpAllGameManagers::~zpAllGameManagers()
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) ZP_SAFE_DELETE( m_##mng );
	#include "zpAllGameManagers.inl"
}

#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) zp##mng* zpAllGameManagers::get##mng() { return m_##mng; }
#include "zpAllGameManagers.inl"

void zpAllGameManagers::serialize( zpSerializedOutput* out )
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->serialize( out );
#include "zpAllGameManagers.inl"
}
void zpAllGameManagers::deserialize( zpSerializedInput* in )
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->deserialize( in );
#include "zpAllGameManagers.inl"
}

void zpAllGameManagers::update()
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->update();
#include "zpAllGameManagers.inl"
}
void zpAllGameManagers::simulate()
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->simulate();
#include "zpAllGameManagers.inl"
}

void zpAllGameManagers::create()
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->create();
#include "zpAllGameManagers.inl"
}
void zpAllGameManagers::destroy()
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->destroy();
#include "zpAllGameManagers.inl"
}

void zpAllGameManagers::setGame( zpGame* game )
{
#undef ZP_GAME_MANAGER_DEF
#define ZP_GAME_MANAGER_DEF( mng ) m_##mng->setGame( game );
#include "zpAllGameManagers.inl"
}
