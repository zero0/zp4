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
	
	
	while( wnd.processMessages() )
	{
		cxt->clearRenderTarget( t, zpColor4f( 1.f, 0, 0, 1.f ) );

		cxt->processCommands();
		re->present();
	}

	zpRenderingFactory::destroyRenderingEngine();
	wnd.destroy();

	return 0;
}
