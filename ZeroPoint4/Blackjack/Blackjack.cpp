#include "Blackjack.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#define DEFAULT_BLACKJACK_CONFIG	"blackjack.config"

void ProcessConfig( zpGame& game )
{
#if 0
	zpProperties properties( zpString( DEFAULT_BLACKJACK_CONFIG ) );
	ZP_ASSERT( properties.hasProperty( "game.file" ), "Game file not defined" );
	const zpString& gameFile = properties[ "game.file" ];

	if( properties[ "console.enabled" ] == "true" ) {
	//	zpConsole::getInstance()->create();
	}

	zpXmlParser gameParser;
	gameParser.parseFile( gameFile, true );

	zpProperties gameOverrides( properties.getSubProperties( "game.overrides" ) );

	zpXmlSerializedInput gameIn( gameParser.getRootNode(), gameOverrides );
	game.deserialize( &gameIn );
	game.create();

	const zpString& worldPrefab = properties[ "world.prefab" ];
	
	//zpSerializable* world = zpPrefabs::getInstance()->loadPrefab( worldPrefab );
	//ZP_ASSERT( world != ZP_NULL, "Failed to load world prefab %s", worldPrefab.c_str() );
	
	//game.setNextWorld( (zpWorld*)world );

	//return game;
#endif
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{

	return 0;
}
#if 0
{
	zpConsole::getInstance()->create();

	zpJson outJson;
	zpJsonParser parser;

	zp_long s = zpTime::getInstance()->getTime();
	parser.parseFile( zpString( "test.txt" ), outJson );
	zp_long e = zpTime::getInstance()->getTime();

	zp_printfln( "JSON Parse + File IO %d", e - s );

	zpDataBuffer buff;

	s = zpTime::getInstance()->getTime();
	zpBison::compileToBuffer( buff, outJson );
	//zpBison::compileToFile( zpString( "bison.txt" ), outJson );	
	e = zpTime::getInstance()->getTime();
	zp_printfln( "Bison Compile %d", e - s );

	zpBison bison;
	s = zpTime::getInstance()->getTime();
	bison.readFromBuffer( buff );
	//bison.readFromFile( zpString( "bison.txt" ) );
	e = zpTime::getInstance()->getTime();
	zp_printfln( "Bison Parse + File IO %d", e - s );

	ZP_ASSERT( outJson[ "a" ].asInt() != bison.root()[ "a" ].asInt(), "" );

	zp_printfln( "JSON:\n%s", zpJsonWriter().styleWrite( outJson ) );

	zp_printfln( "BISON:\n%s", zpBisonWriter().styleWrite( bison ) );
	return 0;
}
#endif
#if 0
{
	zpWindow wnd;
	wnd.setPosition( zpVector2i( 20, 20 ) );
	wnd.setScreenSize( zpVector2i( 640, 360 ) );
	wnd.setTitle( zpString( "ZP4 - Blackjack" ) );

	wnd.create();
	
	zpDisplayMode displayMode;
	displayMode.width = 0;
	displayMode.height = 0;
	displayMode.refreshRate = 60;
	displayMode.displayFormat = ZP_DISPLAY_FORMAT_RGBA8_UNORM;

	zpRenderingEngine* re = zpRenderingFactory::getRenderingEngine();
	re->setWindow( &wnd );
	re->setScreenMode( ZP_SCREEN_MODE_WINDOWED );
	re->setDisplayMode( displayMode );
	re->create();

	zpRenderingContext* cxt = re->getImmediateRenderingContext();
	zpTexture* t = re->getBackBufferRenderTarget();
	cxt->setRenderTarget( 0, 1, &t, ZP_NULL );
	
	zpRasterStateDesc rs;
	zpRasterState* r = re->createRasterState( rs );
	cxt->setRasterState( r );

	while( wnd.processMessages() )
	{
		cxt->clearRenderTarget( t, zpColor4f( 1.f, 1.f, 0, 1.f ) );

		cxt->beginDrawImmediate( ZP_RENDERING_LAYER_OPAQUE, ZP_TOPOLOGY_LINE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );
		cxt->addLine( zpVector4f( 2 ), zpColor4f(), zpVector4f( 1 ), zpColor4f() );
		cxt->endDrawImmediate();

		cxt->beginDrawImmediate( ZP_RENDERING_LAYER_OPAQUE, ZP_TOPOLOGY_LINE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );
		cxt->addLine( zpVector4f( 4 ), zpColor4f(), zpVector4f( 3 ), zpColor4f() );
		cxt->addLine( zpVector4f( 5 ), zpColor4f(), zpVector4f( 6 ), zpColor4f() );
		cxt->endDrawImmediate();

		cxt->processCommands();
		re->present();
	}

	zpRenderingFactory::destroyRenderingEngine();
	wnd.destroy();

	return 0;
}
#endif
