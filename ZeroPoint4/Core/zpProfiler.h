#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

enum zpProfilerSteps
{
	ZP_PROFILER_STEP_FRAME,
	ZP_PROFILER_STEP_UPDATE,
	ZP_PROFILER_STEP_SIMULATE,
	ZP_PROFILER_STEP_RENDER,

	zpProfilerSteps_Count,
};

#if ZP_USE_PROFILER
#define ZP_PROFILE_START( step )	zpProfiler::getInstance()->start( ZP_PROFILER_STEP_##step )
#define ZP_PROFILE_END( step )		zpProfiler::getInstance()->end( ZP_PROFILER_STEP_##step )
#else
#define ZP_PROFILE_START( step )	(void)0
#define ZP_PROFILE_END( step )		(void)0
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

	zp_long getTime( zpProfilerSteps step );

private:
	zpProfiler();

	static zpProfiler s_instance;

	zpTime* m_time;

	zpFixedArrayList< zp_long, zpProfilerSteps_Count > m_startTimes;
	zpFixedArrayList< zp_long, zpProfilerSteps_Count > m_endTimes;
};

#endif