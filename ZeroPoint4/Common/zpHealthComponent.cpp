#include "zpCommon.h"

#if 0
zpHealthComponent::zpHealthComponent()
	: m_health( 1.f )
	, m_maxHealth( 1.f )
	, m_isInvulnerable( false )
{}
zpHealthComponent::~zpHealthComponent() {}

void zpHealthComponent::receiveMessage( const zpMessage& message ) {
	switch( message.getMessageType() ) {
	case zpMessageTypes::DAMAGE:
		{
			zp_float damage = message.getMessageData<zp_float>();
			loseHealth( damage );
		}
		break;
	case zpMessageTypes::HEAL:
		{
			zp_float heal = message.getMessageData<zp_float>();
			gainHealth( heal );
		}
		break;
	case zpMessageTypes::RESPAWN:
		{
			gainHealth( m_maxHealth + m_maxHealth );
		}
		break;
	}
}

void zpHealthComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeFloat( m_health, "@health" );

	out->writeFloat( m_maxHealth, "@max-health" );

	out->writeBoolean( m_isInvulnerable, "@is-invulnerable" );

	out->endBlock();
}
void zpHealthComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readFloat( &m_health, "@health" );
	
	in->readFloat( &m_maxHealth, "@max-health" );

	in->readBoolean( &m_isInvulnerable, "@is-invulnerable" );

	in->endBlock();
}

zp_float zpHealthComponent::getHealth() const {
	return m_health;
}
zp_float zpHealthComponent::getMaxHealth() const {
	return m_maxHealth;
}
zp_float zpHealthComponent::getHealthPercent() const {
	return m_health / m_maxHealth;
}

void zpHealthComponent::gainHealth( zp_float health ) {
	if( health < 0.f ) {
		loseHealth( -health );
	} else if( m_health < m_maxHealth ) {
		zp_float prevHealth = m_health;

		if( ( m_health + health ) >= m_maxHealth ) {
			m_health = m_maxHealth;
		} else {
			m_health += health;
		}

		zpHealthChangeData data;
		data.prevHealth = prevHealth;
		data.currentHealth = m_health;
		data.maxHealth = m_maxHealth;
		data.healthChange = health;

		zpMessage msg( zpMessageTypes::HEALTH_CHANGE, data, this );
		sendMessageToParentGameObject( msg );
	}
}
void zpHealthComponent::loseHealth( zp_float health ) {
	if( health < 0.f ) {
		gainHealth( -health );
	} else if( m_health > 0.f ) {
		if( m_isInvulnerable ) return;

		zp_float prevHealth = m_health;

		if( ( m_health - health ) < 0.f ) {
			m_health = 0.f;
		} else {
			m_health -= health;
		}

		zpHealthChangeData data;
		data.prevHealth = prevHealth;
		data.currentHealth = m_health;
		data.maxHealth = m_maxHealth;
		data.healthChange = -health;

		zpMessage msg( zpMessageTypes::HEALTH_CHANGE, data, this );
		sendMessageToParentGameObject( msg );
	}
}
void zpHealthComponent::setMaxHealth( zp_float health, zp_bool scaleCurrentHealth ) {
	if( health < 1.f ) {
		health = 1.f;
	}

	zp_float deltaHealth = 0.f;
	if( scaleCurrentHealth ) {
		deltaHealth = m_health - ( health * ( m_health / m_maxHealth ) );
	}
	m_maxHealth = health;

	if( deltaHealth > 0.f ) {
		gainHealth( deltaHealth );
	}
}
void zpHealthComponent::setInvulnerable( zp_bool invulnerable ) {
	m_isInvulnerable = invulnerable;
}

zp_bool zpHealthComponent::isDead() const {
	return m_health == 0.f;
}
zp_bool zpHealthComponent::isInvulnerable() const {
	return m_isInvulnerable;
}

void zpHealthComponent::onCreate() {}
void zpHealthComponent::onDestroy() {}

void zpHealthComponent::onUpdate() {}

void zpHealthComponent::onEnabled() {}
void zpHealthComponent::onDisabled() {}

#endif
