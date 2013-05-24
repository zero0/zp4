#pragma once
#ifndef ZP_ALL_COMPONENTS_H
#define ZP_ALL_COMPONENTS_H

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) class zp##cmp;
#include "zpAllComponents.inl"

class zpAllComponents
{
public:
	zpAllComponents();
	~zpAllComponents();

	void create();
	void destroy();

	void simulate();
	void update();

	void receiveMessage( const zpMessage& message );

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) zp##cmp* get##cmp();
	#include "zpAllComponents.inl"

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

private:
	zp_ptr m_unused;
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )	zp##cmp* m_##cmp;
	#include "zpAllComponents.inl"
};

#endif