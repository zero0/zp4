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

	zpPhysicsEngine* getPhysicsEngine() { return &m_physicsEngine; }

	zpTextContentManager* getTextContentManager() { return &m_textContent; }
	zpObjectContentManager* getObjectContentManager() { return &m_objectContent; }
	zpScriptContentManager* getScriptContentManager() { return &m_scriptContent; }
	zpAudioContentManager* getAudioContentManager() { return &m_audioContent; }

	zpInputManager* getInputManager() { return &m_inputManager; }

	zpGUI* getGUI() { return &m_gui; }

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) zp##cmp##ComponentPool* get##cmp##ComponentPool() { return &m_componentPool##cmp; }
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

private:
	void runGarbageCollect();
	void runReloadAllResources();

#if ZP_USE_HOT_RELOAD
	void runReloadChangedResources();
#endif

	void enterEditMode();
	void leaveEditMode();

	void guiEditMode();

	zp_bool m_isRunning;
	zp_bool m_hasNextWorld;
	zp_bool m_addNextWorld;
	zp_bool m_shouldGarbageCollect;
	zp_bool m_shouldReloadAllResources;
	zp_bool m_inEditMode;

	zp_int m_exitCode;
	zpString m_optionsFilename;
	zpString m_nextWorldFilename;

	zpProfiler m_profiler;
	zpConsole* m_console;
	zpTime* m_timer;
	zpWorld* m_currentWorld;

	zp_long m_lastTime;
	zp_long m_simulateHz;
	zp_int m_renderMsHz;

	zpWindow m_window;

	zpGUI m_gui;

	zpTextResourceInstance m_appOptions;

	zpInputManager m_inputManager;
	zpRenderingPipeline m_renderingPipeline;
	
	zpObjectContentManager m_objectContent;
	zpWorldContentManager m_worldContent;
	zpScriptContentManager m_scriptContent;
	zpAudioContentManager m_audioContent;

	zpPhysicsEngine m_physicsEngine;

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )	zp##cmp##ComponentPool m_componentPool##cmp;
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

	zpTextContentManager m_textContent;
};

#endif