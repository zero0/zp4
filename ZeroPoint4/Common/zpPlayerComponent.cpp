#include "zpCommon.h"

zpPlayerComponent::zpPlayerComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
{
    m_player = (zpPlayerNumber)def[ "Player" ].asInt();
    m_input = new zpControllerPlayerInput();
}
zpPlayerComponent::~zpPlayerComponent()
{
    ZP_SAFE_DELETE( m_input );
}

void zpPlayerComponent::onCreate()
{
}
void zpPlayerComponent::onInitialize()
{
    m_input->setup( this );
}
void zpPlayerComponent::onDestroy()
{
    m_input->teardown();
}

void zpPlayerComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    m_input->update( deltaTime, realTime );
}
void zpPlayerComponent::onSimulate()
{

}

void zpPlayerComponent::onEnabled()
{

}
void zpPlayerComponent::onDisabled()
{

}

zpControllerPlayerInput::zpControllerPlayerInput()
{}
zpControllerPlayerInput::~zpControllerPlayerInput()
{}
void zpControllerPlayerInput::setup( zpPlayerComponent* player )
{
    m_player = player;
    //m_controller = m_player->getApplication()->getInputManager()->getController( ZP_CONTROLLER_1 );
    m_keyboard = m_player->getApplication()->getInputManager()->getKeyboard();
}
void zpControllerPlayerInput::teardown()
{
    m_player = ZP_NULL;
    m_controller = ZP_NULL;
    m_keyboard = ZP_NULL;
}
void zpControllerPlayerInput::update( zp_float deltaTime, zp_float realTime )
{
    zpKinematicBodyComponent* kinematic = m_player->getParentObject()->getComponents()->getKinematicBodyComponent();
    if( kinematic != ZP_NULL )
    {
        zp_bool keyPressed = false;

        zpVector4f walk = zpMath::Vector4( 0, 0, 0, 0 ); //m_player->getParentObject()->getComponents()->getTransformComponent();
        zpScalar speed = zpMath::Scalar( 5 );

        if( m_keyboard->isKeyDown( ZP_KEY_CODE_W ) )
        {
            walk = zpMath::Vector4Add( walk, zpMath::Vector4( 0, 0, 1, 0 ) );
            keyPressed = true;
        }
        else if( m_keyboard->isKeyDown( ZP_KEY_CODE_S ) )
        {
            walk = zpMath::Vector4Add( walk, zpMath::Vector4( 0, 0, -1, 0 ) );
            keyPressed = true;
        }

        if( m_keyboard->isKeyDown( ZP_KEY_CODE_A ) )
        {
            walk = zpMath::Vector4Add( walk, zpMath::Vector4( -1, 0, 0, 0 ) );
            keyPressed = true;
        }
        else if( m_keyboard->isKeyDown( ZP_KEY_CODE_D ) )
        {
            walk = zpMath::Vector4Add( walk, zpMath::Vector4( 1, 0, 0, 0 ) );
            keyPressed = true;
        }

        if( m_keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) )
        {
            speed = zpMath::ScalarMul( speed, zpMath::Scalar( 3 ) );
        }

        if( keyPressed )
        {
            walk = zpMath::Vector4Normalize3( walk );
            kinematic->walk( walk, speed );
        }
        else
        {
            kinematic->stop();
        }

        if( m_keyboard->isKeyPressed( ZP_KEY_CODE_SPACE ) )
        {
            kinematic->jump();
        }
    }
}



zpPlayerComponentPool::zpPlayerComponentPool()
{

}
zpPlayerComponentPool::~zpPlayerComponentPool()
{

}

void zpPlayerComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpPlayerComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
