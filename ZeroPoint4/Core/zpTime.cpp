#include "zpCore.h"
#include <Windows.h>

zpTime::zpTime() :
	m_baseTime( 0 ),
	m_currentTime( 0 ),
	m_previousTime( 0 ),
	m_deltaTime( 0 ),
	m_secondsPerTick( 0.f ),
	m_timeScale( 1.f ),
	m_deltaSeconds( 0.f ),
	m_actualDeltaSeconds( 0.f )
{
	zp_long countsPerSecond;
	QueryPerformanceFrequency( (LARGE_INTEGER*)&countsPerSecond );
	m_secondsPerTick = 1.f / countsPerSecond;

	resetTimer();
}
zpTime::~zpTime() {}

zpTime zpTime::s_instance;
zpTime* zpTime::getInstance() {
	return &s_instance;
}

zp_float zpTime::getDeltaSeconds() const {
	return m_deltaSeconds;
}
zp_float zpTime::getActualDeltaSeconds() const {
	return m_actualDeltaSeconds;
}

void zpTime::setTimeScale( zp_float timeScale ) {
	m_timeScale = timeScale;
}
zp_float zpTime::getTimeScale() const {
	return m_timeScale;
}

void zpTime::tick() {
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_currentTime );

	m_deltaTime = m_currentTime - m_previousTime;
	m_previousTime = m_currentTime;
	if( m_deltaTime < 0 ) {
		m_deltaTime = 0;
	}

	m_actualDeltaSeconds = m_deltaTime * m_secondsPerTick;
	m_deltaSeconds = m_actualDeltaSeconds * m_timeScale;
}
void zpTime::resetTimer() {
	zp_long time;
	QueryPerformanceCounter( (LARGE_INTEGER*)&time );
	m_baseTime = m_previousTime = time;
}

zp_long zpTime::getTime() const {
	zp_long time;
	QueryPerformanceCounter( (LARGE_INTEGER*)&time );
	return time;
}
zp_long zpTime::getDeltaTime() const {
	return m_deltaTime;
}