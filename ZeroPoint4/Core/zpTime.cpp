#include "zpCore.h"
#include <Windows.h>

zpTime::zpTime()
    : m_isPaused( false )
    , m_baseTime( 0 )
    , m_currentTime( 0 )
    , m_previousTime( 0 )
    , m_deltaTime( 0 )
    , m_timeSinceStart( 0 )
    , m_wallClockDeltaTime( 0 )
    , m_countsPerSecond( 0 )
    , m_secondsPerTick( 0.f )
    , m_timeScale( 1.f )
    , m_deltaSeconds( 0.f )
    , m_actualDeltaSeconds( 0.f )
    , m_interpolation( 0.f )
{
    QueryPerformanceFrequency( (LARGE_INTEGER*)&m_countsPerSecond );
    m_secondsPerTick = 1.f / (zp_float)m_countsPerSecond;

    resetTimer();
}
zpTime::~zpTime() {}

zp_float zpTime::getDeltaSeconds() const
{
    return m_deltaSeconds;
}
zp_float zpTime::getActualDeltaSeconds() const
{
    return m_actualDeltaSeconds;
}
zp_float zpTime::getSecondsPerTick() const
{
    return m_secondsPerTick;
}
zp_float zpTime::getSecondsSinceStart() const
{
    return m_timeSinceStart * m_secondsPerTick;
}
zp_float zpTime::getWallClockDeltaSeconds() const
{
    return m_wallClockDeltaTime * m_secondsPerTick;
}
zp_long zpTime::getCountsPerSecond() const
{
    return m_countsPerSecond;
}


void zpTime::setTimeScale( zp_float timeScale )
{
    m_timeScale = timeScale;
}
zp_float zpTime::getTimeScale() const
{
    return m_timeScale;
}

void zpTime::tick()
{
    QueryPerformanceCounter( (LARGE_INTEGER*)&m_currentTime );
    m_wallClockDeltaTime = m_currentTime - m_previousTime;

    if( m_isPaused )
    {
        m_deltaTime = 0;
    }
    else
    {
        m_deltaTime = m_wallClockDeltaTime;
        m_deltaTime = ZP_MAX( 0, m_deltaTime );
    }
    m_previousTime = m_currentTime;

    m_timeSinceStart += m_wallClockDeltaTime;
    m_actualDeltaSeconds = m_deltaTime * m_secondsPerTick;
    m_deltaSeconds = m_actualDeltaSeconds * m_timeScale;
}
void zpTime::resetTimer()
{
    zp_long time;
    QueryPerformanceCounter( (LARGE_INTEGER*)&time );
    m_baseTime = m_previousTime = time;
}

zp_long zpTime::getTime() const
{
    zp_long time;
    QueryPerformanceCounter( (LARGE_INTEGER*)&time );
    return time;
}
zp_long zpTime::getDeltaTime() const
{
    return m_deltaTime;
}
zp_long zpTime::getTimeSinceStart() const
{
    return m_timeSinceStart;
}
zp_long zpTime::getWallClockDeltaTime() const
{
    return m_wallClockDeltaTime;
}

zp_float zpTime::getInterpolation() const
{
    return m_interpolation;
}
void zpTime::setInterpolation( zp_float interpolation )
{
    m_interpolation = interpolation;
}
