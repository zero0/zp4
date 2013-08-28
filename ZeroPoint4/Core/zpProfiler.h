#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

enum zpProfilerSteps
{
	ZP_PROFILER_STEP_FRAME,
	ZP_PROFILER_STEP_UPDATE,
	ZP_PROFILER_STEP_SIMULATE,
	ZP_PROFILER_STEP_RENDER_FRAME,
	ZP_PROFILER_STEP_RENDER_BEGIN,
	ZP_PROFILER_STEP_RENDER,
	ZP_PROFILER_STEP_DEBUG_RENDER,
	ZP_PROFILER_STEP_RENDER_PRESENT,
	ZP_PROFILER_STEP_OBJECT_UPDATE,
	ZP_PROFILER_STEP_INPUT_UPDATE,

	zpProfilerSteps_Count,
};

#if ZP_USE_PROFILER
#define ZP_PROFILE_START( step )	zpProfiler::getInstance()->start( ZP_PROFILER_STEP_##step )
#define ZP_PROFILE_END( step )		zpProfiler::getInstance()->end( ZP_PROFILER_STEP_##step )
#define ZP_PROFILE_FINALIZE()		zpProfiler::getInstance()->finalize()
#else
#define ZP_PROFILE_START( step )	(void)0
#define ZP_PROFILE_END( step )		(void)0
#define ZP_PROFILE_FINALIZE()		(void)0
#endif

class zpProfiler
{
	ZP_NON_COPYABLE( zpProfiler );
public:
	~zpProfiler();

	static zpProfiler* getInstance();

	void start( zpProfilerSteps step );
	void end( zpProfilerSteps step );

	void reset();
	void finalize();

	zp_long getPreviousTime( zpProfilerSteps step );
	zp_long getAverageTime( zpProfilerSteps step );
	zp_long getMaxTime( zpProfilerSteps step );

	void printProfile( zpProfilerSteps step );

private:
	zpProfiler();

	static zpProfiler s_instance;

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