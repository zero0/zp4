#include "zpCore.h"

zpProfiler::zpProfiler()
    : m_currentFrame( 0 )
    , m_previousFrame( 0 )
    , m_numSteps( 0 )
    , m_time( ZP_NULL )
    , m_mem( ZP_NULL )
{
}
zpProfiler::~zpProfiler()
{
}

void zpProfiler::setup( zp_size_t numSteps, zpTime* time, zpMemorySystem* mem )
{
    m_numSteps = numSteps;
    m_time = time;
    m_mem = mem;

    m_profiles.resize( numSteps );
    m_frames.resize( numSteps );

    reset();
}
void zpProfiler::teardown()
{
    reset();

    m_time = ZP_NULL;
    m_mem = ZP_NULL;
}

void zpProfiler::start( zp_size_t step )
{
    zp_long time = m_time->getTime();
    zp_long mem = m_mem->getMemoryUsed();

    zpProfilerPart& part = m_profiles[ step ];
    part.currentStartTime = time;
    part.currentStartMemory = mem;

    zpProfilerFrame& frame = m_frames[ step ].frames[ m_currentFrame ];
    frame.startTime = time;
    frame.startMemory = mem;
}
void zpProfiler::end( zp_size_t step )
{
    zp_long time = m_time->getTime();
    zp_long mem = m_mem->getMemoryUsed();

    zpProfilerFrame& frame = m_frames[ step ].frames[ m_currentFrame ];
    frame.endTime = time;
    frame.endMemory = mem;

    zpProfilerPart& part = m_profiles[ step ];
    part.currentEndTime = time;
    part.currentEndMemory = mem;
    part.samples++;

    part.prevStartTime = part.currentStartTime;
    part.prevEndTime = part.currentEndTime;

    part.prevStartMemory = part.currentStartMemory;
    part.prevEndMemory = part.currentEndMemory;

    zp_long t = part.currentEndTime - part.currentStartTime;
    part.maxTime = ZP_MAX( t, part.maxTime );
    part.minTime = ZP_MIN( t, part.minTime );
    part.averageTime = ( part.averageTime + t ) / part.samples;

    part.currentStartTime = 0;
    part.currentEndTime = 0;
}

void zpProfiler::reset()
{
    for( zp_uint i = 0; i < m_numSteps; ++i )
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
    m_previousFrame = m_currentFrame;
    m_currentFrame = ( m_currentFrame + 1 ) % ZP_PROFILER_MAX_FRAMES;
}

zp_long zpProfiler::getPreviousTime( zp_size_t step ) const
{
    const zpProfilerPart& part = m_profiles[ step ];

    return part.prevEndTime - part.prevStartTime;
}
zp_long zpProfiler::getAverageTime( zp_size_t step ) const
{
    return m_profiles[ step ].averageTime;
}
zp_long zpProfiler::getMaxTime( zp_size_t step ) const
{
    return m_profiles[ step ].maxTime;
}

zp_size_t zpProfiler::getPreviousMemoryUsed( zp_size_t step ) const
{
    const zpProfilerPart& part = m_profiles[ step ];
    return part.prevEndMemory;
}

zp_float zpProfiler::getPreviousTimeSeconds( zp_size_t step, zp_float secondsPerTick ) const
{
    return getPreviousTime( step ) * secondsPerTick;
}

void zpProfiler::printProfile( zp_size_t step, zp_float secondsPerTick ) const
{
    const zpProfilerPart& part = m_profiles[ step ];

    zp_float t = (zp_float)( part.prevEndTime - part.prevStartTime ) * secondsPerTick;
    zp_printfln( "   sec duration min_time max_time avg_time\n%1.4f %8d %8d %8d %8d", t, ( part.prevEndTime - part.prevStartTime ), part.minTime, part.maxTime, part.averageTime );
}

const zpProfilerTimeline& zpProfiler::getTimeline( zp_size_t step ) const
{
    return m_frames[ step ];
}
