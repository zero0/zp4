#include "zpCommon.h"

#define BREAK_HEALTH    (1e-6f)

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
    m_health = zp_clamp( health, 0.0f, m_maxHealth );
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
        m_health = zp_clamp( m_health + change, 0.0f, m_maxHealth );
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
        m_health = zp_clamp( m_health - change, 0.0f, m_maxHealth );
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
    zpTransformComponent* t = getParentObject()->getComponents()->getTransformComponent();

    zpVector4f worldPos = t->getWorldPosition();
    zpObjectContentManager* objectContent = getApplication()->getObjectContentManager();

    // replace object with same transform
    if( !m_replacementObject.isEmpty() )
    {
        zpQuaternion4f worldRot = t->getLocalRotation();
        zpVector4f worldScale = t->getLocalScale();

        zpObject* replacement = objectContent->createObject( m_replacementObject.str() );
        zpTransformComponent* transform = replacement->getComponents()->getTransformComponent();
        transform->setLocalTransform( worldPos, worldRot, worldScale );
    }

    // create objects at same position as broken object
    if( !m_breakIntoObjects.isEmpty() )
    {
        zpObject* obj;
        zpString* b = m_breakIntoObjects.begin();
        zpString* e = m_breakIntoObjects.end();
        for( ; b != e; ++b )
        {
            obj = objectContent->createObject( b->str() );
            obj->getComponents()->getTransformComponent()->setLocalPosition( worldPos );
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

void zpBreakableComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    // if decay is set, lose health
    if( !zp_approximate( m_healthDecayPerSecond, 0.0f ) )
    {
        zp_float h = m_healthDecayPerSecond * deltaTime;
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

void zpBreakableComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpBreakableComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
