#include "Blackjack.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#define DEFAULT_BLACKJACK_CONFIG	"blackjack.config"

void ProcessConfig( zpGame& game ) {
	zpProperties properties( DEFAULT_BLACKJACK_CONFIG );
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
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	//ZP_REGISTER_SERIALIZABLES( zpCore );
	//ZP_REGISTER_SERIALIZABLES( zpContent );
	//ZP_REGISTER_SERIALIZABLES( zpAudio );
	//ZP_REGISTER_SERIALIZABLES( zpInput );
	//ZP_REGISTER_SERIALIZABLES( zpScripting );
	//ZP_REGISTER_SERIALIZABLES( zpRendering );
	//ZP_REGISTER_SERIALIZABLES( zpPhysics );
	//
	//zpGame game;
	//
	//ProcessConfig( game );
	//
	//game.process();
	//
	//game.destroy();

	return 0;
}
