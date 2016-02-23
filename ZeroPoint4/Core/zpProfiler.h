#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

enum
{
	ZP_PROFILER_MAX_FRAMES = 2 * 60
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
	zp_long startTime;
	zp_long endTime;

	zp_size_t startMemory;
	zp_size_t endMemory;
};

struct zpProfilerTimeline
{
	zpProfilerFrame frames[ ZP_PROFILER_MAX_FRAMES ];
};

class zpProfiler
{
	ZP_NON_COPYABLE( zpProfiler );
public:
	zpProfiler();
	~zpProfiler();

	void setup( zp_size_t numSteps, zpTime* time, zpMemorySystem* mem );
	void teardown();

	void start( zp_size_t step );
	void end( zp_size_t step );

	void reset();
	void finalize();

	zp_long getPreviousTime( zp_size_t step ) const;
	zp_long getAverageTime( zp_size_t step ) const;
	zp_long getMaxTime( zp_size_t step ) const;

	zp_size_t getPreviousMemoryUsed( zp_size_t step ) const;

	zp_float getPreviousTimeSeconds( zp_size_t step, zp_float secondsPerTick ) const;

	void printProfile( zp_size_t step, zp_float secondsPerTick ) const;

	const zpProfilerTimeline& getTimeline( zp_size_t step ) const;

private:
	zp_size_t m_currentFrame;
	zp_size_t m_previousFrame;
	zp_size_t m_numSteps;

	zpTime* m_time;
	zpMemorySystem* m_mem;

	zpArrayList< zpProfilerPart > m_profiles;
	zpArrayList< zpProfilerTimeline > m_frames;
};

#endif