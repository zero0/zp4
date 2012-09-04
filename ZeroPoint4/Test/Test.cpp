#include "Core\zpCore.h"
#include "Rendering\zpRendering.h"
#include "Content\zpContent.h"
#include "Scripting\zpScripting.h"
#include "Audio\zpAudio.h"

#include <stdio.h>

#define TEST_NONE		0
#define TEST_DX			1
#define TEST_OPENGL		2
#define TEST_RENDERING	TEST_DX

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
	
	zpMatrix4f mat, ident;
	zpVector4f vec;

	//zpVector4f* vv = new zpVector4f();
	//zpMatrix4f* mm = new zpMatrix4f();

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
	
	getchar();
}

void rendering_test_main() {
	zpConsole::getInstance()->create();

	zpGameObject ro;

	zp_printfcln( ZP_CC( ZP_CC_RED, ZP_CC_LIGHT_BLUE ), "Size: %d", sizeof( zpGameObject ) );

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
		zp_printfln( "%s : %s", key.c_str(), value.c_str() );
	} );


	zpGame game;
	game.setWindow( &wnd );

	zpRenderingResourceCreator rrc;
	rrc.setRootDirectory( "rendering/" );

	zpLog::debug() << "Hello Debug" << zpLog::endl;

	zpContentManager cm;
	cm.setRootDirectory( "Assets/" );
	cm.registerFileExtension( "shader", &rrc );
	cm.registerFileExtension( "png", &rrc );
	cm.registerFileExtension( "obj", &rrc );

	zpRenderingManager rm;
#if TEST_RENDERING == TEST_DX
	rm.setRenderingEngineType( ZP_RENDERING_ENGINE_DX );
#elif TEST_RENDERING == TEST_OPENGL
	rm.setRenderingEngineType( ZP_RENDERING_ENGINE_OPENGL );
#endif

	game.addGameManager( &rm );
	game.addGameManager( &cm );

	game.create();

	cm.loadResource( "simple.shader", "simple_shader" );
	cm.loadResource( "tex_norm.shader", "tex_norm_shader" );
	cm.loadResource( "test.png", "tex" );
	cm.loadResource( "cube.obj", "cube" );

	//zpLog::getInstance()->disableLogLevel( ZP_LOG_LEVEL_DEBUG );
	//zpLogOutput& d = zpLog::debug();
	//zpLog::debug() << "hello" << zpLogOptions::endl << zpLogOptions::red << "debug log" << zpLogOptions::reset << " Log";

	struct TestRenderable : public zpRenderable {
		zpBuffer* buff;
		zpBuffer* buff2;
		zpBuffer* origBuff;
		zpBuffer* cameraBuffer;

		zpCamera camera;

		zpShaderResource* sr;
		zpShaderResource* srtex;
		zpRenderingEngine* engine;
		zpTextureResource* tex;
		zpSamplerState* state;
		zpRasterState* raster;
		zpStaticMeshResource* mesh;
		zp_float time;
		zp_uint frames;
		void start( zpContentManager* cm ) {
			engine = zpRenderingFactory::getRenderingEngine();
#if TEST_RENDERING
			buff = engine->createBuffer();
			buff2 = engine->createBuffer();
			origBuff = engine->createBuffer();

			const zp_float f = .5f;
			const zp_float z = 2.f;
			const zp_float o = 5.f;

			zpVertexPositionColor sv[] = {
				{ zpVector4f(-f,-f, z, 1 ), zpColor4f( 1, 0, 0, 1 ) },
				{ zpVector4f(-f, f, z, 1 ), zpColor4f( 0, 1, 0, 1 ) },
				{ zpVector4f( f,-f, z, 1 ), zpColor4f( 0, 0, 1, 1 ) },
				{ zpVector4f( f, f, z, 1 ), zpColor4f( 1, 0, 1, 1 ) },
			};
			buff->create( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, sv );

			zpVertexPositionNormalTexture pnt[] = {
				{ zpVector4f(-o,-o, -z, 1 ), zpVector4f( 0, 0, 0, 1 ), zpVector2f( 0, 2 )  },
				{ zpVector4f(-o, o, -z, 1 ), zpVector4f( 0, 0, 0, 1 ), zpVector2f( 0, 0 )  },
				{ zpVector4f( o,-o, -z, 1 ), zpVector4f( 0, 0, 0, 1 ), zpVector2f( 2, 2 )  },
				{ zpVector4f( o, o, -z, 1 ), zpVector4f( 0, 0, 0, 1 ), zpVector2f( 2, 0 )  }
			};
			buff2->create( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, pnt );

			zpVertexPositionColor orig[] = {
				{ zpVector4f( 0, 0, 0, 1 ), zpColor4f( 1, 0, 0, 1 ) },
				{ zpVector4f( o, 0, 0, 1 ), zpColor4f( 1, 0, 0, 1 ) },

				{ zpVector4f( 0, 0, 0, 1 ), zpColor4f( 0, 1, 0, 1 ) },
				{ zpVector4f( 0, o, 0, 1 ), zpColor4f( 0, 1, 0, 1 ) },

				{ zpVector4f( 0, 0, 0, 1 ), zpColor4f( 0, 0, 1, 1 ) },
				{ zpVector4f( 0, 0, o, 1 ), zpColor4f( 0, 0, 1, 1 ) },
			};
			origBuff->create( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, orig );

			sr = cm->getResourceOfType<zpShaderResource>( "simple_shader" );
			srtex = cm->getResourceOfType<zpShaderResource>( "tex_norm_shader" );
			tex = cm->getResourceOfType<zpTextureResource>( "tex" );
			mesh = cm->getResourceOfType<zpStaticMeshResource>( "cube" );

			frames = 0;
			time = 0;

			engine->getImmediateRenderingContext()->bindRenderTargetAndDepthBuffer();
			engine->setVSyncEnabled( true );

			zpSamplerStateDesc samplerDesc;
			state = engine->createSamplerState( samplerDesc );

			zpRasterStateDesc rasterDesc;
			//rasterDesc.fillMode = ZP_FILL_MODE_WIREFRAME;
			//rasterDesc.cullMode = ZP_CULL_MODE_NONE;
			raster = engine->createRasterState( rasterDesc );

			camera.setProjectionType( ZP_CAMERA_PROJECTION_ORTHO );
			camera.setNearFar( 1, 1000 );
			camera.set( zpVector4f( 0, 0, 0 ), zpVector4f( 0, 0, 0 ), zpVector4f( 0, 1, 0 ) );
			camera.setAspectRatio( 800.f / 600.f );
			camera.setFovy( 45.f );
			camera.update();

			cameraBuffer = engine->createBuffer();

			struct cb {
				zpMatrix4f v, p, vp, w;
			};
			cb cambuf[1];
			cambuf[0].v.lookAt( zpVector4f( -5, 5, 5 ), zpVector4f( 0, 0, 0 ), zpVector4f( 0, 1, 0 ) );// = camera.getView();
			cambuf[0].p.ortho( 800, 600, 1, 1000 );// = camera.getProjection();
			//cambuf[0].p.perspective( 45.f, 4.f / 3.f, 1, 1000 );
			//cambuf[0].vp = camera.getViewProjection();
			cambuf[0].w.scale( 1 );

			cameraBuffer->create( ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, cambuf );
			//cameraBuffer->create( ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, 3, sizeof( zpMatrix4f ), (void*)(const zp_float*)camera.getViewProjection() );
			
#endif
		}
		void render() {
#if TEST_RENDERING
			zp_long start = zpTime::getInstance()->getTime();

			zpRenderingContext* i = engine->getImmediateRenderingContext();

			zpColor4f c(.23f, .15f, .88f, 1.f );
			i->setViewport( zpViewport( 800, 600 ) );
			i->bindRenderTargetAndDepthBuffer();
			i->clearRenderTarget( &c );
			i->clearDepthStencilBuffer( 1.0f, 0 );

			i->setSamplerState( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, state );
			i->setRasterState( raster );

			i->bindBuffer( cameraBuffer, 0 );

			i->setTopology( ZP_TOPOLOGY_TRIANGLE_STRIP );
			
			i->bindBuffer( buff );
			i->bindShader( sr );
			i->draw( 4 );

			i->setTopology( ZP_TOPOLOGY_LINE_LIST );
			i->bindBuffer( origBuff );
			i->draw( 6 );

			i->setTopology( ZP_TOPOLOGY_TRIANGLE_STRIP );

			i->bindBuffer( buff2 );
			i->bindShader( srtex );
			i->bindTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, tex->getTexture() );
			i->draw( 4 );
			
			i->setTopology( ZP_TOPOLOGY_TRIANGLE_LIST );

			i->bindBuffer( mesh->getVertexBuffer() );
			i->draw( mesh->getNumVertices() );
			
			engine->present();
			zp_long end = zpTime::getInstance()->getTime();

			++frames;
			time += zpTime::getInstance()->getDeltaSeconds();
			if( time > 1.f ) {
				end -= start;
				zp_printfcln( ZP_CC( ZP_CC_GREEN, ZP_CC_WHITE ), "FPS: %d\tFT(ms): %.1f", frames, ( end / 1000.f ) );
				time = 0;
				frames = 0;
			}
#endif
		}
	};
	TestRenderable* tr = new TestRenderable;
	tr->start( &cm );

	game.setRenderable( tr );
	
	while( wnd.processMessages() ) {
		game.process();
	}
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

int fffff(int, float, void*, int) { return 1010; };
void asdf( int a ) { zp_printfln( "asdf: %d", a ); }
void qwerty( int a ) { zp_printfln( "asdf: %d", a * 2 ); }
void zxcvb( int a ) { zp_printfln( "asdf: %d", a + 1 ); }

int main() {
	//zpDelegate<int ()> dd = zpCreateFunctionDelegate( fffff );
	
	//int ab = dd();

	zpVector2f vec2;
	zpVector4f vec4;
	zpVector2i vec2i;
	vec2.set( 10, 12 );

	//zpDelegate<void ()> aa = zpCreateMemberDelegate( &zpVector2f::zero, &vec2 );

	//aa();
	zpDelegate<int( int, float, void*, int )> vi = zpCreateFunctionDelegate( fffff );
	zpDelegate<void(  )> ss = zpCreateMemberDelegate( &zpVector2f::zero, &vec2 );
	zpCreateMemberDelegate( &zpVector4f::setX, &vec4 );

	//zpDelegateFunctionFactory4<int, int, float, void*, int>::Bind<fffff>();

	zpString str( "This string" );

	zpDelegate<zp_uint ()> ssss = zpCreateMemberDelegate( &zpString::size, &str );

	zpDelegateEvent<void( int )> de;
	de += zpCreateFunctionDelegate( asdf );
	de += zpCreateFunctionDelegate( qwerty );
	de += zpCreateFunctionDelegate( zxcvb );
	de += zpCreateMemberDelegate( &zpVector2i::setX, &vec2i );

	de( 10 );

	int vv = vi( 1, 2.f, 0, 0 );
	ss();
	zp_uint strSize = ssss();

	zpTween<zpVector4f> anim;
	anim.queue( 2.f, zpVector4f(), ZP_TWEEN_EASING_SIN_IN );
	anim.update( .1f );
	anim.getCurrentValue();
	anim.isComplete();

	//zpArray<zpDelegate<void ()>> dels( 4 );
	//dels[0] = zpDelegateFunctionFactory<void>::Bind<core_test_main>();
	//dels[1] = zpDelegateFunctionFactory<void>::Bind<scripting_test_main>();
	//dels[2] = aa;
	//dels[3] = zpCreateMemberDelegate( &zpVector2f::zero, &vec2 );

	//core_test_main();
	rendering_test_main();
	//scripting_test_main();

	return 0;
}
