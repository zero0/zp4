#include "zpCommon.h"

#define ZP_APPLICATION_DEFAULT_OPTIONS_FILE        "AppOptions.jsonb"
#define ZP_APPLICATION_DEFAULT_CONFIG_FILE        "AppConfig.jsonb"
#define ZP_APPLICATION_DEFAULT_WINDOW_WIDTH        640
#define ZP_APPLICATION_DEFAULT_WINDOW_HEIGHT    480

#if ZP_USE_PROFILER
#define ZP_PROFILE_START( eventName )   zp_size_t __index_##eventName = m_profiler.start( __FILE__, __FUNCTION__, #eventName, __LINE__ );
#define ZP_PROFILE_END( eventName )     m_profiler.end( __index_##eventName )
#define ZP_PROFILE_FINALIZE()           m_profiler.finalize()
#else
#define ZP_PROFILE_SCOPE( profiler )    (void)0
#define ZP_PROFILE_END( eventName )     (void)0
#define ZP_PROFILE_FINALIZE()           (void)0
#endif

zpApplication::zpApplication()
    : m_isRunning( false )
    , m_restartApplication( false )
    , m_shouldGarbageCollect( false )
    , m_shouldReloadAllResources( false )
    , m_isApplicationPaused( false )
    , m_isApplicationStepped( false )
    , m_exitCode( 0 )
    , m_optionsFilename( ZP_APPLICATION_DEFAULT_OPTIONS_FILE )
    , m_configFilename( ZP_APPLICATION_DEFAULT_CONFIG_FILE )
    , m_console( ZP_NULL )
    , m_timer()
    , m_memory( ZP_NULL )
    , m_currentWorld( ZP_NULL )
    , m_nextWorld( ZP_NULL )
    , m_lastTime( 0 )
    , m_simulateHz( 1000000 / 60 )
    , m_totalFrameTimeMs( 1000.f / 60.f )
    , m_statsTimer( 0 )
    , m_frameCount( 0 )
{}
zpApplication::~zpApplication()
{}

void zpApplication::setOptionsFilename( const zp_char* filename )
{
    m_optionsFilename = filename;
}
const zpString& zpApplication::getOptionsFilename() const
{
    return m_optionsFilename;
}

void zpApplication::setConfigFilename( const zp_char* filename )
{
    m_configFilename = filename;
}
const zpString& zpApplication::getConfigFilename() const
{
    return m_configFilename;
}

void zpApplication::processCommandLine( const zpArrayList< zpString >& args )
{
    onProcessCommandLine( args );
}

void zpApplication::initialize()
{
    m_textContent.setApplication( this );
    m_prefabContent.setApplication( this );
    m_objectContent.setApplication( this );
    m_worldContent.setApplication( this );
    m_scriptContent.setApplication( this );
    m_audioContent.setApplication( this );

    m_physicsEngine.create();

    m_renderingPipeline.setApplication( this );
    m_renderingPipeline.getMaterialContentManager()->setApplication( this );
    m_renderingPipeline.getShaderContentManager()->setApplication( this );
    m_renderingPipeline.getTextureContentManager()->setApplication( this );
    m_renderingPipeline.getMeshContentManager()->setApplication( this );
    m_renderingPipeline.getFontContentManager()->setApplication( this );
    
    m_memory = zpMemorySystem::getInstance();
    m_objectContent.setup( m_memory );
    m_worldContent.setup( m_memory );

    zp_bool ok;
    ok = m_textContent.getResource( m_configFilename, m_appConfig );
    ZP_ASSERT( ok, "Failed to load app config '%s'", m_configFilename.str() );

    const zpBison::Value& appConfig = m_appConfig.getResource()->getData()->root();

    const zpBison::Value& console = appConfig[ "Console" ];
    if( console.asBool() )
    {
        m_console = zpConsole::getInstance();
        m_console->create();
    }

    const zpBison::Value& window = appConfig[ "Window" ];
    ZP_ASSERT( window.isObject(), "" );

    zpVector2i pos( window[ "X" ].asInt(), window[ "Y" ].asInt() );
    zpVector2i size( window[ "Width" ].asInt(), window[ "Height" ].asInt() );

    m_window.setPosition( pos );
    m_window.setScreenSize( size );
    m_window.setTitle( window[ "Title" ].asCString() );
    m_window.addProcListener( this );
    m_window.addFocusListener( this );
    m_window.addDragDropListener( this );

    m_window.create();

    const zpBison::Value& render = appConfig[ "Render" ];
    ZP_ASSERT( render.isObject(), "" );

    zpDisplayMode displayMode;
    displayMode.width = size.getX();
    displayMode.height = size.getY();
    displayMode.refreshRate = render[ "Refresh" ].asInt();
    displayMode.displayFormat = ZP_DISPLAY_FORMAT_RGBA8_UNORM;

    m_renderingPipeline.initialize();

    zpRenderingEngine* re = m_renderingPipeline.getRenderingEngine();
    re->setScreenMode( ZP_SCREEN_MODE_WINDOWED );
    re->setDisplayMode( displayMode );
    re->setVSyncEnabled( render[ "VSync" ].asBool() );
    re->setup( m_window.getWindowHandle(), m_window.getScreenSize() );

    m_gui.setApplication( this );
    m_gui.create();

    m_audioEngine.create( m_window.getWindowHandle() );

    zpAngelScript::createInstance();
    zpAngelScript::getInstance()->createEngine( this );

    // register input with window
    m_window.addFocusListener( &m_inputManager );
    m_window.addProcListener( &m_inputManager );

    onInitialize();
}
void zpApplication::setup()
{
    m_profiler.setup( &m_timer, m_memory );

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )            m_componentPool##cmp.setup( m_memory );
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    m_renderingPipeline.setup();
    m_gui.setup();
    m_eventManager.setup();

    zp_bool ok;
    ok = m_textContent.getResource( m_optionsFilename, m_appOptions );
    ZP_ASSERT( ok, "Failed to load app options '%s'", m_optionsFilename.str() );

    const zpBison::Value& appOptions = m_appOptions.getResource()->getData()->root();

    const zpBison::Value& world = appOptions[ "InitialWorld" ];
    if( world.isString() )
    {
        m_initialWorldFilename = world.asCString();

        loadWorld( world.asCString() );
    }

    const zpBison::Value& loadingWorld = appOptions[ "LoadingWorld" ];
    if( loadingWorld.isString() )
    {
        m_loadingWorldFilename = loadingWorld.asCString();
    }

    m_protoDBManager.setProtoDBFile( appOptions[ "ProtoDB" ].asCString() );

    m_renderingPipeline.getMaterialContentManager()->getResource( appOptions[ "DefaultMaterial" ].asCString(), m_defaultMaterial );

    onSetup();

    m_lastTime = m_timer.getTime();
}

void zpApplication::run()
{
    do
    {
        // rest loop booleans
        m_restartApplication = false;
        m_isRunning = true;

        // setup app
        setup();

        // loop while app is running and window is processing messages
        while( m_isRunning && m_window.processMessages() )
        {
            processFrame();
        }

        // teardown app
        teardown();

        // if the app should restart, restart the loop
    } while ( m_restartApplication );
}

void zpApplication::teardown()
{
    onTeardown();

    m_appOptions.release();
    m_appConfig.release();

    m_defaultMaterial.release();
    m_gui.destroy();

    m_objectContent.destroyAllObjects();
    m_objectContent.update();

    m_worldContent.destroyAllWorlds();
    m_worldContent.update();

    m_renderingPipeline.teardown();
    m_gui.teardown();
    m_eventManager.teardown();

#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp )        m_componentPool##cmp.teardown();
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    garbageCollect();

    m_profiler.teardown();
}
zp_int zpApplication::shutdown()
{
    onShutdown();

    runGarbageCollect();

    m_objectContent.teardown();
    m_worldContent.teardown();

    m_renderingPipeline.shutdown();
    m_physicsEngine.destroy();

    zpAngelScript::getInstance()->destroyEngine();
    zpAngelScript::destroyInstance();

    m_protoDBManager.destroy();

    m_audioEngine.destroy();

    if( m_console )
    {
        m_console->destroy();
        m_console = ZP_NULL;
    }

    m_window.removeFocusListener( &m_inputManager );
    m_window.removeProcListener( &m_inputManager );

    m_window.destroy();

    return m_exitCode;
}

void zpApplication::update()
{
    // get times
    zp_float deltaTime = m_isApplicationPaused && !m_isApplicationStepped ? 0.f : m_timer.getDeltaSeconds();
    zp_float realTime = m_timer.getWallClockDeltaSeconds();
    
    ZP_PROFILE_SCOPE( &m_profiler );

    // clear application step
    m_isApplicationStepped = false;

    if( !m_nextWorldFilename.isEmpty() )
    {
        // destroy old world and objects
        if( m_currentWorld != ZP_NULL )
        {
            m_currentWorld->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
            m_objectContent.destroyAllObjectsInWorld( m_currentWorld );
        }

        // load the loading world and create it's objects
        m_currentWorld = m_worldContent.createWorld( m_loadingWorldFilename.str(), false );

        // step load the next world
        m_nextWorld = m_worldContent.createWorld( m_nextWorldFilename.str(), false );
        m_nextWorld->setFlag( ZP_WORLD_FLAG_STEP_CREATE );

        // reset state
        m_nextWorldFilename.clear();
    }

    // if the next world is done loading
    if( m_nextWorld != ZP_NULL && m_nextWorld->isFlagSet( ZP_WORLD_FLAG_CREATED ) )
    {
        // destroy the loading world and it's objects
        m_currentWorld->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
        m_objectContent.destroyAllObjectsInWorld( m_currentWorld );

        // current world is the world that was loaded
        m_currentWorld = m_nextWorld;
        m_nextWorld = ZP_NULL;

        // collect garbage next frame
        garbageCollect();
    }

    ZP_PROFILE_START( APP_UPDATE );
    onUpdate( deltaTime, realTime );
    ZP_PROFILE_END( APP_UPDATE );

    // pump queued events
    ZP_PROFILE_START( EVENT_UPDATE );
    m_eventManager.update();
    ZP_PROFILE_END( EVENT_UPDATE );

    // update world, delete, create objects, etc.
    ZP_PROFILE_START( WORLD_UPDATE );
    m_worldContent.update();
    ZP_PROFILE_END( WORLD_UPDATE );

    // update object, delete any etc
    ZP_PROFILE_START( OBJECT_UPDATE );
    m_objectContent.update();
    ZP_PROFILE_END( OBJECT_UPDATE );

    // update physics
    ZP_PROFILE_START( PHYSICS_UPDATE );
    m_physicsEngine.update( deltaTime );
    ZP_PROFILE_END( PHYSICS_UPDATE );

    // update input
    ZP_PROFILE_START( INPUT_UPDATE );
    m_inputManager.update();
    ZP_PROFILE_END( INPUT_UPDATE );

    // update all components
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) m_componentPool##cmp.update( deltaTime, realTime );
    #include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

    ZP_PROFILE_START( RENDERING_UPDATE );
    m_renderingPipeline.update( deltaTime, realTime );
    ZP_PROFILE_END( RENDERING_UPDATE );

    ZP_PROFILE_START( AUDIO_UPDATE );
    m_audioEngine.update();
    ZP_PROFILE_END( AUDIO_UPDATE );

    ZP_PROFILE_START( APP_LATE_UPDATE );
    onLateUpdate( deltaTime, realTime );
    ZP_PROFILE_END( APP_LATE_UPDATE );
}
void zpApplication::fixedUpdate()
{
    //m_componentPoolEditorCamera.simulate();
    m_physicsEngine.simulate();

    m_componentPoolRigidBody.simulate();

    onFixedUpdate();
}

void zpApplication::garbageCollect()
{
    m_shouldGarbageCollect = true;
}
void zpApplication::reloadAllResources()
{
    m_shouldReloadAllResources = true;
}

void zpApplication::exit( zp_int exitCode )
{
    m_isRunning = false;
    m_exitCode = exitCode;
}
void zpApplication::restart()
{
    m_isRunning = false;
    m_restartApplication = true;
}

const zpBison::Value& zpApplication::getOptions() const
{
    return m_appOptions.getResource()->getData()->root();
}

void zpApplication::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{
    m_inputManager.onWindowProc( uMessage, wParam, lParam );
}
void zpApplication::onFocusGained()
{
    m_inputManager.onFocusGained();
    m_renderingPipeline.onFocusGained();

    onFocusChanged( true );
}
void zpApplication::onFocusLost()
{
    m_inputManager.onFocusLost();
    m_renderingPipeline.onFocusLost();

    onFocusChanged( false );
}
void zpApplication::onDragDrop( const zp_char* filename, zp_int x, zp_int y )
{
    handleDragAndDrop( filename, x, y );
}

zp_bool zpApplication::loadFile( const zp_char* filename )
{
    return handleDragAndDrop( filename, 0, 0 );
}
zp_bool zpApplication::handleDragAndDrop( const zp_char* filename, zp_int x, zp_int y )
{
    zpString strFilename( filename );
    zp_bool loaded = false;

    if( strFilename.endsWith( ".jsonb" ) )
    {
        loaded = m_textContent.reloadResource( filename );
    }
    else if( strFilename.endsWith( ".objectb" ) )
    {
        zpObject* o = m_objectContent.createObject( filename );
        o->setWorld( m_currentWorld );
        loaded = o != ZP_NULL;
    }
    else if( strFilename.endsWith( ".worldb" ) )
    {
        loadWorld( strFilename.str() );
    }
    else if( strFilename.endsWith( ".shaderb" ) )
    {
        loaded = m_renderingPipeline.getShaderContentManager()->reloadResource( filename );
    }
    else if( strFilename.endsWith( ".textureb" ) )
    {
        loaded = m_renderingPipeline.getTextureContentManager()->reloadResource( filename );
    }

    return loaded;
}
void zpApplication::handleInput()
{
    const zpKeyboard* keyboard = m_inputManager.getKeyboard();

    if( keyboard->isKeyPressed( ZP_KEY_CODE_ESC ) )
    {
        exit( 0 );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_RIGHT ) )
    {
        m_isApplicationStepped = true;
    }
    else if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) && keyboard->isKeyPressed( ZP_KEY_CODE_G ) )
    {
        garbageCollect();
    }
    else if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) && keyboard->isKeyPressed( ZP_KEY_CODE_R ) )
    {
        reloadAllResources();
    }
    else if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) && keyboard->isKeyPressed( ZP_KEY_CODE_1 ) )
    {
        loadWorld( "worlds/physics.worldb" );
    }
    else if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) && keyboard->isKeyPressed( ZP_KEY_CODE_2 ) )
    {
        loadWorld( "worlds/test.worldb" );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F1 ) )
    {
        zp_bool marked = m_displayStats.toggle( ZP_APPLICATION_STATS_FPS );
        if( marked )
        {
            //m_profiler.printProfile( ZP_PROFILER_STEP_PHYSICS_UPDATE, m_timer.getSecondsPerTick() );
        }
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F2 ) )
    {
        m_displayStats.toggle( ZP_APPLICATION_STATS_RENDERING );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F3 ) )
    {
        m_displayStats.toggle( ZP_APPLICATION_STATS_UPDATE );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F4 ) )
    {
        m_displayStats.toggle( ZP_APPLICATION_STATS_RENDERING_CMDS );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F5 ) )
    {
        zp_bool wasSet = m_displayStats.isMarked( ZP_APPLICATION_STATS_DRAW_PHYSICS );
        zp_bool isSet = m_displayStats.toggle( ZP_APPLICATION_STATS_DRAW_PHYSICS );

        if( !wasSet && isSet )
        {
            m_debugPhysicsDrawer.setRenderingContext( getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext() );
            m_physicsEngine.setDebugDrawer( &m_debugPhysicsDrawer );
        }
        else if( wasSet && !isSet )
        {
            m_physicsEngine.setDebugDrawer( ZP_NULL );
        }
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F6 ) )
    {
        m_displayStats.toggle( ZP_APPLICATION_STATS_DRAW_OCTREE );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F8 ) )
    {
        m_memory->takeMemorySnapshot( m_timer.getTime(), ZP_MEMORY_KB( 3.0f ) );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F9 ) )
    {
        restart();
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_F11 ) )
    {
        m_renderingPipeline.takeScreenshot( keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) ? ZP_SCREENSHOT_TYPE_NO_UI : ZP_SCREENSHOT_TYPE_ALL, ".", m_timer.getTime() );
    }
    else if( keyboard->isKeyPressed( ZP_KEY_CODE_C ) )
    {
        zp_float x = zpRandom::getInstance()->randomFloat( -5, 5 );
        zp_float z = zpRandom::getInstance()->randomFloat( -5, 5 );
        zpObject* obj = getObjectContentManager()->createObject( "objects/cube.objectb" );
        obj->getComponents()->getTransformComponent()->setLocalPosition( zpMath::Vector4( x, 10, z, 1 ) );
    }

    // draw physics debug
    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_DRAW_PHYSICS ) )
    {
        zpRenderingContext* context = m_renderingPipeline.getRenderingEngine()->getImmediateRenderingContext();
        context->beginDrawImmediate( ZP_RENDERING_LAYER_DEFAULT, ZP_RENDERING_QUEUE_OPAQUE, ZP_TOPOLOGY_LINE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_defaultMaterial );
        m_physicsEngine.debugDraw();
        context->endDrawImmediate();
    }

    // draw octree
    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_DRAW_OCTREE ) )
    {
        zpRenderingContext* context = m_renderingPipeline.getRenderingEngine()->getImmediateRenderingContext();
        context->beginDrawImmediate( ZP_RENDERING_LAYER_DEFAULT, ZP_RENDERING_QUEUE_OPAQUE, ZP_TOPOLOGY_LINE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_defaultMaterial );
        
        zpTransformOctree* tree = m_componentPoolTransform.getTree();
        tree->foreachNode( [ context ]( zpTransformOctreeNode* node ) {
            const zpBoundingAABB& aabb = node->getBounts();
            zpVector4f edgeCoord = zpMath::Vector4( 1, 1, 1, 0 );
            zpVector4f pa, pb;
            zpVector4f center = aabb.getCenter();
            zpVector4f halfExt = aabb.getExtents();
            
            zp_int i, j;

            ZP_ALIGN16 zp_float e[4];

            for( i = 0; i < 4; ++i )
            {
                for( j = 0; j < 3; ++j )
                {
                    pa = zpMath::Vector4Mul( edgeCoord, halfExt );
                    pa = zpMath::Vector4Add( pa, center );

                    zp_int otherCoord = j % 3;
                    zpMath::Vector4Store4( edgeCoord, e );
                    e[ otherCoord ] *= -1.f;
                    edgeCoord = zpMath::Vector4Load4( e );

                    pb = zpMath::Vector4Mul( edgeCoord, halfExt );
                    pb = zpMath::Vector4Add( pb, center );

                    context->addLine( pa, pb, zpColor4f( j/3.f, i/4.f, 0.f, 1.f ) );
                }

                edgeCoord = zpMath::Vector4( -1, -1, -1, 0 );
                if( i < 3 )
                {
                    zpMath::Vector4Store4( edgeCoord, e );
                    e[ i ] *= -1.f;
                    edgeCoord = zpMath::Vector4Load4( e );
                }
            }
        } );

        context->endDrawImmediate();
    }

    onHandleInput();
}

void zpApplication::loadWorld( const zp_char* worldFilename )
{
    m_nextWorldFilename = worldFilename;
}
void zpApplication::loadWorldAdditive( const zp_char* worldFilename )
{
    m_nextWorldFilename = worldFilename;
}
zp_float zpApplication::getLoadingWorldProgress() const
{
    return m_nextWorld == ZP_NULL ? 1.f : m_nextWorld->getLoadingProgress();
}

void zpApplication::processFrame()
{
    m_timer.tick();
    zp_long startTime = m_timer.getTime();

    ZP_PROFILE_START( PROCESS_FRAME );

    ZP_PROFILE_START( FRAME );

    if( m_shouldGarbageCollect )
    {
        m_shouldGarbageCollect = false;

        ZP_PROFILE_START( GARBAGE_COLLECT );
        runGarbageCollect();
        ZP_PROFILE_END( GARBAGE_COLLECT );
    }

    if( m_shouldReloadAllResources )
    {
        m_shouldReloadAllResources = false;

        ZP_PROFILE_START( RELOAD_ALL );
        runReloadAllResources();
        ZP_PROFILE_END( RELOAD_ALL );
    }

#if ZP_USE_HOT_RELOAD
    ZP_PROFILE_START( HOT_RELOAD );
    runReloadChangedResources();
    ZP_PROFILE_END( HOT_RELOAD );
#endif

    // handle input
    ZP_PROFILE_START( HANDLE_INPUT );
    handleInput();
    ZP_PROFILE_END( HANDLE_INPUT );

    // update
    ZP_PROFILE_START( UPDATE );
    update();
    ZP_PROFILE_END( UPDATE );

    // fixed update
    ZP_PROFILE_START( FIXED_UPDATE );
    zp_int numUpdates = 0;
    while( ( startTime - m_lastTime ) > m_simulateHz && numUpdates < 5 )
    {
        fixedUpdate();
        m_lastTime += m_simulateHz;
        ++numUpdates;
    }
    ZP_PROFILE_END( FIXED_UPDATE );

    // adjust timer
    if( ( startTime - m_lastTime ) > m_simulateHz )
    {
        m_lastTime = startTime - m_simulateHz;
    }

    m_timer.setInterpolation( (zp_float)( startTime - m_lastTime ) / (zp_float)m_simulateHz );

    ZP_PROFILE_START( DRAW_GUI );
    if( !m_displayStats.isZero() )
    {
        onGUI();
    }
    ZP_PROFILE_END( DRAW_GUI );

    ZP_PROFILE_START( RENDER_FRAME );
    {
        zpRenderingContext* i = m_renderingPipeline.getRenderingEngine()->getImmediateRenderingContext();

        // render sky box
        ZP_PROFILE_START( RENDER_SKYBOX );
        m_componentPoolSkyBox.render( i );
        ZP_PROFILE_END( RENDER_SKYBOX );

        // individual component render
        ZP_PROFILE_START( RENDER_MESHES );
        m_componentPoolMeshRenderer.render( i );
        ZP_PROFILE_END( RENDER_MESHES );

        // animated mesh render
        ZP_PROFILE_START( RENDER_ANIMATED_MESHES );
        m_componentPoolAnimatedMeshRenderer.render( i );
        ZP_PROFILE_END( RENDER_ANIMATED_MESHES );

        // render UI
        ZP_PROFILE_START( RENDER_UI );
        m_componentPoolUICanvas.render( i );
        ZP_PROFILE_END( RENDER_UI );

        // render particles for each camera
        ZP_PROFILE_START( RENDER_PARTICLES );
        m_componentPoolParticleEmitter.render( i, ZP_NULL );
        //for( zp_uint p = 0; p < zpCameraType_Count; ++p )
        //{
        //    const zpArrayList< zpCamera* >& cameras = m_renderingPipeline.getUsedCameras( (zpCameraType)p );
        //    cameras.foreach( [ i, this ]( const zpCamera* camera ) 
        //    {
        //        if( camera->isActive() ) 
        //    } );
        //}
        ZP_PROFILE_END( RENDER_PARTICLES );
        
        // render begin
        ZP_PROFILE_START( RENDER_BEGIN );
        m_renderingPipeline.beginFrame( i, &m_timer );
        ZP_PROFILE_END( RENDER_BEGIN );

        // render commands
        ZP_PROFILE_START( RENDER );
        m_renderingPipeline.submitRendering( i );
        ZP_PROFILE_END( RENDER );

        // present
        ZP_PROFILE_START( RENDER_PRESENT );
        m_renderingPipeline.endFrame( i );
        m_renderingPipeline.present();
        ZP_PROFILE_END( RENDER_PRESENT );
    }
    ZP_PROFILE_END( RENDER_FRAME );

    ZP_PROFILE_END( FRAME );

    // increment frame counter
    ++m_frameCount;

    ZP_PROFILE_START( SLEEP );
    // sleep for the remainder of the frame
    zp_long endTime = m_timer.getTime();

    zp_long diff = ( endTime - startTime ) * 1000L;
    zp_float d = m_timer.getSecondsPerTick() * (zp_float)diff;
    zp_float sleepTime = m_totalFrameTimeMs - d;
    while( sleepTime < 0.f )
    {
        sleepTime += m_totalFrameTimeMs;
    }

    zp_sleep( (zp_uint)sleepTime );
    ZP_PROFILE_END( SLEEP );

    ZP_PROFILE_END( PROCESS_FRAME );

    ZP_PROFILE_FINALIZE();
}

void zpApplication::runGarbageCollect()
{
    m_textContent.garbageCollect();
    m_prefabContent.garbageCollect();
    m_objectContent.garbageCollect();
    m_worldContent.garbageCollect();
    m_scriptContent.garbageCollect();
    m_audioContent.garbageCollect();

    m_renderingPipeline.getFontContentManager()->garbageCollect();
    m_renderingPipeline.getMeshContentManager()->garbageCollect();
    m_renderingPipeline.getMaterialContentManager()->garbageCollect();
    m_renderingPipeline.getShaderContentManager()->garbageCollect();
    m_renderingPipeline.getTextureContentManager()->garbageCollect();

    onGarbageCollect();
}
void zpApplication::runReloadAllResources()
{
    m_textContent.reloadAllResources();
    m_prefabContent.reloadAllResources();
    m_objectContent.reloadAllResources();
    m_worldContent.reloadAllResources();
    m_scriptContent.reloadAllResources();
    m_audioContent.reloadAllResources();

    m_renderingPipeline.getFontContentManager()->reloadAllResources();
    m_renderingPipeline.getMeshContentManager()->reloadAllResources();
    m_renderingPipeline.getMaterialContentManager()->reloadAllResources();
    m_renderingPipeline.getShaderContentManager()->reloadAllResources();
    m_renderingPipeline.getTextureContentManager()->reloadAllResources();

    onReloadAllResources();
}

#if ZP_USE_HOT_RELOAD
void zpApplication::runReloadChangedResources()
{
    m_textContent.reloadChangedResources();
    m_prefabContent.reloadChangedResources();
    m_objectContent.reloadChangedResources();
    m_worldContent.reloadChangedResources();
    m_scriptContent.reloadChangedResources();
    m_audioContent.reloadChangedResources();

    m_renderingPipeline.getMeshContentManager()->reloadChangedResources();
    m_renderingPipeline.getMaterialContentManager()->reloadChangedResources();
    m_renderingPipeline.getShaderContentManager()->reloadChangedResources();
    m_renderingPipeline.getTextureContentManager()->reloadChangedResources();

    onReloadChangedResources();
}
#endif

void zpApplication::onGUI()
{
    m_gui.startGUI();

    zpFixedStringBuffer< 256 > buff;

    zp_float secondsPerTick = m_timer.getSecondsPerTick();

    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_FPS ) )
    {
        const zpProfilerTimeline& timeline = m_profiler.getPreviousFrameTimeline();

        zpRectf rect( 5, 5, 320, 230 );
        m_gui.beginWindow( "Profiler", rect, rect );
        zp_char buf[ 256 ];

        for( zp_size_t i = 0, imax = timeline.frames.size(); i < imax; ++i )
        {
            const zpProfilerFrame& frame = timeline.frames[ i ];

            zp_snprintf( buf, 256, 256, "%-16s %-32s %-16s %4d %8dt %8db", frame.fileName, frame.functionName, frame.eventName, frame.lineNumber, ( frame.endTime - frame.startTime ), ( frame.endMemory - frame.startMemory ) );
            //buff << frame.fileName << "@" << frame.functionName << ":" << frame.lineNumber << "    " << ( frame.endTime - frame.startTime ) << "t " << ( frame.endMemory - frame.startMemory ) << "b";
            m_gui.label( 18, buf, zpColor4f( 0, 0, 0, 1 ) );
            //buff.clear();
        }

        m_gui.endWindow();
    }

#if 0
    // draw FPS
    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_FPS ) )
    {
        zp_float frameMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_FRAME, secondsPerTick );
        zp_float procFramMS = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_PROCESS_FRAME, secondsPerTick );

        buff << frameMs * 1000.f << " ms ";
        buff << ( 1.f / ( frameMs ) ) << " fps ";
        buff << ( 1.f / ( procFramMS ) ) << " actual fps ";

#if 0
        zpRectf rect( 5, 5, 320, 24 + ( ZP_PROFILER_MAX_FRAMES * 5 ) + 24 );
#else
        zpRectf rect( 5, 5, 320, 50 );
#endif
        m_gui.beginWindow( "FPS Stats", rect, rect );
        
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 0, 1 ) );
        buff.clear();

        buff << "Mem Used " << m_profiler.getPreviousMemoryUsed( ZP_PROFILER_STEP_FRAME ) << "B";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 0, 1 ) );
        buff.clear();

#if 0
        m_gui.setMargin( 0, 1 );
        
        const zpProfilerTimeline& timeline = m_profiler.getTimeline( ZP_PROFILER_STEP_FRAME );
        zp_long maxTime = m_profiler.getMaxTime( ZP_PROFILER_STEP_FRAME );
        for( zp_int i = 0; i < ZP_PROFILER_MAX_FRAMES; ++i )
        {
            zp_float percent = (zp_float)( timeline.frames[ i ].endTime - timeline.frames[ i ].startTime ) / (zp_float)maxTime;
            m_gui.box( percent, 4, zpColor4f( 0, 1, 1, 1 ) );
        }
#endif
        m_gui.endWindow();
    }

    // draw rendering stats
    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_RENDERING ) )
    {
        zp_float renderAllMs =         m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER                , secondsPerTick );
        zp_float renderBeginMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_BEGIN        , secondsPerTick );
        zp_float renderFrameMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_FRAME        , secondsPerTick );
        zp_float renderMeshesMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_MESHES        , secondsPerTick );
        zp_float renderParticlesMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_PARTICLES    , secondsPerTick );
        zp_float renderUIMs =         m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_UI            , secondsPerTick );
        zp_float renderPresentMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_PRESENT        , secondsPerTick );
        zp_float renderDebugMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_DEBUG_RENDER        , secondsPerTick );
        zp_float renderGUIMs =         m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_DRAW_GUI            , secondsPerTick );

        zpRectf rect( 5, 5, 320, 230 );
        m_gui.beginWindow( "Rendering", rect, rect );

        buff << "Submit    " << renderAllMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "Begin     " << renderBeginMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "Frame     " << renderFrameMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "Mesh      " << renderMeshesMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "Particles " << renderParticlesMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "UI        " << renderUIMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "Present   " << renderPresentMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "Debug     " << renderDebugMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        buff << "GUI       " << renderGUIMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
        buff.clear();

        m_gui.endWindow();
    }

    // draw update stats
    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_UPDATE ) )
    {
        zp_float updateMs =             m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_UPDATE            , secondsPerTick );
        zp_float simulateMs =         m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_FIXED_UPDATE        , secondsPerTick );
        zp_float sleepMs =             m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_SLEEP            , secondsPerTick );
        zp_float objectUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_OBJECT_UPDATE    , secondsPerTick );
        zp_float worldUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_WORLD_UPDATE    , secondsPerTick );
        zp_float scriptUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_SCRIPT_UPDATE    , secondsPerTick );
        zp_float inputUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_INPUT_UPDATE    , secondsPerTick );
        zp_float audioUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_AUDIO_UPDATE    , secondsPerTick );
        zp_float physicsUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_PHYSICS_UPDATE    , secondsPerTick );
        zp_float phaseUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_UPDATE_PHASE    , secondsPerTick );
        zp_float stateUpdateMs =     m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_UPDATE_STATE    , secondsPerTick );

        zpRectf rect( 5, 5, 320, 320 );
        m_gui.beginWindow( "Update", rect, rect );

        buff << "Update   " << updateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Simulate " << simulateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Sleep    " << sleepMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Object   " << objectUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "World    " << worldUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Script   " << scriptUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Input    " << inputUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Audio    " << audioUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Physics  " << physicsUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "Phase    " << phaseUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        buff << "State    " << stateUpdateMs * 1000.f << " ms";
        m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
        buff.clear();

        m_gui.endWindow();
    }
#endif

    if( m_displayStats.isMarked( ZP_APPLICATION_STATS_RENDERING_CMDS ) )
    {
        const zpRenderingStats& stats = m_renderingPipeline.getRenderingEngine()->getImmediateRenderingContext()->getPreviousFrameStats();

        zpRectf rect( 5, 5, 320, 230 );
        m_gui.beginWindow( "Rendering Stats", rect, rect );

        buff << "Num Draw Cmds " << stats.totalDrawCommands;
        m_gui.label( 24, buff.str(), zpColor4f( 1, 1, 0, 1 ) );
        buff.clear();

        buff << "Total Verts   " << stats.totalVerticies;
        m_gui.label( 24, buff.str(), zpColor4f( 1, 1, 0, 1 ) );
        buff.clear();

        for( zp_size_t i = 0; i < zpRenderingQueue_Count; ++i )
        {
            zp_uint numCommands = stats.numDrawCommands[ i ];
            if( numCommands )
            {
                buff << g_renderingQueues[ i ] << "  " << stats.visibleDrawCommands[ i ] << "/" << numCommands;
                m_gui.label( 24, buff.str(), zpColor4f( 1, 1, 0, 1 ) );
                buff.clear();
            }
        }

        m_gui.endWindow();
    }

    m_gui.endGUI();
}

zp_uint zpApplication::getFrameCount() const
{
    return m_frameCount;
}

zp_bool zpApplication::isApplicationPaused() const
{
    return m_isApplicationPaused;
}
void zpApplication::setApplicationPaused( zp_bool applicationPaused )
{
    m_isApplicationPaused = applicationPaused;

    onApplicationPaused( applicationPaused );
}
