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

    virtual zpApplicationPhaseResult onUpdatePhase( zpApplication* app, zp_float deltaTime, zp_float realTime ) = 0;
};

ZP_PURE_INTERFACE zpApplicationState
{
public:
    virtual ~zpApplicationState() {}

    virtual const zp_char* getStateName() const = 0;

    virtual void onEnterState( zpApplication* app ) = 0;
    virtual void onLeaveState( zpApplication* app ) = 0;

    virtual void onUpdateState( zpApplication* app, zp_float deltaTime, zp_float realTime ) = 0;
};

class zpEmptyPhase : public zpApplicationPhase
{
public:
    zpEmptyPhase() {}
    ~zpEmptyPhase() {}

    const zp_char* getPhaseName() const { return "Empty"; }

    void onEnterPhase( zpApplication* app ) {}
    void onLeavePhase( zpApplication* app ) {}

    zpApplicationPhaseResult onUpdatePhase( zpApplication* app, zp_float deltaTime, zp_float realTime ) { return ZP_APPLICATION_PHASE_NORMAL; }
};

class zpEmptyState : public zpApplicationState
{
public:
    zpEmptyState() {}
    ~zpEmptyState() {}

    const zp_char* getStateName() const { return "Empty"; }

    void onEnterState( zpApplication* app ) {}
    void onLeaveState( zpApplication* app ) {}

    void onUpdateState( zpApplication* app, zp_float deltaTime, zp_float readTime ) {}
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

    void setConfigFilename( const zp_char* filename );
    const zpString& getConfigFilename() const;

    void setOptionsFilename( const zp_char* filename );
    const zpString& getOptionsFilename() const;

    void addPhase( zpApplicationPhase* phase );
    void addState( zpApplicationState* state );

    void popCurrentPhase();
    void updatePhase( zp_float deltaTime, zp_float realTime );
    void pushNextPhase();

    void swapState( const zp_char* stateName );
    void pushState( const zp_char* stateName );
    void popState();
    void updateState( zp_float deltaTime, zp_float realTime );
    zpApplicationState* getCurrentState() const;

    void processCommandLine( const zpArrayList< zpString >& args );
    
    void initialize();
    void setup();
    void run();
    void teardown();
    zp_int shutdown();

    void update();
    void simulate();

    void garbageCollect();
    void reloadAllResources();

    void exit( zp_int exitCode = 0 );
    void restart();

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
    zpAudioEngine* getAudioEngine() { return &m_audioEngine; }

    zpTextContentManager* getTextContentManager() { return &m_textContent; }

    zpPrefabContentManager* getPrefabContentManager() { return &m_prefabContent; }
    zpObjectContentManager* getObjectContentManager() { return &m_objectContent; }
    zpScriptContentManager* getScriptContentManager() { return &m_scriptContent; }
    zpAudioContentManager* getAudioContentManager() { return &m_audioContent; }

    zpInputManager* getInputManager() { return &m_inputManager; }

    zpGUI* getGUI() { return &m_gui; }

    zpEventManager* getEventManager() { return &m_eventManager; }

    zpProtoDBManager* getProtoDBManager() { return &m_protoDBManager; }

    zpTags* getTags() { return &m_tags; }

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            zp##cmp##ComponentPool* get##cmp##ComponentPool() { return &m_componentPool##cmp; }
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    zp_uint getFrameCount() const { return m_frameCount; }
    
    const zpTime& getTime() const { return m_timer; }
    zpTime& getTime() { return m_timer; }

    zp_bool isApplicationPaused() const { return m_isApplicationPaused; }
    void setApplicationPaused( zp_bool applicationPaused ) { m_isApplicationPaused = applicationPaused; }

private:

    void runGarbageCollect();
    void runReloadAllResources();

#if ZP_USE_HOT_RELOAD
    void runReloadChangedResources();
#endif

    void onGUI();

    zp_bool m_isRunning;
    zp_bool m_restartApplication;
    zp_bool m_shouldGarbageCollect;
    zp_bool m_shouldReloadAllResources;
    zp_bool m_isApplicationPaused;
    zp_bool m_isApplicationStepped;

    zp_int m_currentPhase;
    zp_int m_exitCode;

    zpString m_optionsFilename;
    zpString m_configFilename;
    zpString m_loadingWorldFilename;
    zpString m_initialWorldFilename;
    zpString m_nextWorldFilename;
    zpString m_editorStateName;

    zpProfiler m_profiler;
    zpConsole* m_console;
    zpTime m_timer;
    zpMemorySystem* m_memory;

    zpWorld* m_currentWorld;
    zpWorld* m_nextWorld;

    zp_long m_lastTime;
    zp_long m_simulateHz;
    zp_float m_totalFrameTimeMs;

    zpArrayList< zpApplicationPhase* > m_phases;

    zpArrayList< zpApplicationState* > m_allStates;
    zpArrayList< zpApplicationState* > m_stateStack;

    zpWindow m_window;

    zpGUI m_gui;

    zpTags m_tags;

    zpMaterialResourceInstance m_defaultMaterial;

    zpTextResourceInstance m_appConfig;
    zpTextResourceInstance m_appOptions;

    zpInputManager m_inputManager;
    zpRenderingPipeline m_renderingPipeline;
    
    zpPrefabContentManager m_prefabContent;
    zpObjectContentManager m_objectContent;
    zpWorldContentManager m_worldContent;
    zpScriptContentManager m_scriptContent;
    zpAudioContentManager m_audioContent;
    zpTextContentManager m_textContent;

    zpPhysicsEngine m_physicsEngine;
    zpAudioEngine m_audioEngine;

    zpEventManager m_eventManager;

    zpProtoDBManager m_protoDBManager;

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            zp##cmp##ComponentPool m_componentPool##cmp;
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    enum zpApplicationStats : zp_uint
    {
        ZP_APPLICATION_STATS_FPS = 0,
        ZP_APPLICATION_STATS_RENDERING,
        ZP_APPLICATION_STATS_DRAW_PHYSICS,
        ZP_APPLICATION_STATS_UPDATE,
        ZP_APPLICATION_STATS_RENDERING_CMDS,
        ZP_APPLICATION_STATS_DRAW_OCTREE,
        ZP_APPLICATION_STATS_DRAW_TIMELINE_FPS,
    };
    zpFlag32 m_displayStats;
    zp_uint m_statsTimer;

    zp_uint m_frameCount;

    zpPhysicsDebugDrawer m_debugPhysicsDrawer;
};

#endif