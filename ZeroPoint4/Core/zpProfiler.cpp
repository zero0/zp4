#include "zpCore.h"

zpProfiler::zpProfiler()
	: m_time( zpTime::getInstance() )
{
	for( zp_uint i = 0; i < zpProfilerSteps_Count; ++i )
	{
		m_profiles.pushBackEmpty();
	}
	reset();
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
	m_profiles[ step ].currentStartTime = m_time->getTime();
}
void zpProfiler::end( zpProfilerSteps step )
{
	m_profiles[ step ].currentEndTime = m_time->getTime();
}

void zpProfiler::reset()
{
	for( zp_uint i = 0; i < zpProfilerSteps_Count; ++i )
	{
		zpProfilerPart& part = m_profiles[ i ];
		zp_zero_memory( &part );

		part.maxTime = zp_limit_min<zp_long>();
	}
}
void zpProfiler::finalize()
{
	for( zp_uint i = 0; i < zpProfilerSteps_Count; ++i )
	{
		zpProfilerPart& part = m_profiles[ i ];
		part.samples++;

		part.prevStartTime = part.currentStartTime;
		part.prevEndTime = part.currentEndTime;

		zp_long t = part.currentEndTime - part.currentStartTime;
		part.maxTime = ZP_MAX( t, part.maxTime );
		part.averageTime = ( part.averageTime + t ) / part.samples;

		part.currentStartTime = 0;
		part.currentEndTime = 0;
	}
}

zp_long zpProfiler::getPreviousTime( zpProfilerSteps step )
{
	zpProfilerPart& part = m_profiles[ step ];

	return part.prevEndTime - part.prevStartTime;
}
zp_long zpProfiler::getAverageTime( zpProfilerSteps step )
{
	return m_profiles[ step ].averageTime;
}
zp_long zpProfiler::getMaxTime( zpProfilerSteps step )
{
	return m_profiles[ step ].maxTime;
}

void zpProfiler::printProfile( zpProfilerSteps step )
{
	zpProfilerPart& part = m_profiles[ step ];

	zp_printfln( "step duration max_time avg_time\n%4d %8d %8d %8d", step, ( part.prevEndTime - part.prevStartTime ), part.maxTime, part.averageTime );
}
