#include "zpCore.h"

zpProfiler::zpProfiler()
    : m_currentFrame( 0 )
    , m_previousFrame( 0 )
    , m_time( ZP_NULL )
    , m_mem( ZP_NULL )
{
}
zpProfiler::~zpProfiler()
{
    m_time = ZP_NULL;
    m_mem = ZP_NULL;
}

void zpProfiler::setup( zpTime* time, zpMemorySystem* mem )
{
    m_time = time;
    m_mem = mem;
    
    m_frames.resize( ZP_PROFILER_MAX_FRAMES );

    clear();
}
void zpProfiler::teardown()
{
    clear();

    m_time = ZP_NULL;
    m_mem = ZP_NULL;
}

zp_size_t zpProfiler::start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber )
{
    zp_long time = m_time->getTime();
    zp_long mem = m_mem->getMemoryUsed();
    
    zp_size_t index = m_frames[ m_currentFrame ].frames.size();

    zpProfilerFrame& frame =  m_frames[ m_currentFrame ].frames.pushBackEmpty();
    frame.fileName = fileName;
    frame.functionName = functionName;
    frame.eventName = "";
    frame.lineNumber = lineNumber;
    frame.startTime = time;
    frame.startMemory = mem;

    return index;
}
zp_size_t zpProfiler::start( const zp_char* fileName, const zp_char* functionName, const zp_char* eventName, zp_long lineNumber )
{
    zp_long time = m_time->getTime();
    zp_long mem = m_mem->getMemoryUsed();

    zp_size_t index = m_frames[ m_currentFrame ].frames.size();

    zpProfilerFrame& frame =  m_frames[ m_currentFrame ].frames.pushBackEmpty();
    frame.fileName = fileName;
    frame.functionName = functionName;
    frame.eventName = eventName;
    frame.lineNumber = lineNumber;
    frame.startTime = time;
    frame.startMemory = mem;

    return index;
}
void zpProfiler::end( zp_size_t index )
{
    zp_long time = m_time->getTime();
    zp_long mem = m_mem->getMemoryUsed();

    zpProfilerFrame& frame = m_frames[ m_currentFrame ].frames[ index ];
    frame.endTime = time;
    frame.endMemory = mem;
}

void zpProfiler::clear()
{
    for( zp_size_t i = 0, imax = m_frames.size(); i < imax; ++i )
    {
        m_frames[ i ].frames.clear();
    }
}
void zpProfiler::finalize()
{
    m_previousFrame = m_currentFrame;
    m_currentFrame = ( m_currentFrame + 1 ) % ZP_PROFILER_MAX_FRAMES;
    m_frames[ m_currentFrame ].frames.reset();
}
#if 0
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
#endif

const zpProfilerTimeline& zpProfiler::getPreviousFrameTimeline() const
{
    return m_frames[ m_previousFrame ];
}
