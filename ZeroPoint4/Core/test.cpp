#include "zpCore.h"
#include <stdio.h>

int main() {

	zpString empty;
	zpString small( "orld" );
	zpString* strprt = new zpString( "abcd" );
	zpString str( "12345678" );
	zpString cpy( str );
	zpString ss( small, 2, 1 );
	zpString ffss( "abcdefghijkl", 6, 3 );
	zpString mv( zpString( "testing the move" ) );

	//mv = ss;

	zp_char a = strprt->charAt( 1 );

	auto ind = str.endsWith( small );

	small.setCharAt( 1, 'z' );

	zpString sub = str.substring( 3, -1 );

	zp_printfln( "String sub: %s", sub.getChars() );
	zp_printfln( "String mv : %s", mv.getChars() );

	zpMatrix4 mat;
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
	go1.addGameObjectComponent( new zpGameObjectComponent() );
	go1.addGameObjectComponent( new zpGameObjectComponent() );

	go2.addGameObjectComponent( new zpGameObjectComponent() );
	go2.addGameObjectComponent( new zpGameObjectComponent() );
	go2.addGameObjectComponent( new zpGameObjectComponent() );

	go3.addGameObjectComponent( new zpGameObjectComponent() );
	go3.addGameObjectComponent( new zpGameObjectComponent() );

	go4.addGameObjectComponent( new zpGameObjectComponent() );
	*/
	go1.getComponents()->foreach([](zpGameObjectComponent* goc){
		zp_printfln(" GOC: %p Ref: %d", goc, goc->getReferenceCount() );
	});
	go1.getChildren()->foreach([](zpGameObject* go){
		zp_printfln("GO: %p Ref: %d", go, go->getReferenceCount() );
		go->getComponents()->foreach([](zpGameObjectComponent* goc){
			zp_printfln("  GOC: %p Ref: %d", goc, goc->getReferenceCount() );
		});


		go->getChildren()->foreach([](zpGameObject* go){
			zp_printfln("\tGO: %p Ref: %d", go, go->getReferenceCount());
			go->getComponents()->foreach([](zpGameObjectComponent* goc){
				zp_printfln("\t  GOC: %p Ref: %d", goc, goc->getReferenceCount() );
			});

			go->update();
			go->getChildren()->foreach([](zpGameObject* go){
				zp_printfln("\t\tGO: %p Ref: %d", go, go->getReferenceCount() );
				go->getComponents()->foreach([](zpGameObjectComponent* goc){
					zp_printfln("\t\t  GOC: %p Ref: %d", goc, goc->getReferenceCount() );
				});

				go->update();
			});
		});
	});

	return 0;
}