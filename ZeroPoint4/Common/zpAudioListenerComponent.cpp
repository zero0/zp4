#include "zpCommon.h"

zpAudioListenerComponent::zpAudioListenerComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_engine( getApplication()->getAudioEngine() )
{
    const zpBison::Value& rolloff = def[ "Rolloff" ];
    const zpBison::Value& dist = def[ "Distance" ];
    const zpBison::Value& doppler = def[ "Doppler" ];

    if( rolloff.isFloat() )
    {
        setRolloffFactor( rolloff.asFloat() );
    }
    if( dist.isFloat() )
    {
        setDistanceFactor( dist.asFloat() );
    }
    if( doppler.isFloat() )
    {
        setDopplerFactor( doppler.asFloat() );
    }
}
zpAudioListenerComponent::~zpAudioListenerComponent()
{}

void zpAudioListenerComponent::setRolloffFactor( zp_float rolloff )
{
    m_engine->setListenerRolloff( rolloff );
}
void zpAudioListenerComponent::setDistanceFactor( zp_float dist )
{
    m_engine->setListenerDistance( dist );
}
void zpAudioListenerComponent::setDopplerFactor( zp_float doppler )
{
    m_engine->setListenerDoppler( doppler );
}

void zpAudioListenerComponent::onCreate()
{}
void zpAudioListenerComponent::onInitialize()
{
    m_oldPosition = getParentObject()->getComponents()->getTransformComponent()->getWorldPosition();
}
void zpAudioListenerComponent::onDestroy()
{}

void zpAudioListenerComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    const zpMatrix4f& transform = getParentObject()->getComponents()->getTransformComponent()->getWorldTransform();

    zpScalar dt = zpMath::Scalar( deltaTime );

    zpVector4f up( transform.r[ 1 ] );
    zpVector4f fwd( transform.r[ 2 ] );
    zpVector4f pos( transform.r[ 3 ] );
    zpVector4f vel;
    vel = zpMath::Vector4Sub( pos, m_oldPosition );
    vel = zpMath::Vector4Mul( vel, dt );

    m_engine->setListenerPosition( pos );
    m_engine->setListenerVelocity( vel );
    m_engine->setListenerOrientation( fwd, up );

    m_oldPosition = pos;
}
void zpAudioListenerComponent::onSimulate()
{}

void zpAudioListenerComponent::onEnabled()
{}
void zpAudioListenerComponent::onDisabled()
{}


zpAudioListenerComponentPool::zpAudioListenerComponentPool()
{}
zpAudioListenerComponentPool::~zpAudioListenerComponentPool()
{}

void zpAudioListenerComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpAudioListenerComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
void zpAudioListenerComponentPool::simulate()
{
    m_used.foreach( []( zpAudioListenerComponent* o )
    {
        o->simulate();
    } );
}
