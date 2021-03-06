
#include "Core\zpCore.h"


int main()
{
	return 0;
}

#pragma region Old Test Code
#if 0

#include "Core\zpCore.h"
#include "Rendering\zpRendering.h"
#include "Content\zpContent.h"
#include "Scripting\zpScripting.h"
#include "Audio\zpAudio.h"
#include "Input\zpInput.h"
#include "Physics\zpPhysics.h"
#include "Common\zpCommon.h"
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

void on_loaded( const zpString& filename, zp_bool loaded, zp_uint ){
	zp_printfln( "File '%s' %s", filename.c_str(), loaded ? "loaded" : "failed to load" );
}
void on_finish() {
	zp_printfln( "finished loading" );
}

struct TestRenderable : public zpRenderable {
	zpBuffer* buff;
	zpBuffer* buff2;
	zpBuffer* origBuff;
	zpBuffer* cameraBuffer;

	zpCamera camera; 

	zpResourceInstance<zpShaderResource> sr;
	zpResourceInstance<zpShaderResource> srtex;

	zpRenderingEngine* engine;
	zpResourceInstance<zpTextureResource> tex;
	zpSamplerState* state;
	zpRasterState* raster;
	zpStaticMeshResource* mesh;
	zpStaticMeshRenderingComponent smrc;
	zpUIRenderingComponent uirc;
	zp_int time;
	zp_uint frames;
	zp_uint avgFrameTime;
	zpRenderingManager* rendering;
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

		sr = cm->createInstanceOfResource<zpShaderResource>( "simple_shader" );
		srtex = cm->createInstanceOfResource<zpShaderResource>( "tex_norm_shader" );
		tex = cm->createInstanceOfResource<zpTextureResource>( "tex" );
		//mesh = cm->createInstanceOfResource<zpStaticMeshResource>( "cube" );

		frames = 0;
		time = 0;
		avgFrameTime = 0;

		engine->getImmediateRenderingContext()->bindRenderTargetAndDepthBuffer();
		engine->setVSyncEnabled( false );

		zpSamplerStateDesc samplerDesc;
		state = engine->createSamplerState( samplerDesc );

		zpRasterStateDesc rasterDesc;
		//rasterDesc.fillMode = ZP_FILL_MODE_WIREFRAME;
		//rasterDesc.cullMode = ZP_CULL_MODE_NONE;
		raster = engine->createRasterState( rasterDesc );

		camera.setProjectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE );
		camera.setNearFar( 1, 1000 );
		camera.setPosition( zpVector4f( 5, 6, 7 ) );
		camera.setLookAt( zpVector4f() );
		camera.setUp( zpVector4f( 0, 1, 0 ) );
		camera.setAspectRatio( 800.f / 600.f );
		camera.setFovy( 45.f );
		camera.update();

		cm->getGame()->getCurrentWorld()->getRootGameObject()->addComponent( &smrc );
		smrc.create();

		zpCameraBufferData cbd;
		cbd.viewProjection = camera.getViewProjection();
		cbd.invViewProjection = camera.getInvViewProjection();

		rendering = smrc.getGameManagerOfType<zpRenderingManager>();
		rendering->setCamera( &camera );
#endif
	}
	void render() {
#if TEST_RENDERING
		
		zpRenderingContext* i = engine->getImmediateRenderingContext();
		zp_long start = zpTime::getInstance()->getTime();

		zpColor4f c(.23f, .15f, .88f, 1.f );
		i->setViewport( zpViewport( 800, 600 ) );
		i->bindRenderTargetAndDepthBuffer();
		i->clearRenderTarget( &c );
		i->clearDepthStencilBuffer( 1.0f, 0 );

		i->setSamplerState( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, state );
		i->setRasterState( raster );


		zpVector4f pos( 0, 2.f * zpTime::getInstance()->getDeltaSeconds() * zpTime::getInstance()->getInterpolation(), 0 );
		pos.add4( camera.getPosition() );
		//pos = camera.getPosition().lerp4( pos, zp_real_from_float( zpTime::getInstance()->getInterpolation() ) );
		camera.setPosition( pos );
		
		camera.update();
		//rendering->getGlobalBuffer( ZP_RENDERING_GLOBAL_BUFFER_CAMERA )->update( camera.getCameraBufferData() );
		i->setBuffer( rendering->getGlobalBuffer( ZP_RENDERING_GLOBAL_BUFFER_CAMERA ), 4 );

		if( sr ) {
			i->setTopology( ZP_TOPOLOGY_TRIANGLE_STRIP );
		
			i->setBuffer( buff );
			i->setShader( &sr );
			i->draw( 4 );
		

			i->setTopology( ZP_TOPOLOGY_LINE_LIST );
			i->setBuffer( origBuff );
			i->draw( 6 );
		}

		if( srtex ) {
			i->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, tex.getResource()->getTexture() );
			i->setShader( &srtex );
			smrc.render();
		}
		
		if( srtex ) {
			i->setTopology( ZP_TOPOLOGY_TRIANGLE_STRIP );

			i->setBuffer( buff2 );
			i->setShader( &srtex );
			i->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, tex.getResource()->getTexture() );
			i->draw( 4 );
		}
		/*
			i->setTopology( ZP_TOPOLOGY_TRIANGLE_LIST );
		
			//i->bindBuffer( mesh->getVertexBuffer() );
			//i->draw( mesh->getNumVertices() );
		}
		*/


		zp_long end = zpTime::getInstance()->getTime();
		end -= start;
		engine->present();
		/*
		++frames;
		time += zpTime::getInstance()->getDeltaTime();
		avgFrameTime += end;
		if( time > 1000000 ) {
			
			zp_printfcln( ZP_CC( ZP_CC_GREEN, ZP_CC_WHITE ), "FPS: %d\tavgFT(ms): %.3f", frames, (zp_float)avgFrameTime / frames / 1000.f );
			time = 0;
			frames = 0;
			avgFrameTime = 0;
		}
		*/
#endif
	}
};

void rendering_test_main() {
	zpConsole::getInstance()->create();

	zpGameObject ro;
	zpWorld world;
	world.setRootGameObject( &ro );

	zpEditorCameraComponent ecc;
	ro.addComponent( &ecc );
	ro.addComponent( new zpEditorCameraComponent );

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
	cm.registerFileExtension( "fnt", &rrc );
	cm.onResourceLoaded() += zpCreateFunctionDelegate( on_loaded );
	cm.onAllResourcesLoaded() += zpCreateFunctionDelegate( on_finish );

	zpInputManager im;

	zpRenderingManager rm;

	zpPhysicsManager pm;

	game.addGameManager( &rm );
	game.addGameManager( &cm );
	game.addGameManager( &im );
	game.addGameManager( &pm );

	game.create();

	zp_bool loaded = false;
	loaded = cm.loadResource( "simple.shader", "simple_shader" );
	loaded = cm.loadResource( "tex_norm.shader", "tex_norm_shader" );
	loaded = cm.loadResource( "test.png", "tex" );
	loaded = cm.loadResource( "cube.obj", "cube" );
	loaded = cm.loadResource( "verdana.fnt", "verdana" );

	//zpLog::getInstance()->disableLogLevel( ZP_LOG_LEVEL_DEBUG );
	//zpLogOutput& d = zpLog::debug();
	//zpLog::debug() << "hello" << zpLogOptions::endl << zpLogOptions::red << "debug log" << zpLogOptions::reset << " Log";

	struct KeyboardListener : public zpKeyboardListener {
		zpContentManager* content;

		void onKeyDown( zpKeyCode key ) { zp_printfln( "Key Down %d", key ); };
		void onKeyRepeat( zpKeyCode key ) { zp_printfln( "Key Repeat %d", key ); };
		void onKeyUp( zpKeyCode key ) {
			if( key == ZP_KEY_CODE_R ) { 
				content->reloadResource( "simple_shader" ); 
			} else if( key == ZP_KEY_CODE_A ) {
				content->unloadAllResources();
			}
			zp_printfln( "Key Up %d", key );
		};
	};
	struct MouseListener : public zpMouseListener {
		void onMouseMove( const zpVector2i& loc ) { zp_printfln( "Mouse Move %d %d", loc.getX(), loc.getY() ); };
		void onMouseChange( const zpVector2i& loc ) { zp_printfln( "Mouse Change %d %d", loc.getX(), loc.getY() ); };
		void onMouseButtonUp( zpMouseButton button ) { zp_printfln( "Mouse Up %d", button ); };
	};
	struct ControllerListener : public zpControllerListener {
		void onButtonDown( zpControllerButton button ) { zp_printfln( "Button Down %d", button ); }
		void onLeftStickMove( const zpVector2i& move ) { zp_printfln( "Left Stick %d %d", move.getX(), move.getY() ); }
		void onRightTriggerPull( zp_ushort amount ) { zp_printfln( "Right Trigger %d", amount ); }

		void onControllerDisconnected() {};
		void onControllerReconnected() {};
	};

	KeyboardListener kl;
	kl.content = &cm;

	//im.getKeyboard()->addListener( &kl );
	//im.getMouse()->addListener( new MouseListener );
	//im.getController( ZP_CONTROLLER_1 )->addListener( new ControllerListener );

	game.addWorld( &world, true );

	TestRenderable tr;
	tr.start( &cm );
	game.setRenderable( &tr );

	//while( wnd.processMessages() ) {
		game.process();
	//s}
}

void scripting_test_main() {
	zpWindow wnd;
	wnd.setTitle( "ZeroPoint4 Window" );
	wnd.setPosition( zpVector2i( 50, 50 ) );
	wnd.setScreenSize( zpVector2i( 800, 600 ) );
	wnd.create();

	zpGame game;
	zpWorld world;
	zpGameObject root;
	zpGameObject obj;

	zpScriptingManager sm;

	zpScriptingResourceCreator src;
	src.setRootDirectory( "scripts/" );

	zpContentManager content;
	content.setRootDirectory( "Assets/" );
	content.registerFileExtension( "as", &src );
	
	zpScriptingComponent sc;
	sc.setScriptAlias( "TestScript" );

	zpScriptingComponent sc2;
	sc2.setScriptAlias( "TestScript" );

	root.addComponent( &sc );
	obj.addComponent( &sc2 );

	
	world.setRootGameObject( &root );
	world.setName( "world" );

	root.addChildGameObject( &obj );

	game.addGameManager( &content );
	game.addGameManager( &sm );

	game.create();
	zp_bool isLoaded = content.loadResource( "TestScript.as", "TestScript" );

	game.addWorld( &world, true );

	zpStringBuffer buff;
	zpXmlParser xml;
	zpXmlNode rootNode;
	rootNode.type = ZP_XML_NODE_TYPE_DOCUMENT;
	xml.setRootNode( &rootNode, false );
	zpXmlSerializedOutput out( xml.getRootNode() );
	
	world.serialize( &out );

	xml.writeToBuffer( xml.getRootNode(), buff );

	while( wnd.processMessages() ) game.process();
}

int fffff(int, float, void*, int) { return 1010; };
void asdf( int a ) { zp_printfln( "asdf: %d", a ); }
void qwerty( int a ) { zp_printfln( "asdf: %d", a * 2 ); }
void zxcvb( int a ) { zp_printfln( "asdf: %d", a + 1 ); }

void printNode( const zpXmlNode* node, zp_int tab ) {
	for( zp_int t = tab; t --> 0; ) printf( "-" );

	printf( "%s '%s' \n", node->name.c_str(), node->value.c_str() );

	node->children.foreach( [ &tab ]( const zpXmlNode* n ) {
		printNode( n, tab + 1 );
	} );
}

int main() {
	rendering_test_main();
	//scripting_test_main();

	//zpXmlNode* node = zpXmlParser::parseFile( "Assets/test.xml" );

	//printNode( node, 0 );

	return 0;
	
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

#endif
#pragma endregion
