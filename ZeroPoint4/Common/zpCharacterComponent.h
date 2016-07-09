#pragma once
#ifndef ZP_CHARACTER_COMPONENT_H
#define ZP_CHARACTER_COMPONENT_H

struct zpCharacterState
{
    zpString animationName;

    zpString onStateEnteredMessage;
    zpString onStateExitedMessage;

    zp_int nextStateIndex;
    zp_float nextStateWaitTime;

    zp_float stateEnterCrossFade;
    zp_float stateExitCrossFade;
};

class zpCharacterComponent : public zpComponent
{
public:
    zpCharacterComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpCharacterComponent();

    zp_int getCurrentStateIndex() const;
    const zpString& getCurrentState() const;

    void setState( const zpString& stateName );

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    zp_int m_defaultState;
    zp_int m_currentState;

    zpArrayList< zpCharacterState > m_states;
    zpArrayList< zpString > m_stateNames;
};

#endif
