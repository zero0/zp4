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
	, m_exitCode( 0 )
	, m_optionsFilename( ZP_APPLICATION_DEFAULT_OPTIONS_FILE )
	, m_console( ZP_NULL )
	, m_timer( zpTime::getInstance() )
	, m_currentWorld( ZP_NULL )
	, m_lastTime( 0 )
	, m_simulateHz( 10000000 / 60 )
	, m_renderMsHz( 1000 / 120 )
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

void zpApplication::initialize( const zpArrayList< zpString >& args )
{
	m_textContent.setApplication( this );
	m_objectContent.setApplication( this );
	m_worldContent.setApplication( this );
	m_scriptContent.setApplication( this );
	m_audioContent.setApplication( this );

	m_renderingPipeline.setApplication( this );
	m_renderingPipeline.getMaterialContentManager()->setApplication( this );
	m_renderingPipeline.getShaderContentManager()->setApplication( this );
	m_renderingPipeline.getTextureContentManager()->setApplication( this );
	m_renderingPipeline.getMeshContentManager()->setApplication( this );

	zp_bool ok;
	ok = m_textContent.getResource( m_optionsFilename, m_appOptions );
	ZP_ASSERT( ok, "Failed to load Options '%s'", m_optionsFilename.str() );
	if( !ok )
	{
		return;
	}

	const zpBison::Value& appOptions = m_appOptions.getResource()->getData()->root();

	const zpBison::Value console = appOptions[ "Console" ];
	if( console.asBool() )
	{
		m_console = zpConsole::getInstance();
		m_console->create();
	}

	const zpBison::Value window = appOptions[ "Window" ];
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

	const zpBison::Value render = appOptions[ "Render" ];
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

	m_isRunning = true;
	m_lastTime = m_timer->getTime();

	m_renderingPipeline.initialize();

	m_audioContent.getAudioEngine()->create( m_window.getWindowHandle() );

	zpAngelScript::createInstance();
	zpAngelScript::getInstance()->createEngine( this );

	// register input with window
	m_window.addFocusListener( &m_inputManager );
	m_window.addProcListener( &m_inputManager );

	const zpBison::Value world = appOptions[ "World" ];
	if( world.isString() )
	{
		m_nextWorldFilename = world.asCString();
		m_hasNextWorld = true;
	}
}
void zpApplication::run()
{
	while( m_isRunning && m_window.processMessages() )
	{
		processFrame();
	}
}
zp_int zpApplication::shutdown()
{
	m_renderingPipeline.destroy();

	m_objectContent.destroyAllObjects( false );
	m_objectContent.update();

	m_worldContent.destroyAllWorlds();
	m_worldContent.update();

	m_appOptions.release();

	runGarbageCollect();

	zpAngelScript::getInstance()->destroyEngine();
	zpAngelScript::destroyInstance();

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
	if( m_hasNextWorld && !m_nextWorldFilename.isEmpty() )
	{
		// if the next world should be added
		if( m_addNextWorld )
		{
			zpWorld* addWorld = m_worldContent.createWorld( m_nextWorldFilename.str() );
			addWorld->setFlag( ZP_WORLD_FLAG_SHOULD_CREATE );
			addWorld->setFlag( ZP_WORLD_FLAG_DESTROY_AFTER_LOAD );
		}
		// otherwise, do a world swap
		else
		{
			// if the current world is set, it's a world swap, unload objects
			if( m_currentWorld )
			{
				m_currentWorld->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
				m_objectContent.destroyAllObjects( true );
			}

			// create the new world and mark for create
			m_currentWorld = m_worldContent.createWorld( m_nextWorldFilename.str() );
			m_currentWorld->setFlag( ZP_WORLD_FLAG_SHOULD_CREATE );
		}

		// reset state
		m_nextWorldFilename.clear();
		m_hasNextWorld = false;
		m_addNextWorld = false;
	}

	// update world, delete, create objects, etc.
	ZP_PROFILE_START( WORLD_UPDATE );
	m_worldContent.update();
	ZP_PROFILE_END( WORLD_UPDATE );

	// update object, delete any etc
	ZP_PROFILE_START( OBJECT_UPDATE );
	m_objectContent.update();
	ZP_PROFILE_END( OBJECT_UPDATE );

	// update input
	ZP_PROFILE_START( INPUT_UPDATE );
	m_inputManager.update();
	ZP_PROFILE_END( INPUT_UPDATE );

	m_renderingPipeline.update();
	//m_componentPoolEditorCamera.update();

	ZP_PROFILE_START( SCRIPT_UPDATE );
	m_componentPoolScript.update();
	ZP_PROFILE_END( SCRIPT_UPDATE );

	ZP_PROFILE_START( SCRIPT_PROC_THREADS );
	zpAngelScript::getInstance()->processThreads();
	ZP_PROFILE_END( SCRIPT_PROC_THREADS );

	//m_componentPoolAudioEmitter.update();

	m_audioContent.getAudioEngine()->update();

	handleInput();
}
void zpApplication::simulate()
{
	if( !m_inEditMode )
	{
		//m_componentPoolEditorCamera.simulate();
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
		zpObject* o = m_objectContent.createObject( this, filename );
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
	else if( keyboard->isKeyPressed( ZP_KEY_CODE_P ) )
	{
		m_profiler.printProfile( ZP_PROFILER_STEP_FRAME );
	}
}

void zpApplication::processFrame()
{
	m_timer->tick();

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


	zp_long now = m_timer->getTime();
	zp_uint numUpdates = 0;

	// update
	ZP_PROFILE_START( UPDATE );
	update();
	ZP_PROFILE_END( UPDATE );

	// simulate
	ZP_PROFILE_START( SIMULATE );
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
		// individual component render
		m_componentPoolMeshRenderer.render();

		// render begin
		ZP_PROFILE_START( RENDER_BEGIN );
		m_renderingPipeline.beginFrame();
		ZP_PROFILE_END( RENDER_BEGIN );

		// render commands
		ZP_PROFILE_START( RENDER );
		m_renderingPipeline.submitRendering();
		ZP_PROFILE_END( RENDER );

		// present
		ZP_PROFILE_START( RENDER_PRESENT );
		m_renderingPipeline.endFrame();
		ZP_PROFILE_END( RENDER_PRESENT );
	}
	ZP_PROFILE_END( RENDER_FRAME );

	ZP_PROFILE_END( FRAME );

	// sleep for the remainder of the frame
	m_timer->sleep( m_renderMsHz );
}

void zpApplication::runGarbageCollect()
{
	m_textContent.garbageCollect();
	m_objectContent.garbageCollect();
	m_worldContent.garbageCollect();
	m_scriptContent.garbageCollect();
	m_audioContent.garbageCollect();

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

	m_renderingPipeline.getMeshContentManager()->reloadAllResources();
	m_renderingPipeline.getMaterialContentManager()->reloadAllResources();
	m_renderingPipeline.getShaderContentManager()->reloadAllResources();
	m_renderingPipeline.getTextureContentManager()->reloadAllResources();
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
}
#endif

void zpApplication::enterEditMode()
{
	zp_printfln( "enter edit" );
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
			if( w != 0 )
			{
				if( keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) )
				{
					w *= 6;
				}

				zpVector4f f;
				zpMath::Mul( f, forward, zpScalar( w ) );
				zpMath::Add( f, pos, f );
				
				camera->setPosition( f );
			}
			
			if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) )
			{
				if( mouse->isButtonDown( ZP_MOUSE_BUTTON_LEFT ) )
				{
					zpVector4f r, u;
					zpMath::Mul( r, right, zpScalar( dt * mouse->getDelta().getX() ) );
					zpMath::Mul( u, up, zpScalar( dt * mouse->getDelta().getY() ) );
					zpMath::Add( r, r, u );
					zpMath::Add( r, r, pos );

					camera->setPosition( r );
				}
			}

			return false;
		};

	private:
		zpVector4f startPos, startLook, startUp;
	};
	
	m_renderingPipeline.pushCameraState< EditorCameraController >( ZP_CAMERA_TYPE_MAIN );
}
void zpApplication::leaveEditMode()
{
	zp_printfln( "leave edit" );
	m_renderingPipeline.popCameraState( ZP_CAMERA_TYPE_MAIN );
}
