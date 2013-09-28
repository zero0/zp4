#pragma once
#ifndef ZP_ALL_COMPONENTS_H
#define ZP_ALL_COMPONENTS_H

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) class zp##cmp;
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

class zpAllComponents
{
public:
	zpAllComponents();
	~zpAllComponents();

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) zp##cmp##Component* get##cmp##Component() { return m_##cmp; }
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

	void load( zpObject* obj, const zp_char* componentName, const zpBison::Value& cmp );
	void unload();

	void setApplication( zpApplication* app ) { m_app = app; }

private:
	zpApplication* m_app;

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )	zp##cmp##Component* m_##cmp;
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
};

#endif