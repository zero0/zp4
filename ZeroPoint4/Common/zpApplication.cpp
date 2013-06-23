#include "zpCommon.h"

#define ZP_APPLICATION_DEFAULT_OPTIONS_FILE		"zp.json"
#define ZP_APPLICATION_DEFAULT_WINDOW_WIDTH		640
#define ZP_APPLICATION_DEFAULT_WINDOW_HEIGHT	480

zpApplication::zpApplication()
	: m_isRunning( false )
	, m_exitCode( 0 )
	, m_optionsFilename( ZP_APPLICATION_DEFAULT_OPTIONS_FILE )
	, m_console( ZP_NULL )
	, m_timer( zpTime::getInstance() )
	, m_currentWorld( ZP_NULL )
	, m_lastTime( 0 )
	, m_simulateHz( 10000000 / 60 )
	, m_renderMsHz( 1000 / 120 )
{

}
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
	zp_bool ok;
	
	zpJsonParser parser;
	zpJson json;
	zpDataBuffer buffer;
	ok = parser.parseFile( m_optionsFilename, json );
	ZP_ASSERT( ok, "Failed to parse Options '%s'", m_optionsFilename.str() );

	ok = zpBison::compileToBuffer( buffer, json );
	ZP_ASSERT( ok, "Failed to compile Options '%s'", m_optionsFilename.str() );

	ok = m_appOptions.readFromBuffer( buffer );
	//ok = m_appOptions.readFromFile( zpString( ZP_APPLICATION_OPTIONS_FILE ) );
	ZP_ASSERT( ok, "Failed to load Options '%s'", m_optionsFilename.str() );
	if( !ok )
	{
		return;
	}

	const zpBison::Value console = m_appOptions.root()[ "Console" ];
	if( !console.isNull() )
	{
		m_console = zpConsole::getInstance();
		m_console->create();
	}

	const zpBison::Value window = m_appOptions.root()[ "Window" ];
	ZP_ASSERT( window.isObject(), "" );

	zpVector2i pos( window[ "X" ].asInt(), window[ "Y" ].asInt() );
	zpVector2i size( window[ "Width" ].asInt(), window[ "Height" ].asInt() );

	m_window.setPosition( pos );
	m_window.setScreenSize( size );
	m_window.setTitle( window[ "Title" ].asCString() );
	m_window.addProcListener( this );
	m_window.addFocusListener( this );

	m_window.create();

	const zpBison::Value render = m_appOptions.root()[ "Render" ];
	ZP_ASSERT( render.isObject(), "" );

	zpDisplayMode displayMode;
	displayMode.width = size.getX();
	displayMode.height = size.getY();
	displayMode.refreshRate = render[ "Refresh" ].asInt();
	displayMode.displayFormat = ZP_DISPLAY_FORMAT_RGBA8_UNORM;

	zpRenderingEngine* re = m_renderingPipeline.getRenderingEngine();
	re->setWindow( &m_window );
	re->setScreenMode( ZP_SCREEN_MODE_WINDOWED );
	re->setDisplayMode( displayMode );
	re->setVSyncEnabled( render[ "VSync" ].asBool() );
	re->create();

	m_isRunning = true;
	m_lastTime = m_timer->getTime();
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
	m_renderingPipeline.getRenderingEngine()->destroy();

	m_textContent.garbageCollect();

	if( m_console )
	{
		m_console->destroy();
		m_console = ZP_NULL;
	}

	m_window.destroy();

	return m_exitCode;
}

void zpApplication::update()
{
	if( m_hasNextWorld )
	{
		if( m_currentWorld )
		{
			m_currentWorld->destroy();
			m_worldContent.destroy( m_currentWorld );
		}

		m_currentWorld = m_worldContent.create( m_nextWorldFilename.str() );
		m_currentWorld->create();

		m_hasNextWorld = false;
	}

	// update object, delete any etc
	m_objectContent.update();

}
void zpApplication::simulate()
{

}

void zpApplication::garbageCollect()
{
	m_textContent.garbageCollect();
}

void zpApplication::exit( zp_int exitCode )
{
	m_isRunning = false;
	m_exitCode = exitCode;
}

const zpBison::Value& zpApplication::getOptions() const
{
	return m_appOptions.root();
}

void zpApplication::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{

}
void zpApplication::onFocusGained()
{
	zp_printfcln( ZP_CC_LIGHT_BLUE, "focused" );
}
void zpApplication::onFocusLost()
{
	zp_printfcln( ZP_CC_LIGHT_RED, "unfocused" );
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
	else if( strFilename.endsWith( ".zpo" ) )
	{
		if( m_currentWorld )
		{
			zpObject* o = m_objectContent.create( filename );
			if( o )
			{
				m_currentWorld->addObject( o );
				loaded = true;
			}
		}
	}
	else if( strFilename.endsWith( ".zpw" ) )
	{
		m_nextWorldFilename = filename;
		m_hasNextWorld = true;
	}

	return loaded;
}

void zpApplication::processFrame()
{
	m_timer->tick();

	ZP_PROFILE_START( FRAME );

	zp_long now = m_timer->getTime();
	zp_uint numUpdates = 0;

	// update
	ZP_PROFILE_START( UPDATE );
	update();
	//zp_printfcln( ZP_CC_LIGHT_GREEN, "update" );
	ZP_PROFILE_END( UPDATE );


	// simulate
	ZP_PROFILE_START( SIMULATE );
	while( ( now - m_lastTime ) > m_simulateHz && numUpdates < 5 )
	{
		simulate();
		//zp_printfcln( ZP_CC_LIGHT_BLUE, "simulate" );
		m_lastTime += m_simulateHz;
		++numUpdates;
	}
	ZP_PROFILE_END( SIMULATE );

	// adjust timer
	if( ( now - m_lastTime ) > m_simulateHz )
	{
		m_lastTime = now - m_simulateHz;
	}

	// render
	ZP_PROFILE_START( RENDER );
	m_timer->setInterpolation( (zp_float)( now - m_lastTime ) / (zp_float)m_simulateHz );
	//zp_printfcln( ZP_CC_LIGHT_YELLOW, "render" );	
	m_renderingPipeline.submitRendering();
	ZP_PROFILE_END( RENDER );

	ZP_PROFILE_START( DEBUG_RENDER );
	//zp_printfcln( ZP_CC_LIGHT_YELLOW, "render" );	
	m_renderingPipeline.submitDebugRendering();
	ZP_PROFILE_END( DEBUG_RENDER );

	ZP_PROFILE_START( RENDER_PRESENT );
	m_renderingPipeline.finalize();
	ZP_PROFILE_END( RENDER_PRESENT );

	ZP_PROFILE_END( FRAME );

	ZP_PROFILE_FINALIZE();

	// sleep for the remainder of the frame
	m_timer->sleep( m_renderMsHz );
}
