#include "zpCommon.h"

zpAudioEmitterComponent::zpAudioEmitterComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_isMoving( false )
	, m_prevPosition( 0 )
{
	m_isMoving = def[ "IsMoving" ].asBool();
}
zpAudioEmitterComponent::~zpAudioEmitterComponent()
{
	m_sounds.clear();
}

void zpAudioEmitterComponent::playSound( const zpString& filename )
{
	zpAudioResourceInstance* sound = ZP_NULL;
	if( !m_sounds.find( filename, &sound ) )
	{
		zpAudioResourceInstance& inst = m_sounds[ filename ];
		if( getApplication()->getAudioContentManager()->getResource( filename.str(), inst ) )
		{
			sound = &inst;
		}
	}
	if( sound && !sound->isPlaying() ) sound->play();
}
void zpAudioEmitterComponent::stopSound( const zpString& filename )
{
	zpAudioResourceInstance* sound;
	if( m_sounds.find( filename, &sound ) )
	{
		sound->stop();
	}
}
void zpAudioEmitterComponent::stopAllSounds()
{
	m_sounds.foreach( []( const zpString& key, zpAudioResourceInstance& sound )
	{
		sound.stop();
	} );
}

void zpAudioEmitterComponent::clearSoundCache()
{
	stopAllSounds();
	m_sounds.clear();
}

zp_bool zpAudioEmitterComponent::isMovingSound() const
{
	return m_isMoving;
}

void zpAudioEmitterComponent::onCreate()
{}
void zpAudioEmitterComponent::onInitialize()
{
	m_prevPosition = getParentObject()->getComponents()->getTransformComponent()->getWorldPosition();
}
void zpAudioEmitterComponent::onDestroy()
{}
	
void zpAudioEmitterComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	if( m_isMoving )
	{
		if( !m_sounds.isEmpty() )
		{
			zp_float dt = deltaTime;

			zpVector4f pos( getParentObject()->getComponents()->getTransformComponent()->getWorldPosition() );
			zpVector4f vel;

			zpMath::Sub( vel, pos, m_prevPosition );
			zpMath::Div( vel, vel, zpScalar( dt ) );
			m_prevPosition = pos;

			m_sounds.foreach( [ &pos, &vel ]( const zpString& key, zpAudioResourceInstance& sound )
			{
				sound.setPosition( pos );
				sound.setVelocity( vel );
			} );
		}
	}
}
void zpAudioEmitterComponent::onSimulate()
{}
	
void zpAudioEmitterComponent::onEnabled()
{}
void zpAudioEmitterComponent::onDisabled()
{}



zpAudioEmitterComponentPool::zpAudioEmitterComponentPool() {}
zpAudioEmitterComponentPool::~zpAudioEmitterComponentPool() {}

void zpAudioEmitterComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpAudioEmitterComponent* o )
	{
		o->update( deltaTime, realTime );
	} );
}
void zpAudioEmitterComponentPool::simulate()
{
	m_used.foreach( []( zpAudioEmitterComponent* o )
	{
		o->simulate();
	} );
}
