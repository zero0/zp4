#include "zpCore.h"

zpProfiler::zpProfiler()
{
}
zpProfiler::~zpProfiler()
{
}

void zpProfiler::setup()
{
	m_profiles.resize( zpProfilerSteps_Count );
	m_frames.resize( zpProfilerSteps_Count );

	reset();
}
void zpProfiler::teardown()
{
	reset();
}

void zpProfiler::start( zpProfilerSteps step, zp_long time )
{
	zpProfilerPart& part = m_profiles[ step ];
	part.currentStartTime = time;

	m_frames[ step ].frames[ m_currentFrame ].startTime = time;
}
void zpProfiler::end( zpProfilerSteps step, zp_long time )
{
	m_frames[ step ].frames[ m_currentFrame ].endTime = time;

	zpProfilerPart& part = m_profiles[ step ];
	part.currentEndTime = time;
	part.samples++;

	part.prevStartTime = part.currentStartTime;
	part.prevEndTime = part.currentEndTime;

	zp_long t = part.currentEndTime - part.currentStartTime;
	part.maxTime = ZP_MAX( t, part.maxTime );
	part.minTime = ZP_MIN( t, part.minTime );
	part.averageTime = ( part.averageTime + t ) / part.samples;

	part.currentStartTime = 0;
	part.currentEndTime = 0;
}

void zpProfiler::reset()
{
	for( zp_uint i = 0; i < zpProfilerSteps_Count; ++i )
	{
		zpProfilerPart& part = m_profiles[ i ];
		zp_zero_memory( &part );

		part.maxTime = zp_limit_min<zp_long>();
		part.minTime = zp_limit_max<zp_long>();

		zpProfilerTimeline& timeline = m_frames[ i ];
		zp_zero_memory_array( timeline.frames );
	}
}
void zpProfiler::finalize()
{
	m_currentFrame = ( m_currentFrame + 1 ) % ZP_PROFILER_MAX_FRAMES;
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

zp_float zpProfiler::getPreviousTimeSeconds( zpProfilerSteps step, zp_float secondsPerTick )
{
	return getPreviousTime( step ) * secondsPerTick;
}

void zpProfiler::printProfile( zpProfilerSteps step, zp_float secondsPerTick )
{
	zpProfilerPart& part = m_profiles[ step ];

	zp_float t = (zp_float)( part.prevEndTime - part.prevStartTime ) * secondsPerTick;
	zp_printfln( "   sec duration min_time max_time avg_time\n%1.4f %8d %8d %8d %8d", t, ( part.prevEndTime - part.prevStartTime ), part.minTime, part.maxTime, part.averageTime );
}
