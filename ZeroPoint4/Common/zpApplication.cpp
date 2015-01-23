#include "zpCommon.h"

#define ZP_APPLICATION_DEFAULT_OPTIONS_FILE		"zp.json"
#define ZP_APPLICATION_DEFAULT_WINDOW_WIDTH		640
#define ZP_APPLICATION_DEFAULT_WINDOW_HEIGHT	480

zpApplication::zpApplication()
	: m_isRunning( false )
	, m_hasNextWorld( false )
	, m_addNextWorld( false )
	, m_shouldGarbageCollect( false )
	, m_shouldReloadAllResources( false )
	, m_inEditMode( false )
	, m_currentPhase( 0 )
	, m_exitCode( 0 )
	, m_optionsFilename( ZP_APPLICATION_DEFAULT_OPTIONS_FILE )
	, m_console( ZP_NULL )
	, m_timer( zpTime::getInstance() )
	, m_currentWorld( ZP_NULL )
	, m_nextWorld( ZP_NULL )
	, m_lastTime( 0 )
	, m_simulateHz( 10000000 / 60 )
	, m_renderHz(   10000000 / 120 )
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

void zpApplication::popCurrentPhase()
{
	ZP_ASSERT( m_currentPhase >= 0, "Trying to pop an empty phase stack" );

	m_phases[ m_currentPhase ]->onLeavePhase( this );
	m_currentPhase--;
}
void zpApplication::updatePhase()
{
	zpApplicationPhaseResult r = m_phases[ m_currentPhase ]->onUpdatePhase( this );
	switch( r )
	{
	case ZP_APPLICATION_PHASE_NORMAL:
		break;
	case ZP_APPLICATION_PHASE_COMPLETE:
		pushNextPhase();
		break;
	case ZP_APPLICATION_PHASE_FAILURE:
		popCurrentPhase();
		break;
	}
}
void zpApplication::pushNextPhase()
{
	++m_currentPhase;
	m_phases[ m_currentPhase ]->onEnterPhase( this );
}

void zpApplication::initialize( const zpArrayList< zpString >& args )
{
	m_textContent.setApplication( this );
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
	
	zp_bool ok;
	ok = m_textContent.getResource( m_optionsFilename, m_appOptions );
	ZP_ASSERT( ok, "Failed to load Options '%s'", m_optionsFilename.str() );
	if( !ok )
	{
		return;
	}

	const zpBison::Value& appOptions = m_appOptions.getResource()->getData()->root();

	const zpBison::Value& console = appOptions[ "Console" ];
	if( console.asBool() )
	{
		m_console = zpConsole::getInstance();
		m_console->create();
	}

	const zpBison::Value& window = appOptions[ "Window" ];
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

	const zpBison::Value& render = appOptions[ "Render" ];
	ZP_ASSERT( render.isObject(), "" );

	zpDisplayMode displayMode;
	displayMode.width = size.getX();
	displayMode.height = size.getY();
	displayMode.refreshRate = render[ "Refresh" ].asInt();
	displayMode.displayFormat = ZP_DISPLAY_FORMAT_RGBA8_UNORM;

	zpRenderingEngine* re = m_renderingPipeline.getRenderingEngine();
	re->setScreenMode( ZP_SCREEN_MODE_WINDOWED );
	re->setDisplayMode( displayMode );
	re->setVSyncEnabled( render[ "VSync" ].asBool() );
	re->create( m_window.getWindowHandle(), m_window.getScreenSize() );

	m_lastTime = m_timer->getTime();

	m_renderingPipeline.initialize();

	m_gui.setApplication( this );
	m_gui.create();

	m_audioContent.getAudioEngine()->create( m_window.getWindowHandle() );

	zpAngelScript::createInstance();
	zpAngelScript::getInstance()->createEngine( this );

	// register input with window
	m_window.addFocusListener( &m_inputManager );
	m_window.addProcListener( &m_inputManager );
}
void zpApplication::setup()
{
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

	m_currentPhase = -1;

	m_protoDBManager.setProtoDBFile( appOptions[ "ProtoDB" ].asCString() );

	m_renderingPipeline.getMaterialContentManager()->getResource( appOptions[ "DefaultMaterial" ].asCString(), m_defaultMaterial );
}

void zpApplication::run()
{
	m_isRunning = true;

	pushNextPhase();

	while( m_isRunning && m_window.processMessages() )
	{
		processFrame();
	}
}

void zpApplication::teardown()
{
	m_appOptions.release();

	m_defaultMaterial.release();
	m_gui.destroy();

	m_objectContent.destroyAllObjects();
	m_objectContent.update();

	m_worldContent.destroyAllWorlds();
	m_worldContent.update();

	// pop all phases
	while( m_currentPhase >= 0 )
	{
		popCurrentPhase();
	}

	garbageCollect();
}
zp_int zpApplication::shutdown()
{
	m_phases.clear();

	m_renderingPipeline.destroy();
	m_physicsEngine.destroy();

	runGarbageCollect();

	zpAngelScript::getInstance()->destroyEngine();
	zpAngelScript::destroyInstance();

	m_protoDBManager.destroy();

	m_audioContent.getAudioEngine()->destroy();

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
	updatePhase();

	zp_bool initalizeCurrentWorld = false;
	if( m_hasNextWorld && !m_nextWorldFilename.isEmpty() )
	{
		//// if the next world should be added
		//if( m_addNextWorld )
		//{
		//	m_worldContent.createWorld( m_nextWorldFilename.str(), true );
		//}
		//// otherwise, do a world swap
		//else
		//{
		//	// if the current world is set, it's a world swap, unload objects
		//	if( m_currentWorld != ZP_NULL )
		//	{
		//		m_currentWorld->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
		//		m_objectContent.destroyAllObjects( true );
		//	}
		//
		//	// create the new world and mark for create
		//	m_currentWorld = m_worldContent.createWorld( m_nextWorldFilename.str(), false );
		//}

		// destroy old world and objects
		if( m_currentWorld != ZP_NULL )
		{
			m_currentWorld->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
			m_objectContent.destroyAllObjectsInWorld( m_currentWorld );
		}

		// load the loading world and create it's objects
		m_currentWorld = m_worldContent.createWorld( m_loadingWorldFilename.str(), false );
		initalizeCurrentWorld = true;

		// step load the next world
		m_nextWorld = m_worldContent.createWorld( m_nextWorldFilename.str(), false );
		m_nextWorld->setFlag( ZP_WORLD_FLAG_STEP_CREATE );

		/*
		destroy objects in current world
		destroy current world
		current = loading scene
		next = next scene

		current is done, destroy current
		current = next
		next = null
		*/

		// reset state
		m_nextWorldFilename.clear();
		m_hasNextWorld = false;
		m_addNextWorld = false;
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

		// initialize all objects in the world
		initalizeCurrentWorld = true;

		// collect garbage next frame
		garbageCollect();
	}

	// update object, delete any etc
	ZP_PROFILE_START( OBJECT_UPDATE );
	m_objectContent.update();
	ZP_PROFILE_END( OBJECT_UPDATE );

	// update world, delete, create objects, etc.
	ZP_PROFILE_START( WORLD_UPDATE );
	m_worldContent.update();
	ZP_PROFILE_END( WORLD_UPDATE );

	if( initalizeCurrentWorld )
	{
		m_objectContent.initializeAllObjectsInWorld( m_currentWorld );
	}

	m_physicsEngine.update( m_timer->getDeltaSeconds() );

	// update all components
#undef ZP_COMPONENT_DEF
#define ZP_COMPONENT_DEF( cmp ) m_componentPool##cmp.update();
	#include "zpAllComponents.inl"
#undef ZP_COMPONENT_DEF

	// update input
	ZP_PROFILE_START( INPUT_UPDATE );
	m_inputManager.update();
	ZP_PROFILE_END( INPUT_UPDATE );

	m_renderingPipeline.update();

	ZP_PROFILE_START( AUDIO_UPDATE );
	m_audioContent.getAudioEngine()->update();
	ZP_PROFILE_END( AUDIO_UPDATE );

	handleInput();

	m_gui.startGUI();
	onGUI();
	//if( m_inEditMode ) guiEditMode();
	m_gui.endGUI();
}
void zpApplication::simulate()
{
	if( !m_inEditMode )
	{
		//m_componentPoolEditorCamera.simulate();
		m_physicsEngine.simulate();

		m_componentPoolRigidBody.simulate();
	}
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
	zp_printfcln( ZP_CC_LIGHT_BLUE, "focused" );
	m_inputManager.onFocusGained();
	m_renderingPipeline.onFocusGained();
}
void zpApplication::onFocusLost()
{
	zp_printfcln( ZP_CC_LIGHT_RED, "unfocused" );
	m_inputManager.onFocusLost();
	m_renderingPipeline.onFocusLost();
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

	if( strFilename.endsWith( ".json" ) || strFilename.endsWith( ".jsonb" ) )
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
		m_nextWorldFilename = filename;
		m_hasNextWorld = true;
		loaded = true;
	}
	else if( strFilename.endsWith( ".shaderb" ) )
	{
		loaded = m_renderingPipeline.getShaderContentManager()->reloadResource( filename );
	}
	else if( strFilename.endsWith( ".png" ) )
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
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_TAB ) )
	{
		m_inEditMode = !m_inEditMode;
		if( m_inEditMode )
		{
			enterEditMode();
		}
		else
		{
			leaveEditMode();
		}
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
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_F1 ) )
	{
		m_displayStats.toggle( ZP_APPLICATION_STATS_FPS );
	}
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_F2 ) )
	{
		m_displayStats.toggle( ZP_APPLICATION_STATS_RENDERING );
	}
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_F3 ) )
	{
		m_displayStats.toggle( ZP_APPLICATION_STATS_UPDATE );
	}
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_F9 ) )
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
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_F11 ) )
	{
		m_renderingPipeline.takeScreenshot( keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) ? ZP_SCREENSHOT_TYPE_NO_UI : ZP_SCREENSHOT_TYPE_ALL, "." );
	}

	// draw physics debug
	if( m_displayStats.isMarked( ZP_APPLICATION_STATS_DRAW_PHYSICS ) )
	{
		zpRenderingContext* context = m_renderingPipeline.getRenderingEngine()->getImmediateRenderingContext();
		context->beginDrawImmediate( ZP_RENDERING_LAYER_DEFAULT, ZP_RENDERING_QUEUE_OPAQUE_DEBUG, ZP_TOPOLOGY_LINE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_defaultMaterial );
		m_physicsEngine.debugDraw();
		context->endDrawImmediate();
	}
}

void zpApplication::loadWorld( const zp_char* worldFilename )
{
	m_addNextWorld = false;
	m_hasNextWorld = true;
	m_nextWorldFilename = worldFilename;
}
void zpApplication::loadWorldAdditive( const zp_char* worldFilename )
{
	m_addNextWorld = true;
	m_hasNextWorld = true;
	m_nextWorldFilename = worldFilename;
}
zp_float zpApplication::getLoadingWorldProgress() const
{
	return m_nextWorld == ZP_NULL ? 1.f : m_nextWorld->getLoadingProgress();
}

void zpApplication::processFrame()
{
	m_timer->tick();
	zp_long now = m_timer->getTime();

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

	// update
	ZP_PROFILE_START( UPDATE );
	update();
	ZP_PROFILE_END( UPDATE );

	// simulate
	ZP_PROFILE_START( SIMULATE );
	zp_int numUpdates = 0;
	while( ( now - m_lastTime ) > m_simulateHz && numUpdates < 5 )
	{
		simulate();
		m_lastTime += m_simulateHz;
		++numUpdates;
	}
	ZP_PROFILE_END( SIMULATE );

	// adjust timer
	if( ( now - m_lastTime ) > m_simulateHz )
	{
		m_lastTime = now - m_simulateHz;
	}

	m_timer->setInterpolation( (zp_float)( now - m_lastTime ) / (zp_float)m_simulateHz );

	ZP_PROFILE_START( RENDER_FRAME );
	{
		zpRenderingContext* i = m_renderingPipeline.getRenderingEngine()->getImmediateRenderingContext();

		// individual component render
		ZP_PROFILE_START( RENDER_MESHES );
		m_componentPoolMeshRenderer.render( i );
		ZP_PROFILE_END( RENDER_MESHES );

		// render particles for each camera
		ZP_PROFILE_START( RENDER_PARTICLES );
		for( zp_uint p = 0; p < zpCameraType_Count; ++p )
		{
			const zpArrayList< zpCamera* >& cameras = m_renderingPipeline.getUsedCameras( (zpCameraType)p );
			cameras.foreach( [ i, this ]( const zpCamera* camera ) 
			{
				m_componentPoolParticleEmitter.render( i, camera );
			} );
		}
		ZP_PROFILE_END( RENDER_PARTICLES );
		
		// render begin
		ZP_PROFILE_START( RENDER_BEGIN );
		m_renderingPipeline.beginFrame( i );
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

	ZP_PROFILE_START( SLEEP );
	// sleep for the remainder of the frame
	zp_long endTime = m_timer->getTime();

	zp_long diff = ( endTime - now );
	zp_long sleepTime = ( ( m_renderHz - diff ) * 1000L ) / ( m_timer->getCountsPerSecond() );
	zp_sleep( (zp_uint)sleepTime );
	ZP_PROFILE_END( SLEEP );

	// increment frame counter
	++m_frameCount;
}

void zpApplication::addPhase( zpApplicationPhase* phase )
{
	ZP_ASSERT( !m_isRunning, "Trying to add Phases to a running application" );
	m_phases.pushBack( phase );
}

void zpApplication::runGarbageCollect()
{
	m_textContent.garbageCollect();
	m_objectContent.garbageCollect();
	m_worldContent.garbageCollect();
	m_scriptContent.garbageCollect();
	m_audioContent.garbageCollect();

	m_renderingPipeline.getFontContentManager()->garbageCollect();
	m_renderingPipeline.getMeshContentManager()->garbageCollect();
	m_renderingPipeline.getMaterialContentManager()->garbageCollect();
	m_renderingPipeline.getShaderContentManager()->garbageCollect();
	m_renderingPipeline.getTextureContentManager()->garbageCollect();
}
void zpApplication::runReloadAllResources()
{
	m_textContent.reloadAllResources();
	m_objectContent.reloadAllResources();
	m_worldContent.reloadAllResources();
	m_scriptContent.reloadAllResources();
	m_audioContent.reloadAllResources();

	m_renderingPipeline.getFontContentManager()->reloadAllResources();
	m_renderingPipeline.getMeshContentManager()->reloadAllResources();
	m_renderingPipeline.getMaterialContentManager()->reloadAllResources();
	m_renderingPipeline.getShaderContentManager()->reloadAllResources();
	m_renderingPipeline.getTextureContentManager()->reloadAllResources();

	//m_protoDBManager.reloadProtoDB();
}

#if ZP_USE_HOT_RELOAD
void zpApplication::runReloadChangedResources()
{
	m_textContent.reloadChangedResources();
	m_objectContent.reloadChangedResources();
	m_worldContent.reloadChangedResources();
	m_scriptContent.reloadChangedResources();
	m_audioContent.reloadChangedResources();

	m_renderingPipeline.getMeshContentManager()->reloadChangedResources();
	m_renderingPipeline.getMaterialContentManager()->reloadChangedResources();
	m_renderingPipeline.getShaderContentManager()->reloadChangedResources();
	m_renderingPipeline.getTextureContentManager()->reloadChangedResources();

	//m_protoDBManager.reloadChangedProtoDB();
}
#endif

void zpApplication::enterEditMode()
{
	zp_printfln( "enter edit" );
#if 0
	class EditorCameraController : public zpCameraState
	{
	public:
		~EditorCameraController() {}

		void onEnter( zpCamera* camera )
		{
			startPos = camera->getPosition();
			startLook = camera->getLookTo();
			startUp = camera->getUp();
		};
		void onLeave( zpCamera* camera )
		{
			camera->setPosition( startPos );
			camera->setLookTo( startLook );
			camera->setUp( startUp );
		};

		zp_bool onUpdate( zpApplication* app, zpCamera* camera )
		{
			const zpMouse* mouse = app->getInputManager()->getMouse();
			const zpKeyboard* keyboard = app->getInputManager()->getKeyboard();
			zp_float dt = zpTime::getInstance()->getWallClockDeltaSeconds();

			const zpVector4f& pos = camera->getPosition();
			const zpVector4f& forward = camera->getLookTo();
			const zpVector4f& up = camera->getUp();

			zpVector4f right;
			zpMath::Cross3( right, forward, up );

			zp_int w = mouse->getScrollWheelDelta();
			const zpVector2i& mouseDelta = mouse->getDelta();

			if( w != 0 )
			{
				if( keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) )
				{
					w *= 6;
				}

				zpVector4f f;
				zpMath::Mul( f, forward, zpScalar( (zp_float)w ) );
				zpMath::Add( f, pos, f );
				
				camera->setPosition( f );
			}
			
			if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) )
			{
				zp_bool leftButton = mouse->isButtonDown( ZP_MOUSE_BUTTON_LEFT );
				zp_bool rightButton = mouse->isButtonDown( ZP_MOUSE_BUTTON_RIGHT );
				if( leftButton && !rightButton )
				{
					zpVector4f lootTo( camera->getLookTo() );

					zpVector4f r, u;
					zpMath::Mul( r, right, zpScalar( dt * mouseDelta.getX() ) );
					zpMath::Mul( u, up, zpScalar( dt * mouseDelta.getY() ) );
					zpMath::Add( r, r, u );
					zpMath::Add( r, r, pos );
					
					camera->setPosition( r );
					camera->setLookTo( lootTo );
				}
				else if( rightButton && !leftButton )
				{
					zpVector4f lootAt( camera->getLookAt() );

					zpVector4f camPos( pos );
					zpMath::Sub( camPos, camPos, lootAt );
					zpMath::RotateY( camPos, camPos, zpScalar( ZP_DEG_TO_RAD( mouseDelta.getX() ) ) );
					zpMath::RotateX( camPos, camPos, zpScalar( ZP_DEG_TO_RAD( -mouseDelta.getY() ) ) );
					zpMath::Add( camPos, camPos, lootAt );

					camera->setPosition( camPos );
					camera->setLookAt( lootAt );
				}
				else if( rightButton && leftButton )
				{

				}
			}

			return false;
		};

	private:
		zpVector4f startPos, startLook, startUp;
	};

	m_renderingPipeline.pushCameraState< EditorCameraController >( ZP_CAMERA_TYPE_MAIN );
#endif

	m_timer->setTimeScale( 0.f );
}
void zpApplication::leaveEditMode()
{
	m_timer->setTimeScale( 1.f );

	zp_printfln( "leave edit" );
}

void zpApplication::guiEditMode()
{
	static zpRectf window( 10, 10, 320, 480 );
	m_gui.beginWindow( "Application", window, window );

	m_gui.button( 22, "Click Me" );
	m_gui.button( 22, "Click Me" );
	m_gui.label( 16, "First Label Here", zpColor4f( 1, 1, 1, 1 ) );
	if( m_gui.button( 22, "Click Me" ) )
	{
		zp_printfln( "Clicked!" );
	}
	m_gui.label( 16, "Label Here", zpColor4f( 1, 1, 1, 1 ) );

	m_gui.endWindow();
}
void zpApplication::onGUI()
{
	// draw FPS
	if( m_displayStats.isMarked( ZP_APPLICATION_STATS_FPS ) )
	{
		zp_float frameMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_FRAME );
		zpFixedStringBuffer< 64 > fps;
		fps << frameMs * 1000.f << " ms " << ( 1.f / ( frameMs ) ) << " fps";

		m_gui.label( 24, fps.str(), zpColor4f( 1, 1, 1, 1 ) );
	}

	// draw rendering stats
	if( m_displayStats.isMarked( ZP_APPLICATION_STATS_RENDERING ) )
	{
		zp_float renderAllMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER );
		zp_float renderBeginMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_BEGIN );
		zp_float renderFrameMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_FRAME );
		zp_float renderMeshesMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_MESHES );
		zp_float renderParticlesMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_PARTICLES );
		zp_float renderPresentMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_RENDER_PRESENT );
		zp_float renderDebugMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_DEBUG_RENDER );
		zpFixedStringBuffer< 64 > buff;

		zpRectf rect( 5, 5, 320, 200 );
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

		buff << "Present   " << renderPresentMs * 1000.f << " ms";
		m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
		buff.clear();

		buff << "Debug     " << renderDebugMs * 1000.f << " ms";
		m_gui.label( 24, buff.str(), zpColor4f( 1, 0, 0, 1 ) );
		buff.clear();

		m_gui.endWindow();
	}

	// draw update stats
	if( m_displayStats.isMarked( ZP_APPLICATION_STATS_UPDATE ) )
	{
		zp_float updateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_UPDATE );
		zp_float simulateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_SIMULATE );
		zp_float objectUpdateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_OBJECT_UPDATE );
		zp_float worldUpdateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_WORLD_UPDATE );
		zp_float scriptUpdateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_SCRIPT_UPDATE );
		zp_float inputUpdateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_INPUT_UPDATE );
		zp_float audioUpdateMs = m_profiler.getPreviousTimeSeconds( ZP_PROFILER_STEP_AUDIO_UPDATE );
		zpFixedStringBuffer< 64 > buff;

		zpRectf rect( 5, 5, 320, 200 );
		m_gui.beginWindow( "Update", rect, rect );

		buff << "Update   " << updateMs * 1000.f << " ms";
		m_gui.label( 24, buff.str(), zpColor4f( 0, 0, 1, 1 ) );
		buff.clear();

		buff << "Simulate " << simulateMs * 1000.f << " ms";
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

		m_gui.endWindow();
	}
}
