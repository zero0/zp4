#pragma once
#ifndef ZP_PHANTOM_COMPONENT_H
#define ZP_PHANTOM_COMPONENT_H

class zpPhantomComponent : public zpComponent, public zpPhantomCollisionCallback
{
public:
    zpPhantomComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpPhantomComponent();

    void onCollisionEnter( const zpPhantomCollisionHitInfo& hit );
    void onCollisionStay( const zpPhantomCollisionHitInfo& hit );
    void onCollisionLeave( zp_handle otherObject );

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    zp_bool m_addOnEnable;
    zp_bool m_addOnCreate;
    zp_bool m_isAdded;
    
    zpPhantom m_phantom;

    zpString m_eventOnEnter;
    zpString m_eventOnStay;
    zpString m_eventOnLeave;

    zpString m_messageOnEnter;
    zpString m_messageOnStay;
    zpString m_messageOnLeave;
};

class zpPhantomComponentPool : public zpContentPool< zpPhantomComponent, 8 >
{
public:
    zpPhantomComponentPool();
    virtual ~zpPhantomComponentPool();

    void update( zp_float deltaTime, zp_float realTime );
};

#endif
