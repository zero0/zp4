#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

enum zpProfilerSteps
{
	ZP_PROFILER_STEP_FRAME,
	ZP_PROFILER_STEP_UPDATE,
	ZP_PROFILER_STEP_SIMULATE,
	ZP_PROFILER_STEP_RENDER_PARTICLES,
	ZP_PROFILER_STEP_RENDER_MESHES,
	ZP_PROFILER_STEP_RENDER_SKYBOX,
	ZP_PROFILER_STEP_RENDER_UI,
	ZP_PROFILER_STEP_RENDER_FRAME,
	ZP_PROFILER_STEP_RENDER_BEGIN,
	ZP_PROFILER_STEP_RENDER,
	ZP_PROFILER_STEP_DEBUG_RENDER,
	ZP_PROFILER_STEP_RENDER_PRESENT,
	ZP_PROFILER_STEP_OBJECT_UPDATE,
	ZP_PROFILER_STEP_WORLD_UPDATE,
	ZP_PROFILER_STEP_PHYSICS_UPDATE,
	ZP_PROFILER_STEP_SCRIPT_UPDATE,
	ZP_PROFILER_STEP_SCRIPT_PROC_THREADS,

	ZP_PROFILER_STEP_DRAW_GUI,

	ZP_PROFILER_STEP_UPDATE_PHASE,
	ZP_PROFILER_STEP_UPDATE_STATE,

	ZP_PROFILER_STEP_INPUT_UPDATE,
	ZP_PROFILER_STEP_AUDIO_UPDATE,
	ZP_PROFILER_STEP_RENDERING_UPDATE,

	ZP_PROFILER_STEP_GARBAGE_COLLECT,
	ZP_PROFILER_STEP_RELOAD_ALL,
	ZP_PROFILER_STEP_HOT_RELOAD,

	ZP_PROFILER_STEP_SLEEP,

	zpProfilerSteps_Count,
};

enum
{
	ZP_PROFILER_MAX_FRAMES = 2 * 60
};

#if ZP_USE_PROFILER
#define ZP_PROFILE_START( step )	m_profiler.start( ZP_PROFILER_STEP_##step, m_timer.getTime() )
#define ZP_PROFILE_END( step )		m_profiler.end( ZP_PROFILER_STEP_##step, m_timer.getTime() )
#define ZP_PROFILE_FINALIZE()		m_profiler.finalize()
#else
#define ZP_PROFILE_START( step )	(void)0
#define ZP_PROFILE_END( step )		(void)0
#define ZP_PROFILE_FINALIZE()		(void)0
#endif

struct zpProfilerPart
{
	zp_long prevStartTime;
	zp_long prevEndTime;

	zp_long currentStartTime;
	zp_long currentEndTime;

	zp_long maxTime;
	zp_long minTime;
	zp_long averageTime;

	zp_long samples;
};

struct zpProfilerFrame
{
	zp_long startTime;
	zp_long endTime;
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

	void setup();
	void teardown();

	void start( zpProfilerSteps step, zp_long time );
	void end( zpProfilerSteps step, zp_long time );

	void reset();
	void finalize();

	zp_long getPreviousTime( zpProfilerSteps step );
	zp_long getAverageTime( zpProfilerSteps step );
	zp_long getMaxTime( zpProfilerSteps step );

	zp_float getPreviousTimeSeconds( zpProfilerSteps step, zp_float secondsPerTick );

	void printProfile( zpProfilerSteps step, zp_float secondsPerTick );

	const zpProfilerTimeline& getTimeline( zpProfilerSteps step ) const { return m_frames[ step ]; }

private:
	zp_int m_currentFrame;

	zpFixedArrayList< zpProfilerPart, zpProfilerSteps_Count > m_profiles;
	zpFixedArrayList< zpProfilerTimeline, zpProfilerSteps_Count > m_frames;
};

#endif