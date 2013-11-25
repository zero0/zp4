#include "zpCommon.h"
//#include "Audio/zpAudio.h"
//#include "Content/zpContent.h"
//#include "Physics/zpPhysics.h"
//#include "Rendering/zpRendering.h"
//#include "Scripting/zpScripting.h"

zpAllComponents::zpAllComponents()
	: m_app( ZP_NULL )
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) , m_##cmp( ZP_NULL )
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
{}
zpAllComponents::~zpAllComponents()
{
	unload();
}

void zpAllComponents::load( zpObject* obj, const zp_char* componentName, const zpBison::Value& def )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( zp_strcmp( componentName, #cmp ) == 0 ) { m_##cmp = m_app->get##cmp##ComponentPool()->create( obj, def ); m_##cmp->create(); }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) { m_##cmp->initialize(); }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
}
void zpAllComponents::unload()
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) { m_##cmp->destroy(); m_app->get##cmp##ComponentPool()->destroy( m_##cmp ); m_##cmp = ZP_NULL; }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
}

void zpAllComponents::setEnabled( zp_bool enabled )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) { m_##cmp->setEnabled( enabled ); }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
}
