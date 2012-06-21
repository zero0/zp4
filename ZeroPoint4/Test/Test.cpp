#include "Core\zpCore.h"
#include "Rendering\zpRendering.h"
#include "Content\zpContent.h"
#include "Scripting\zpScripting.h"

#include <stdio.h>

void core_test_main() {
	zp_printfln( "Size: %d", sizeof( zpString ) );

	zpString empty;
	zpString small( "WORLD wide Web Is the InterNETS" );
	zpString* strprt = new zpString( "abcd" );
	zpString str( "1234567890123456789012345678901234567890" );
	zpString cpy( str );
	zpString ss( small, 2, 1 );
	zpString ffss( "abcdefghijkl", 6, 3 );
	zpString mv( zpString( "testing the move" ) );

	mv = "assigned";

	zp_char a = strprt->charAt( 1 );

	auto ind = str.indexOf( small );

	small.setCharAt( 1, 'z' );

	zpString sub = str.substring( 3, -1 );

	zpString up( mv.toUpper() );
	zpString lower = small.toLower();

	zp_printfln( "String sub: %s", sub.getChars() );
	zp_printfln( "String mv : %s", mv.getChars() );
	zp_printfln( "String small: %s", small.getChars() );
	zp_printfln( "String lower: %s", lower.getChars() );
	
	zpMatrix4 mat, ident;
	zpVector4 vec;

	//zpVector4* vv = new zpVector4();
	//zpMatrix4* mm = new zpMatrix4();

	zpGameObject go1;
	zpGameObject go2;
	zpGameObject go3;
	zpGameObject go4;

	zpGameObject* root1 = &go1;
	zpGameObject* root2 = &go2;
	zpGameObject* root3 = &go3;
	zpGameObject* root4 = &go4;
	//zpGameObject* root5 = new zpGameObject();

	go1.addChildGameObject( &go2 );
	go2.addChildGameObject( &go3 );
	go2.addChildGameObject( &go4 );

	go1.addChildGameObject( new zpGameObject() );
	go1.addChildGameObject( new zpGameObject() );
	go1.addChildGameObject( new zpGameObject() );
	go1.addChildGameObject( new zpGameObject() );

	go2.addChildGameObject( new zpGameObject() );
	go2.addChildGameObject( new zpGameObject() );
	go2.addChildGameObject( new zpGameObject() );

	go3.addChildGameObject( new zpGameObject() );
	go3.addChildGameObject( new zpGameObject() );

	go4.addChildGameObject( new zpGameObject() );
	go4.addChildGameObject( new zpGameObject() );
	/*
	go1.addComponent( new zpComponent() );
	go1.addComponent( new zpComponent() );

	go2.addComponent( new zpComponent() );
	go2.addComponent( new zpComponent() );
	go2.addComponent( new zpComponent() );

	go3.addComponent( new zpComponent() );
	go3.addComponent( new zpComponent() );

	go4.addComponent( new zpComponent() );
	*/
	go1.getComponents()->foreach([](zpComponent* goc){
		zp_printfln(" GOC: %p Ref: %d", goc, goc->getReferenceCount() );
	});
	go1.getChildGameObjects()->foreach([](zpGameObject* go){
		zp_printfln("GO: %p Ref: %d", go, go->getReferenceCount() );
		go->getComponents()->foreach([](zpComponent* goc){
			zp_printfln("  GOC: %p Ref: %d", goc, goc->getReferenceCount() );
		});


		go->getChildGameObjects()->foreach([](zpGameObject* go){
			zp_printfln("\tGO: %p Ref: %d", go, go->getReferenceCount());
			go->getComponents()->foreach([](zpComponent* goc){
				zp_printfln("\t  GOC: %p Ref: %d", goc, goc->getReferenceCount() );
			});

			go->update();
			go->getChildGameObjects()->foreach([](zpGameObject* go){
				zp_printfln("\t\tGO: %p Ref: %d", go, go->getReferenceCount() );
				go->getComponents()->foreach([](zpComponent* goc){
					zp_printfln("\t\t  GOC: %p Ref: %d", goc, goc->getReferenceCount() );
				});

				go->update();
			});
		});
	});
	zpComponent* gzz = go1.getComponentOfType<zpComponent>();
	
	zpMessage msg1( 22, 4444.424243f, ZP_NULL );
	zpMessage msg2( 43, zpString( "Testing String" ), ZP_NULL );

	float d1 = msg1.getMessageData<float>();
	zpString d2 = msg2.getMessageData<zpString>();

	zpLinkedList<zp_int> intList;
	intList.pushBack( 44 );
	intList.pushFront( 12 );
	intList.foreach( []( const zp_int& val ){ zp_printfln( "List: %d", val ); } );
	//intList.popBack();
	zp_bool cont = intList.contains( 43 );

	zp_printfln( "" );

	zpArrayList<float> rrr( 3 );
	rrr.pushBack( 1 );
	rrr.pushBack( 2 );
	rrr.pushBack( 3 );
	rrr.pushBack( 4 );
	float rerwrwe[3] = { 5, 6, 7 };
	rrr.pushFront( rerwrwe );
	rrr.foreachIndexed( [ &rrr ]( int i, const float& val ) {
		zp_printfln( "%d %cArr: %f", i, ( i == rrr.size() ? '>' : ' ' ), val );
	});
	
	zp_printfln( "" );

	rrr.insert( 0, 9 );
	//rrr.popFront();
	//rrr.popBack( 3 );
	rrr.foreachIndexed( [ &rrr ]( int i, const float& val ) {
		zp_printfln( "%d %cArr: %f", i, ( i == rrr.size() ? '>' : ' ' ), val );
	});

	float fsfsfs;
	rrr.remove( 5, &fsfsfs );
	rrr[5] = 10;
	
	rrr.shrinkToFit(1);

	zp_printfln( "" );
	rrr.foreachIndexed( [ &rrr ]( int i, const float& val ) {
		zp_printfln( "%d %cArr: %f", i, ( i == rrr.size() ? '>' : ' ' ), val );
	});

	zpMap<int, float> mmm;
	mmm.put( 10, 10.2323f );
	mmm.put( 5, 155.1f );
	mmm.put( 59, 99.1f );
	mmm.put( 10, 987654.f );
	float ytryr = mmm[ 59 ];
	ytryr = mmm[ 5 ];
	mmm[ 23 ] = 12123414.f;
	//ytryr = mmm.get( 30 );

	mmm.remove( 59, &ytryr );

	mmm.foreach( []( int key, float val ) {
		zp_printfln( "%d => %f", key, val );
	});

	zpWindow wnd;
	wnd.setTitle( "ZeroPoint4 Window" );
	wnd.setPosition( zpVector2i( 50, 50 ) );
	wnd.create();
	wnd.run();
	
	getchar();
}

void rendering_test_main() {
	zp_printfln( "Size: %d", sizeof( zpString ) );
	zpWindow wnd;
	wnd.setTitle( "ZeroPoint4 Window" );
	wnd.setPosition( zpVector2i( 50, 50 ) );
	wnd.setScreenSize( zpVector2i( 800, 600 ) );
	wnd.create();
	
	zpProperties prop;
	prop[ "a" ] = "b";
	prop.setInt( "one", 1 );
	prop.setFloat( "two.point.three", 2.3f );

	prop.save( "test.properties" );


	zpProperties prop2;
	prop2.load( "test.properties" );
	prop2.foreach( []( const zpString& key, const zpString& value ) {
		zp_printfln( "%s = %s", key.c_str(), value.c_str() );
	} );
	zpGame game;
	game.setWindow( &wnd );

	zpRenderingManager rm;
	rm.setRenderingEngineType( ZP_RENDERING_ENGINE_DX );
	
	game.addGameManager( &rm );

	game.create();

	wnd.run();
}

void scripting_test_main() {
	zpGame game;
	zpWorld w;
	zpGameObject root;

	zpScriptingManager sm;

	zpScriptingResourceCreator src;
	src.setRootDirectory( "scripts/" );

	zpContentManager content;
	content.setRootDirectory( "Assets/" );
	content.registerFileExtension( "as", &src );
	
	zpScriptingComponent sc;
	sc.setScriptAlias( "TestScript" );

	root.addComponent( &sc );

	w.setRootGameObject( &root );

	game.addGameManager( &content );
	game.addGameManager( &sm );

	game.create();
	zp_bool isLoaded = content.loadResource( "TestScript.as", "TestScript" );

	game.addWorld( &w, true );
	
	
}

int main() {
	//core_test_main();
	//rendering_test_main();
	scripting_test_main();

	return 0;
}
