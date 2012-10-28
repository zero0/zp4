#include "Blackjack.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#define DEFAULT_BLACKJACK_CONFIG	"blackjack.config"

zpGame* ProcessConfig() {
	zpProperties properties( DEFAULT_BLACKJACK_CONFIG );
	ZP_ASSERT( properties.hasProperty( "game.file" ), "Game file not defined" );
	const zpString& gameFile = properties[ "game.file" ];

	if( properties[ "console.enabled" ] == "true" ) {
		zpConsole::getInstance()->create();
	}

	zpXmlNode gameRoot;
	zpXmlParser gameParser;
	gameParser.setRootNode( &gameRoot, false );
	gameParser.parseFile( gameFile, true );

	zpProperties gameOverrides( properties.getSubProperties( "game.overrides" ) );

	zpXmlSerializedInput gameIn( gameParser.getRootNode(), gameOverrides );
	zpSerializable* gameSerial = ZP_NULL;

	gameIn.readSerializable( &gameSerial, ZP_NULL );
	ZP_ASSERT( gameSerial != ZP_NULL, "Failed to parse game file %s", gameFile.c_str() );
	
	zpGame* game = (zpGame*)gameSerial;
	game->create();

	const zpString& worldPrefab = properties[ "world.prefab" ];
	
	zpSerializable* world = zpPrefabs::getInstance()->loadPrefab( worldPrefab );
	ZP_ASSERT( world != ZP_NULL, "Failed to load world prefab %s", worldPrefab.c_str() );

	game->addWorld((zpWorld*)world, true );

	return game;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	ZP_REGISTER_SERIALIZABLES( zpCore );
	ZP_REGISTER_SERIALIZABLES( zpContent );
	ZP_REGISTER_SERIALIZABLES( zpAudio );
	ZP_REGISTER_SERIALIZABLES( zpInput );
	ZP_REGISTER_SERIALIZABLES( zpScripting );
	ZP_REGISTER_SERIALIZABLES( zpRendering );

	zpGame* game = ProcessConfig();

	game->process();

	game->destroy();

	ZP_SAFE_DELETE( game );

	return 0;
}
