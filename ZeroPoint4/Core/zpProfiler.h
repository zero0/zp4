#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

enum zpProfilerSteps
{
	ZP_PROFILER_STEP_FRAME,
	ZP_PROFILER_STEP_UPDATE,
	ZP_PROFILER_STEP_SIMULATE,
	ZP_PROFILER_STEP_RENDER_PARTICLES,
	ZP_PROFILER_STEP_RENDER_MESHES,
	ZP_PROFILER_STEP_RENDER_FRAME,
	ZP_PROFILER_STEP_RENDER_BEGIN,
	ZP_PROFILER_STEP_RENDER,
	ZP_PROFILER_STEP_DEBUG_RENDER,
	ZP_PROFILER_STEP_RENDER_PRESENT,
	ZP_PROFILER_STEP_OBJECT_UPDATE,
	ZP_PROFILER_STEP_WORLD_UPDATE,
	ZP_PROFILER_STEP_SCRIPT_UPDATE,
	ZP_PROFILER_STEP_SCRIPT_PROC_THREADS,

	ZP_PROFILER_STEP_INPUT_UPDATE,
	ZP_PROFILER_STEP_AUDIO_UPDATE,

	ZP_PROFILER_STEP_GARBAGE_COLLECT,
	ZP_PROFILER_STEP_RELOAD_ALL,
	ZP_PROFILER_STEP_HOT_RELOAD,

	ZP_PROFILER_STEP_SLEEP,

	zpProfilerSteps_Count,
};

#if ZP_USE_PROFILER
#define ZP_PROFILE_START( step )	m_profiler.start( ZP_PROFILER_STEP_##step )
#define ZP_PROFILE_END( step )		m_profiler.end( ZP_PROFILER_STEP_##step )
#define ZP_PROFILE_FINALIZE()		m_profiler.finalize()
#else
#define ZP_PROFILE_START( step )	(void)0
#define ZP_PROFILE_END( step )		(void)0
#define ZP_PROFILE_FINALIZE()		(void)0
#endif

class zpProfiler
{
	ZP_NON_COPYABLE( zpProfiler );
public:
	zpProfiler();
	~zpProfiler();

	void start( zpProfilerSteps step );
	void end( zpProfilerSteps step );

	void reset();
	void finalize();

	zp_long getPreviousTime( zpProfilerSteps step );
	zp_long getAverageTime( zpProfilerSteps step );
	zp_long getMaxTime( zpProfilerSteps step );

	zp_float getPreviousTimeSeconds( zpProfilerSteps step );

	void printProfile( zpProfilerSteps step );

private:
	zpTime* m_time;

	struct zpProfilerPart
	{
		zp_long prevStartTime;
		zp_long prevEndTime;

		zp_long currentStartTime;
		zp_long currentEndTime;

		zp_long maxTime;
		zp_long averageTime;

		zp_long samples;
	};

	zpFixedArrayList< zpProfilerPart, zpProfilerSteps_Count > m_profiles;
};

#endif