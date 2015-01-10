#include "zpCommon.h"

zpAudioListenerComponent::zpAudioListenerComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_engine( getApplication()->getAudioContentManager()->getAudioEngine() )
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

void zpAudioListenerComponent::onUpdate()
{
	const zpMatrix4f& transform = getParentObject()->getComponents()->getTransformComponent()->getWorldTransform();

	zpScalar dt( zpTime::getInstance()->getDeltaSeconds() );

	zpVector4f fwd( transform.getRow( 2 ) ), up( transform.getRow( 1 ) ), pos( transform.getRow( 3 ) ), vel;
	zpMath::Sub( vel, pos, m_oldPosition );
	zpMath::Mul( vel, vel, dt );

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

void zpAudioListenerComponentPool::update()
{
	m_used.foreach( []( zpAudioListenerComponent* o )
	{
		o->update();
	} );
}
void zpAudioListenerComponentPool::simulate()
{
	m_used.foreach( []( zpAudioListenerComponent* o )
	{
		o->simulate();
	} );
}
