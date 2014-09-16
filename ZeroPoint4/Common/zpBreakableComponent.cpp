#include "zpCommon.h"

#define BREAK_HEALTH	(1e-6f)

zpBreakableComponent::zpBreakableComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_justBroke( false )
	, m_destroyObjectOnBreak( true )
	, m_health( 1.0f )
	, m_maxHealth( 1.0f )
	, m_healthDecayPerSecond( 0.0f )
{
	const zpBison::Value& health = def[ "Health" ];
	if( health.isNumeric() )
	{
		m_health = health.asFloat();
		m_maxHealth = m_health;
	}

	const zpBison::Value& decay = def[ "HealthDecayPerSecond" ];
	if( decay.isNumeric() )
	{
		m_healthDecayPerSecond = decay.asFloat();
	}

	const zpBison::Value& destroyOnBreak = def[ "DestroyObjectOnBreak" ];
	if( destroyOnBreak.isBool() )
	{
		m_destroyObjectOnBreak = destroyOnBreak.asBool();
	}

	const zpBison::Value& messageOnBreak = def[ "MessageOnBreak" ];
	if( messageOnBreak.isString() )
	{
		m_messageOnBreak = messageOnBreak.asCString();
	}

	const zpBison::Value& breakOnMessage = def[ "BreakOnMessage" ];
	if( breakOnMessage.isString() )
	{
		m_breakOnMessage = breakOnMessage.asCString();
	}

	const zpBison::Value& eventOnBreak = def[ "EventOnBreak" ];
	if( eventOnBreak.isString() )
	{
		m_eventOnBreak = eventOnBreak.asCString();
	}

	const zpBison::Value& breakOnEvent = def[ "BreakOnEvent" ];
	if( breakOnEvent.isString() )
	{
		m_breakOnEvent = breakOnEvent.asCString();
		getApplication()->getEventManager()->addEventListener( m_breakOnEvent, this, m_breakEventHandler );
	}

	const zpBison::Value& replacement = def[ "ReplacementObject" ];
	if( replacement.isString() )
	{
		m_replacementObject = replacement.asCString();
	}

	const zpBison::Value& breakInto = def[ "BreakIntoObjects" ];
	if( breakInto.isArray() && !breakInto.isEmpty() )
	{
		m_breakIntoObjects.reserve( breakInto.size() );
		breakInto.foreachArray( [ this ]( const zpBison::Value& obj )
		{
			m_breakIntoObjects.pushBackEmpty() = obj.asCString();
		} );
	}
}
zpBreakableComponent::~zpBreakableComponent()
{
}

zp_bool zpBreakableComponent::isBroken() const
{
	return m_health < BREAK_HEALTH;
}

zp_float zpBreakableComponent::getHealth() const
{
	return m_health;
}
zp_float zpBreakableComponent::getMaxHealth() const
{
	return m_maxHealth;
}

zp_int zpBreakableComponent::getHealthAsInt() const
{
	return zp_floor_to_int( m_health );
}
zp_int zpBreakableComponent::getMaxHealthAsInt() const
{
	return zp_floor_to_int( m_maxHealth );
}

void zpBreakableComponent::setHealth( zp_float health )
{
	zp_clamp( m_health, health, 0.0f, m_maxHealth );
}
void zpBreakableComponent::setMaxHeath( zp_float maxHealth, zpBreakableSetMaxHealthType scale )
{
	switch( scale )
	{
	case ZP_BREAKABLE_SET_MAX_HEALTH_NONE:
		{
			m_maxHealth = maxHealth;
			m_health = ZP_MIN( m_health, maxHealth );
		}
		break;
	case ZP_BREAKABLE_SET_MAX_HEALTH_SCALE:
		{
			zp_float percent = m_health / m_maxHealth;
			m_maxHealth = maxHealth;
			m_health = maxHealth * percent;
		}
		break;
	case ZP_BREAKABLE_SET_MAX_HEALTH_FILL:
		{
			m_maxHealth = maxHealth;
			m_health = maxHealth;
		}
		break;
	}
}

void zpBreakableComponent::gainHealth( zp_float change )
{
	if( change < 0.0f )
	{
		loseHealth( -change );
	}
	else if( change > 0.0f )
	{
		zp_int prevHealth = getHealthAsInt();
		zp_clamp( m_health, m_health + change, 0.0f, m_maxHealth );
		zp_int curHealth = getHealthAsInt();

		if( prevHealth != curHealth )
		{
			// health change
		}
	}
}
void zpBreakableComponent::loseHealth( zp_float change )
{
	if( change < 0.0f )
	{
		gainHealth( -change );
	}
	else if( change > 0.0f )
	{
		zp_int prevHealth = getHealthAsInt();
		zp_clamp( m_health, m_health - change, 0.0f, m_maxHealth );
		zp_int curHealth = getHealthAsInt();

		if( prevHealth != curHealth )
		{
			// health change
		}

		if( isBroken() )
		{
			m_justBroke = true;
		}
	}
}

void zpBreakableComponent::doBreak()
{
	const zpMatrix4f& transform = getParentObject()->getTransform();
	zpObjectContentManager* objectContent = getApplication()->getObjectContentManager();

	// replace object with same transform
	if( !m_replacementObject.isEmpty() )
	{
		zpObject* replacement = objectContent->createObject( getApplication(), m_replacementObject.str() );
		replacement->setTransform( transform );
	}

	// create objects at same position as broken object
	if( !m_breakIntoObjects.isEmpty() )
	{
		zpMatrix4f pos;
		pos.setIdentity();
		pos.setRow( 3, transform.getRow( 3 ) );

		zpObject* obj;
		zpString* b = m_breakIntoObjects.begin();
		zpString* e = m_breakIntoObjects.end();
		for( ; b != e; ++b )
		{
			obj = objectContent->createObject( getApplication(), b->str() );
			obj->setTransform( pos );
		}
	}

	// should send event when destroyed
	if( !m_eventOnBreak.isEmpty() )
	{
		getApplication()->getEventManager()->sendEvent( m_eventOnBreak, m_destroyObjectOnBreak ? ZP_NULL : getParentObject() );
	}

	// should send message to parent object when destroyed
	if( !m_messageOnBreak.isEmpty() )
	{

	}

	// should destroy on break
	if( m_destroyObjectOnBreak )
	{
		getParentObject()->destroy();
	}
	// otherwise, disable object
	else
	{
		getParentObject()->setEnabled( false );
	}
}

void zpBreakableComponent::handleEvent( const zpEvent& e, zpObject* sender )
{
	if( m_breakEventHandler.isHandlerForEvent( e ) )
	{
		m_justBroke = true;
		getApplication()->getEventManager()->removeEventListener( m_breakOnEvent, this, m_breakEventHandler );
	}
}

void zpBreakableComponent::onCreate()
{

}
void zpBreakableComponent::onInitialize()
{

}
void zpBreakableComponent::onDestroy()
{
	if( !m_breakOnEvent.isEmpty() )
	{
		getApplication()->getEventManager()->removeEventListener( m_breakOnEvent, this, m_breakEventHandler );
	}
}

void zpBreakableComponent::onUpdate()
{
	// if decay is set, lose health
	if( !zp_approximate( m_healthDecayPerSecond, 0.0f ) )
	{
		zp_float h = m_healthDecayPerSecond * zpTime::getInstance()->getDeltaSeconds();
		loseHealth( h );
	}

	// if the object just broke, perform break
	if( m_justBroke )
	{
		m_justBroke = false;
		doBreak();
	}
}
void zpBreakableComponent::onSimulate()
{
}

void zpBreakableComponent::onEnabled()
{
}
void zpBreakableComponent::onDisabled()
{
}


zpBreakableComponentPool::zpBreakableComponentPool()
{
}
zpBreakableComponentPool::~zpBreakableComponentPool()
{
}

void zpBreakableComponentPool::update()
{
	m_used.foreach( []( zpBreakableComponent* o )
	{
		o->update();
	} );
}
