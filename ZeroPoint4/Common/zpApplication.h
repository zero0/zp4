#pragma once
#ifndef ZP_APPLICATION_H
#define ZP_APPLICATION_H

class zpApplication : public zpWindowProcListener, public zpWindowFocusListener, public zpWindowDragDropListener
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
	void reloadAllResources();

	void exit( zp_int exitCode = 0 );

	const zpBison::Value& getOptions() const;

	virtual void onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam );
	virtual void onFocusGained();
	virtual void onFocusLost();
	virtual void onDragDrop( const zp_char* filename, zp_int x, zp_int y );

	zp_bool loadFile( const zp_char* filename );
	zp_bool handleDragAndDrop( const zp_char* filename, zp_int x, zp_int y );
	void handleInput();

	void processFrame();

	zpWindow* getWindow() { return &m_window; }
	zpRenderingPipeline* getRenderPipeline() { return &m_renderingPipeline; }

	zpTextContentManager* getTextContentManager() { return &m_textContent; }
	zpObjectContentManager* getObjectContentManager() { return &m_objectContent; }
	zpScriptContentManager* getScriptContentManager() { return &m_scriptContent; }
	zpAudioContentManager* getAudioContentManager() { return &m_audioContent; }

	zpEditorCameraComponentPool* getEditorCameraComponentPool() { return &m_editorCameraContentPool; }
	zpScriptComponentPool* getScriptComponentPool() { return &m_scriptContentPool; }

private:
	void runGarbageCollect();
	void runReloadAllResources();

	zp_bool m_isRunning;
	zp_bool m_hasNextWorld;
	zp_bool m_shouldGarbageCollect;
	zp_bool m_shouldReloadAllResources;

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
	
	zpObjectContentManager m_objectContent;
	zpWorldPooledContent m_worldContent;
	zpScriptContentManager m_scriptContent;
	zpAudioContentManager m_audioContent;

	zpEditorCameraComponentPool m_editorCameraContentPool;
	zpScriptComponentPool m_scriptContentPool;

	zpTextContentManager m_textContent;
};

#endif