#include "zpCore.h"
#include <stdio.h>

int main() {
	//zpGameObject go;
	zpVector4 vv( 4, 5, 6 );
	//vv.add( zpVector4( 1, 0, 3, 0 ) );
	//vv.mul( zpVector4( 2, 4, 6, 8 ) );
	zp_float dot = vv.dot3( zpVector4( 1, 2, 3 ) );

	zpGameObject go1;
	zpGameObject go2;
	zpGameObject go3;
	zpGameObject go4;

	zpGameObject* root1 = &go1;
	zpGameObject* root2 = &go2;
	zpGameObject* root3 = &go3;
	zpGameObject* root4 = &go4;

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

	go1.addGameObjectComponent( new zpGameObjectComponent() );
	go1.addGameObjectComponent( new zpGameObjectComponent() );

	go2.addGameObjectComponent( new zpGameObjectComponent() );
	go2.addGameObjectComponent( new zpGameObjectComponent() );
	go2.addGameObjectComponent( new zpGameObjectComponent() );

	go3.addGameObjectComponent( new zpGameObjectComponent() );
	go3.addGameObjectComponent( new zpGameObjectComponent() );

	go4.addGameObjectComponent( new zpGameObjectComponent() );

	go1.getComponents()->foreach([](zpGameObjectComponent* goc){
		printf_s(" GOC: %p\n", goc);
	});
	go1.getChildren()->foreach([](zpGameObject* go){
		printf_s("GO: %p\n", go);
		go->getComponents()->foreach([](zpGameObjectComponent* goc){
			printf_s("  GOC: %p\n", goc);
		});


		go->getChildren()->foreach([](zpGameObject* go){
			printf_s("\tGO: %p\n", go);
			go->getComponents()->foreach([](zpGameObjectComponent* goc){
				printf_s("\t  GOC: %p\n", goc);
			});

			go->update();
			go->getChildren()->foreach([](zpGameObject* go){
				printf_s("\t\tGO: %p\n", go);
				go->getComponents()->foreach([](zpGameObjectComponent* goc){
					printf_s("\t\t  GOC: %p\n", goc);
				});

				go->update();
			});
		});
	});

	return 0;
}