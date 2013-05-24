#include "zpCommon.h"
//#include "Audio/zpAudio.h"
//#include "Content/zpContent.h"
//#include "Physics/zpPhysics.h"
//#include "Rendering/zpRendering.h"
//#include "Scripting/zpScripting.h"

zpAllComponents::zpAllComponents()
	: m_unused( 0 )
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) , m_##cmp( ZP_NULL )
	#include "zpAllComponents.inl"
{}
zpAllComponents::~zpAllComponents()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) { zp##cmp##Pool::getInstance()->release( m_##cmp ); m_##cmp = ZP_NULL; }
	#include "zpAllComponents.inl"
}

void zpAllComponents::create()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) m_##cmp->create();
	#include "zpAllComponents.inl"
}
void zpAllComponents::destroy()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) m_##cmp->destroy();
	#include "zpAllComponents.inl"
}

void zpAllComponents::simulate()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) m_##cmp->simulate();
	#include "zpAllComponents.inl"
}
void zpAllComponents::update()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) m_##cmp->update();
	#include "zpAllComponents.inl"
}

void zpAllComponents::receiveMessage( const zpMessage& message )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) m_##cmp->receiveMessage( message );
	#include "zpAllComponents.inl"
}

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) zp##cmp* zpAllComponents::get##cmp() { return m_##cmp; }
#include "zpAllComponents.inl"

void zpAllComponents::serialize( zpSerializedOutput* out )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) m_##cmp->serialize( out );
	#include "zpAllComponents.inl"
}
void zpAllComponents::deserialize( zpSerializedInput* in )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) in->readSerializableOfType( &m_##cmp ); //if( m_##cmp ) m_##cmp->deserialize( in );
	#include "zpAllComponents.inl"
}
