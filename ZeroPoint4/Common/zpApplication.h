#pragma once
#ifndef ZP_APPLICATION_H
#define ZP_APPLICATION_H

class zpApplication
{
public:
	zpApplication();
	~zpApplication();

	void setOptionsFilename( const zp_char* filename );
	const zpString& getOptionsFilename() const;

	void initialize( const zpArrayList< zpString >& args );
	void run();
	zp_int shutdown();

	void update();
	void simulate();

	void garbageCollect();

	void exit( zp_int exitCode = 0 );

	const zpBison::Value& getOptions() const;

private:
	void processFrame();

	zp_bool m_isRunning;
	zp_int m_exitCode;
	zpString m_optionsFilename;

	zpConsole* m_console;
	zpTime* m_timer;

	zp_long m_lastTime;
	zp_long m_simulateHz;
	zp_int m_renderMsHz;

	zpWindow m_window;

	zpBison m_appOptions;

	zpRenderingPipeline m_renderingPipeline;
	
	zpTextContentManager m_textContentManager;
};

#endif