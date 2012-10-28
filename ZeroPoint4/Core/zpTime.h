#pragma once
#ifndef ZP_TIME_H
#define ZP_TIME_H

class zpTime {
public:
	~zpTime();

	static zpTime* getInstance();

	zp_float getDeltaSeconds() const;
	zp_float getActualDeltaSeconds() const;
	zp_float getSecondsPerTick() const;

	void setTimeScale( zp_float timeScale );
	zp_float getTimeScale() const;
	

	void tick();
	void resetTimer();

	zp_long getTime() const;
	zp_long getDeltaTime() const;

	zp_float getInterpolation() const;
	void setInterpolation( zp_float interpolation );

	void sleep( zp_int milliseconds );

private:
	zpTime();
	
	static zpTime s_instance;

	zp_long m_baseTime;
	zp_long m_currentTime;
	zp_long m_previousTime;
	zp_long m_deltaTime;

	zp_float m_secondsPerTick;
	zp_float m_timeScale;
	zp_float m_deltaSeconds;
	zp_float m_actualDeltaSeconds;
	zp_float m_interpolation;
};

#endif