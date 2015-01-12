#include "zpCommon.h"

zpAllComponents::zpAllComponents()
	: m_app( ZP_NULL )
	, m_object( ZP_NULL )
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) , m_##cmp( ZP_NULL )
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
{}
zpAllComponents::~zpAllComponents()
{
	unload();
}

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) zp##cmp##Component* zpAllComponents::get##cmp##Component() { return m_##cmp; }
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) zp##cmp##Component* zpAllComponents::add##cmp##Component( const zpBison::Value& def ) { if( m_##cmp == ZP_NULL ) { m_##cmp = m_app->get##cmp##ComponentPool()->create( m_object, def ); } return m_##cmp; }
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

void zpAllComponents::create( const zp_char* componentName, const zpBison::Value& def )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( zp_strcmp( componentName, #cmp ) == 0 ) { m_##cmp = m_app->get##cmp##ComponentPool()->create( m_object, def ); m_##cmp->create(); return; }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

	ZP_ASSERT( false, "Unknown component name %s", componentName );
}

void zpAllComponents::initialize()
{
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

	m_object = ZP_NULL;
	m_app = ZP_NULL;
}

void zpAllComponents::setEnabled( zp_bool enabled )
{
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) if( m_##cmp ) { m_##cmp->setEnabled( enabled ); }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
}

void zpAllComponents::setup( zpApplication* app, zpObject* obj )
{
	m_app = app;
	m_object = obj;
}
