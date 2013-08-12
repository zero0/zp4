#pragma once
#ifndef ZP_APPLICATION_H
#define ZP_APPLICATION_H

class zpApplication : public zpWindowProcListener, public zpWindowFocusListener
{
public:
	zpApplication();
	virtual ~zpApplication();

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

	virtual void onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam );
	virtual void onFocusGained();
	virtual void onFocusLost();

	zp_bool loadFile( const zp_char* filename );
	zp_bool handleDragAndDrop( const zp_char* filename, zp_int x, zp_int y );
	void handleInput();

	void processFrame();

	zpWindow* getWindow() { return &m_window; }
	zpRenderingPipeline* getRenderPipeline() { return &m_renderingPipeline; }

	zpTextContentManager* getTextContentManager() { return &m_textContent; }

private:
	zp_bool m_isRunning;
	zp_bool m_hasNextWorld;
	zp_int m_exitCode;
	zpString m_optionsFilename;
	zpString m_nextWorldFilename;

	zpConsole* m_console;
	zpTime* m_timer;
	zpWorld* m_currentWorld;

	zp_long m_lastTime;
	zp_long m_simulateHz;
	zp_int m_renderMsHz;

	zpWindow m_window;

	zpTextResourceInstance m_appOptions;

	zpInputManager m_inputManager;
	zpRenderingPipeline m_renderingPipeline;
	
	zpObjectPooledContent m_objectContent;
	zpWorldPooledContent m_worldContent;

	zpTextContentManager m_textContent;
};

#endif