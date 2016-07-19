#pragma once
#ifndef ZP_ALL_COMPONENTS_H
#define ZP_ALL_COMPONENTS_H

class zpApplication;
class zpObject;

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            class zp##cmp##Component;
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

class zpAllComponents
{
public:
    zpAllComponents();
    ~zpAllComponents();

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            zp##cmp##Component* get##cmp##Component();
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            zp##cmp##Component* add##cmp##Component( const zpBison::Value& def );
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    void create( const zp_char* componentName, const zpBison::Value& cmp );
    void initialize();
    void unload();

    void setEnabled( zp_bool enabled );

    void setup( zpObject* obj );

private:
    zpObject* m_object;

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            zp##cmp##Component* m_##cmp;
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF
};

#endif
