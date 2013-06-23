#include "zpCore.h"

zpProfiler::zpProfiler()
	: m_time( zpTime::getInstance() )
{
	for( zp_uint i = 0; i < zpProfilerSteps_Count; ++i )
	{
		m_startTimes.pushBackEmpty();
		m_endTimes.pushBackEmpty();
	}
}
zpProfiler::~zpProfiler()
{
	reset();
}

zpProfiler zpProfiler::s_instance;
zpProfiler* zpProfiler::getInstance()
{
	return &s_instance;
}

void zpProfiler::start( zpProfilerSteps step )
{
	m_startTimes[ step ] = m_time->getTime();
	m_endTimes[ step ] = 0;
}
void zpProfiler::end( zpProfilerSteps step )
{
	m_endTimes[ step ] = m_time->getTime();
}

void zpProfiler::reset()
{
	for( zp_uint i = 0; i < zpProfilerSteps_Count; ++i )
	{
		m_startTimes[ i ] = 0;
		m_endTimes[ i ] = 0;
	}
}

zp_long zpProfiler::getTime( zpProfilerSteps step )
{
	zp_long s = m_startTimes[ step ];
	zp_long e = m_endTimes[ step ];
	return e - s;
}
