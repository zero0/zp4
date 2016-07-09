#pragma once
#ifndef ZP_PLAYER_COMPONENT_H
#define ZP_PLAYER_COMPONENT_H

enum zpPlayerNumber
{
    ZP_PLAYER_NUMBER_1,
    ZP_PLAYER_NUMBER_2,
    ZP_PLAYER_NUMBER_3,
    ZP_PLAYER_NUMBER_4,

    zpPlayerNumber_Count,

    ZP_PLAYER_NUMBER_EMPTY = -1,
};

class zpPlayerComponent;

ZP_PURE_INTERFACE zpPlayerInput
{
public:
    virtual ~zpPlayerInput() {}

    virtual void setup( zpPlayerComponent* player ) = 0;
    virtual void teardown() = 0;

    virtual void update( zp_float deltaTime, zp_float realTime ) = 0;
};

class zpEmptyPlayerInput : public zpPlayerInput
{
public:
    zpEmptyPlayerInput() {}
    virtual ~zpEmptyPlayerInput() {}

    void setup( zpPlayerComponent* player ) {}
    void teardown() {}

    void update( zp_float deltaTime, zp_float realTime ) {}
};

class zpControllerPlayerInput : public zpPlayerInput
{
public:
    zpControllerPlayerInput();
    virtual ~zpControllerPlayerInput();

    void setup( zpPlayerComponent* player );
    void teardown();

    void update( zp_float deltaTime, zp_float realTime );

private:
    zpPlayerComponent* m_player;
    const zpController* m_controller;
    const zpKeyboard* m_keyboard;
};

class zpAIPlayerInput : public zpPlayerInput
{

};

class zpPlayerComponent : public zpComponent
{
public:
    zpPlayerComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpPlayerComponent();

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    zpPlayerNumber m_player;
    
    zpPlayerInput* m_input;
};


class zpPlayerComponentPool : public zpContentPool< zpPlayerComponent, zpPlayerNumber_Count >
{
public:
    zpPlayerComponentPool();
    virtual ~zpPlayerComponentPool();

    void update( zp_float deltaTime, zp_float realTime );
};

#endif
