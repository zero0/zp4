#pragma once
#ifndef ZP_APPLICATION_H
#define ZP_APPLICATION_H

enum zpApplicationPhaseResult
{
	ZP_APPLICATION_PHASE_NORMAL,
	ZP_APPLICATION_PHASE_COMPLETE,
	ZP_APPLICATION_PHASE_FAILURE,
};

ZP_PURE_INTERFACE zpApplicationPhase
{
public:
	virtual ~zpApplicationPhase() {}

	virtual const zp_char* getPhaseName() const = 0;

	virtual void onEnterPhase( zpApplication* app ) = 0;
	virtual void onLeavePhase( zpApplication* app ) = 0;

	virtual zpApplicationPhaseResult onUpdatePhase( zpApplication* app ) = 0;
};

class zpPhaseLoadWorld : public zpApplicationPhase
{
public:
	zpPhaseLoadWorld() {}
	~zpPhaseLoadWorld() {}

	const zp_char* getPhaseName() const { return "LoadWorld"; }

	void setup( const zp_char* loadingWorld, const zp_char* nextWorld )
	{
		m_loadingWorld = loadingWorld;
		m_nextWorld = nextWorld;
	}

	void onEnterPhase( zpApplication* app ) {}
	void onLeavePhase( zpApplication* app ) {}

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app ) { return ZP_APPLICATION_PHASE_NORMAL; }

private:
	zpString m_loadingWorld;
	zpString m_nextWorld;
};

class zpEmptyPhase : public zpApplicationPhase
{
public:
	zpEmptyPhase() {}
	~zpEmptyPhase() {}

	const zp_char* getPhaseName() const { return "Empty"; }

	void onEnterPhase( zpApplication* app ) {}
	void onLeavePhase( zpApplication* app ) {}

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app ) { return ZP_APPLICATION_PHASE_NORMAL; }
};

class zpPhysicsDebugDrawer : public zpIDebugPhysicsDebugDrawer
{
public:
	zpPhysicsDebugDrawer() : m_renderContext( ZP_NULL ) {}
	virtual ~zpPhysicsDebugDrawer() { m_renderContext = ZP_NULL; }

	void setRenderingContext( zpRenderingContext* renderContext ) { m_renderContext = renderContext; }

	void drawLine( const zpVector4f& from, const zpVector4f& to, const zpColor4f& fromColor, const zpColor4f& toColor )
	{
		m_renderContext->addLine( from, fromColor, to, toColor );
	}

private:
	zpRenderingContext* m_renderContext;
};

class zpApplication : public zpWindowProcListener, public zpWindowFocusListener, public zpWindowDragDropListener
{
public:
	zpApplication();
	virtual ~zpApplication();

	void setOptionsFilename( const zp_char* filename );
	const zpString& getOptionsFilename() const;

	void addPhase( zpApplicationPhase* phase );

	void popCurrentPhase();
	void updatePhase();
	void pushNextPhase();

	void initialize( const zpArrayList< zpString >& args );
	void setup();
	void run();
	void teardown();
	zp_int shutdown();

	void update();
	void simulate();

	void garbageCollect();
	void reloadAllResources();

	void exit( zp_int exitCode = 0 );

	const zpBison::Value& getOptions() const;

	void onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam );
	void onFocusGained();
	void onFocusLost();
	void onDragDrop( const zp_char* filename, zp_int x, zp_int y );

	zp_bool loadFile( const zp_char* filename );
	zp_bool handleDragAndDrop( const zp_char* filename, zp_int x, zp_int y );
	void handleInput();

	void loadWorld( const zp_char* worldFilename );
	void loadWorldAdditive( const zp_char* worldFilename );
	zp_float getLoadingWorldProgress() const;

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

	zpEventManager* getEventManager() { return &m_eventManager; }

	zpProtoDBManager* getProtoDBManager() { return &m_protoDBManager; }

	zpTags* getTags() { return &m_tags; }

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

	zp_int m_currentPhase;

	zp_int m_exitCode;
	zpString m_optionsFilename;
	zpString m_loadingWorldFilename;
	zpString m_initialWorldFilename;
	zpString m_nextWorldFilename;

	zpProfiler m_profiler;
	zpConsole* m_console;
	zpTime* m_timer;

	zpWorld* m_currentWorld;
	zpWorld* m_nextWorld;

	zp_long m_lastTime;
	zp_long m_simulateHz;
	zp_int m_renderMsHz;

	zpArrayList< zpApplicationPhase* > m_phases;

	zpWindow m_window;

	zpGUI m_gui;

	zpTags m_tags;

	zpMaterialResourceInstance m_defaultMaterial;

	zpTextResourceInstance m_appOptions;

	zpInputManager m_inputManager;
	zpRenderingPipeline m_renderingPipeline;
	
	zpObjectContentManager m_objectContent;
	zpWorldContentManager m_worldContent;
	zpScriptContentManager m_scriptContent;
	zpAudioContentManager m_audioContent;
	zpTextContentManager m_textContent;

	zpPhysicsEngine m_physicsEngine;

	zpEventManager m_eventManager;

	zpProtoDBManager m_protoDBManager;

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )	zp##cmp##ComponentPool m_componentPool##cmp;
#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

	enum zpApplicationStats
	{
		ZP_APPLICATION_STATS_FPS = 0,
		ZP_APPLICATION_STATS_FRAME_TIME,
		ZP_APPLICATION_STATS_DRAW_PHYSICS,
	};
	zpFlag32 m_displayStats;
	zp_uint m_statsTimer;

	zpPhysicsDebugDrawer m_debugPhysicsDrawer;
};

#endif