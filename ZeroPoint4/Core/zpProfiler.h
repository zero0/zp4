#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

#if ZP_USE_PROFILER
#define ZP_PROFILE_SCOPE( profiler )                      zpScopedProfileStep( profiler, __FILE__, __FUNCTION__, __LINE__ )
#define ZP_PROFILE_SCOPE_EVENT( profiler, eventName )     zpScopedProfileStep( profiler, __FILE__, __FUNCTION__, eventName, __LINE__ )
#else
#define ZP_PROFILE_SCOPE( profiler )                      (void)0
#define ZP_PROFILE_SCOPE_EVENT( profiler, eventName )     (void)0
#endif

enum
{
    ZP_PROFILER_MAX_FRAMES = 2 * 60,
    ZP_PROFILER_MAX_STEPS = 128
};

struct zpProfilerPart
{
    zp_long prevStartTime;
    zp_long prevEndTime;

    zp_long currentStartTime;
    zp_long currentEndTime;

    zp_size_t prevStartMemory;
    zp_size_t prevEndMemory;

    zp_size_t currentStartMemory;
    zp_size_t currentEndMemory;

    zp_long maxTime;
    zp_long minTime;
    zp_long averageTime;

    zp_long samples;
};

struct zpProfilerFrame
{
    const zp_char* fileName;
    const zp_char* functionName;
    const zp_char* eventName;
    zp_long lineNumber;

    zp_long startTime;
    zp_long endTime;

    zp_size_t startMemory;
    zp_size_t endMemory;
};

struct zpProfilerTimeline
{
    zpFixedArrayList< zpProfilerFrame, ZP_PROFILER_MAX_STEPS > frames;
};

class zpProfiler
{
    ZP_NON_COPYABLE( zpProfiler );
public:
    zpProfiler();
    ~zpProfiler();

    void setup( zpTime* time, zpMemorySystem* mem );
    void teardown();

    zp_size_t start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber );
    zp_size_t start( const zp_char* fileName, const zp_char* functionName, const zp_char* eventName, zp_long lineNumber );
    void end( zp_size_t index );

    void clear();
    void finalize();
#if 0
    zp_long getPreviousTime( zp_size_t step ) const;
    zp_long getAverageTime( zp_size_t step ) const;
    zp_long getMaxTime( zp_size_t step ) const;

    zp_size_t getPreviousMemoryUsed( zp_size_t step ) const;

    zp_float getPreviousTimeSeconds( zp_size_t step, zp_float secondsPerTick ) const;

    void printProfile( zp_size_t step, zp_float secondsPerTick ) const;
#endif

    const zpProfilerTimeline& getPreviousFrameTimeline() const;

private:
    zp_size_t m_currentFrame;
    zp_size_t m_previousFrame;

    zpTime* m_time;
    zpMemorySystem* m_mem;

    zpFixedArrayList< zpProfilerTimeline, ZP_PROFILER_MAX_FRAMES > m_frames;
};

class zpScopedProfileStep
{
public:
    ZP_FORCE_INLINE zpScopedProfileStep( zpProfiler* profiler, const zp_char* fileName, const zp_char* functionName, zp_long lineNumber )
        : m_profiler( profiler )
        , m_step( profiler->start( fileName, functionName, lineNumber ) )
    {
    }
    ZP_FORCE_INLINE zpScopedProfileStep( zpProfiler* profiler, const zp_char* fileName, const zp_char* functionName, const zp_char* eventName, zp_long lineNumber )
        : m_profiler( profiler )
        , m_step( profiler->start( fileName, functionName, eventName, lineNumber ) )
    {
    }
    ZP_FORCE_INLINE ~zpScopedProfileStep()
    {
        m_profiler->end( m_step );
    }

private:
    zp_size_t m_step;
    zpProfiler* m_profiler;
};

#endif
