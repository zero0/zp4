#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include "Blackjack.h"


#define BLACKJACK_CONFIG	"BlackjackConfig.jsonb"
#define BLACKJACK_OPTIONS	"BlackjackOptions.jsonb"

struct Character
{
	zp_uint num;
};

void zpProtoDBCreateCharacter( void* data, zp_uint stride, const zpBison::Value& def )
{
	Character* c = (Character*)data;
	c->num = def[ "num" ].asInt();
}

class bProtoDBPhase : public zpApplicationPhase
{
public:
	bProtoDBPhase() {}
	~bProtoDBPhase() {}

	const zp_char* getPhaseName() const { return "ProtoDB"; }

	void onEnterPhase( zpApplication* app )
	{
		zp_printfln( "Enter ProtoDB Phase..." );
		m_count = 6;

		zpProtoDBManager* p = app->getProtoDBManager();

		p->setProtoDBFile( "data/protodb.jsonb" );
		p->initialize( 10 );
		ZP_PROTODB_CATEGORY( p, Character );
		
		p->setup();
	}
	void onLeavePhase( zpApplication* app )
	{
		zp_printfln( "Leave ProtoDB Phase..." );
		zpProtoDBManager* p = app->getProtoDBManager();
		p->teardown();
		p->destroy();
	}

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app, zp_float deltaTime, zp_float realTime )
	{
		m_count -= deltaTime;
		return m_count < 0.f ? ZP_APPLICATION_PHASE_COMPLETE : ZP_APPLICATION_PHASE_NORMAL;
	}

private:
	zp_float m_count;
};

class bPlayPhase : public zpApplicationPhase
{
public:
	~bPlayPhase() {}

	const zp_char* getPhaseName() const { return "Play"; }

	void onEnterPhase( zpApplication* app )
	{
		zp_printfln( "Enter Play Phase..." );
	}
	void onLeavePhase( zpApplication* app )
	{
		zp_printfln( "Leave Play Phase..." );
	}

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app, zp_float deltaTime, zp_float realTime )
	{
		if( app->getInputManager()->getKeyboard()->isKeyPressed( ZP_KEY_CODE_P ) )
		{
			app->popCurrentPhase();
		}
		return ZP_APPLICATION_PHASE_NORMAL;
	}
};

class bPhaseLoadWorld : public zpApplicationPhase
{
public:
	bPhaseLoadWorld() {}
	~bPhaseLoadWorld() {}

	const zp_char* getPhaseName() const { return "LoadWorld"; }

	void setup( const zp_char* loadingWorld, const zp_char* nextWorld )
	{
		m_loadingWorld = loadingWorld;
		m_nextWorld = nextWorld;
	}

	void onEnterPhase( zpApplication* app ) {}
	void onLeavePhase( zpApplication* app ) {}

	// update the current phase
	zpApplicationPhaseResult onUpdatePhase( zpApplication* app, zp_float deltaTime, zp_float realTime ) { return ZP_APPLICATION_PHASE_NORMAL; }

private:
	zpString m_loadingWorld;
	zpString m_nextWorld;
};

class bEditorState : public zpApplicationState
{
public:
	bEditorState() {}
	~bEditorState() {}

	const zp_char* getStateName() const { return "Editor"; }

	void onEnterState( zpApplication* app )
	{
		const zpArrayList< zpCamera* >& cameras = app->getRenderPipeline()->getUsedCameras( ZP_CAMERA_TYPE_MAIN );
		cameras.foreach( []( zpCamera* c ) { c->setActive( false ); } );

		m_editorCamera = app->getRenderPipeline()->getCamera( ZP_CAMERA_TYPE_MAIN );
		m_editorCamera->cloneCamera( cameras[ 0 ] );
		m_editorCamera->setActive( true );
		m_editorCamera->setRenderTarget( 0, app->getRenderPipeline()->getRenderingEngine()->getBackBufferRenderTarget() );
		m_editorCamera->setDepthStencilBuffer( app->getRenderPipeline()->getRenderingEngine()->getBackBufferDepthStencilBuffer() );

		app->setApplicationPaused( true );

		zpProtoDBHandle c;
		app->getProtoDBManager()->getPrototype( "Character", "proto_blackjack1", c );
		zp_printfln( "Character num %d", c.getData< Character >()->num );

		m_editorUI = app->getObjectContentManager()->createObject( "objects/editor_ui.objectb" );

		m_runt = app->getObjectContentManager()->createObject( "objects/runt.objectb" );

		zpTweenComponent* tween = m_runt->getComponents()->addTweenComponent( zpBison::null );
		zpTweenAction* a = tween->tweenToPosition( zpMath::Vector4( 0, 0, 10, 1 ), 2, 0, false, false );
		a->flags.mark( ZP_TWEEN_FLAGS_REPEAT );
		a->flags.mark( ZP_TWEEN_FLAGS_PING_PONG );
		a->method = ZP_TWEEN_METHOD_SMOOTHSTEP;
	}
	void onLeaveState( zpApplication* app )
	{
		app->getRenderPipeline()->releaseCamera( m_editorCamera );
		m_editorCamera = ZP_NULL;

		m_editorUI->destroy();
		m_editorUI = ZP_NULL;

		m_runt->destroy();
		m_runt = ZP_NULL;

		const zpArrayList< zpCamera* >& cameras = app->getRenderPipeline()->getUsedCameras( ZP_CAMERA_TYPE_MAIN );
		cameras.foreach( []( zpCamera* c ) { c->setActive( true ); } );

		app->setApplicationPaused( false );
	}

	void onUpdateState( zpApplication* app, zp_float deltaTime, zp_float realTime )
	{
		const zpKeyboard* keyboard = app->getInputManager()->getKeyboard();
		const zpMouse* mouse = app->getInputManager()->getMouse();

		const zpVector4f& pos = m_editorCamera->getPosition();
		const zpVector4f& forward = m_editorCamera->getLookTo();
		const zpVector4f& up = m_editorCamera->getUp();

		zpVector4f right = zpMath::Vector4Cross3( forward, up );

		zp_int w = mouse->getScrollWheelDelta();
		const zpVector2i& mouseDelta = mouse->getDelta();

		if( w != 0 )
		{
			if( keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) )
			{
				w *= 6;
			}

			zpVector4f f = zpMath::Vector4Scale( forward, zpMath::Scalar( (zp_float)w ) );
			f = zpMath::Vector4Add( pos, f );

			m_editorCamera->setPosition( f );
		}

		if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) )
		{
			zp_bool leftButton = mouse->isButtonDown( ZP_MOUSE_BUTTON_LEFT );
			zp_bool rightButton = mouse->isButtonDown( ZP_MOUSE_BUTTON_RIGHT );
			if( leftButton && !rightButton )
			{
				zpVector4f lootTo = forward;
				zpScalar x, y, rt;
				
				x = zpMath::Scalar( (zp_float)mouseDelta.getX() );
				y = zpMath::Scalar( (zp_float)mouseDelta.getY() );
				rt = zpMath::Scalar( realTime );

				x = zpMath::ScalarMul( x, rt );
				y = zpMath::ScalarMul( y, rt );

				zpVector4f r, u;
				r = zpMath::Vector4Scale( right, x );
				u = zpMath::Vector4Scale( up, y );
				r = zpMath::Vector4Add( r, u );
				r = zpMath::Vector4Add( r, pos );

				m_editorCamera->setPosition( r );
				m_editorCamera->setLookTo( lootTo );
			}
			else if( rightButton && !leftButton )
			{
				zpVector4f lootAt = m_editorCamera->getLookAt();
				zpScalar x, y, rt;
				x = zpMath::Scalar( (zp_float)mouseDelta.getX() );
				y = zpMath::Scalar( (zp_float)mouseDelta.getY() );
				rt = zpMath::Scalar( realTime );

				x = zpMath::ScalarMul( x, rt );
				y = zpMath::ScalarMul( y, rt );
				y = zpMath::ScalarNeg( y );

				x = zpMath::ScalarDegToRad( x );
				y = zpMath::ScalarDegToRad( y );

				zpVector4f camPos = pos;
				zpVector4f dir;
				dir = zpMath::Vector4Sub( lootAt, camPos );

				camPos = zpMath::Vector4Add( camPos, dir );
				camPos = zpMath::Vector4RotateY( camPos, x );
				camPos = zpMath::Vector4RotateX( camPos, y );

				dir = zpMath::Vector4Neg( dir );
				camPos = zpMath::Vector4Add( camPos, dir );

				m_editorCamera->setPosition( camPos );
				m_editorCamera->setLookAt( lootAt );
			}
			else if( rightButton && leftButton )
			{

			}
		}
	}

private:
	zpCamera* m_editorCamera;
	zpObject* m_editorUI;
	zpObject* m_runt;
};

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	zp_int exitCode = 0;
	zpRandom *d = zpRandom::getInstance();

	zpMemorySystem* mem = zpMemorySystem::getInstance();
	mem->initialize( ZP_MEMORY_MB( 10 ) );

#if 0
	for( zp_int i = 0; i < 100; ++i )
	{
		void* a = mem->allocate( ZP_MEMORY_MB( d->randomFloat( 0.01f, 0.1f ) ) );
		void* b = mem->allocate( ZP_MEMORY_MB( d->randomFloat( 0.01f, 0.1f ) ) );
		void* c = mem->allocate( ZP_MEMORY_MB( d->randomFloat( 0.01f, 0.1f ) ) );

		mem->deallocate( a );
		mem->deallocate( c );
		mem->deallocate( b );
	}

	mem->takeMemorySnapshot( 0, ZP_MEMORY_KB( 1 ) );




	if( 0 )
#endif
	{
		bProtoDBPhase protoDBPhase;
		bPhaseLoadWorld loadWorld;
		bPlayPhase playPhase;

		bEditorState editorState;

		zpString cmdLine( lpCmdLine );
		zpArrayList< zpString > args;
		cmdLine.split( ' ', args );

		zpApplication* application = new zpApplication;

		// set config files
		application->setConfigFilename( BLACKJACK_CONFIG );
		application->setOptionsFilename( BLACKJACK_OPTIONS );

		// add phases
		application->addPhase( &protoDBPhase );
		//application->addPhase( &loadWorld );
		application->addPhase( &playPhase );

		// add states
		application->addState( &editorState );

		// process command line
		application->processCommandLine( args );
		
		// init, run, and shutdown
		application->initialize();
		application->run();
		exitCode = application->shutdown();

		delete application;
	}
	mem->shutdown();

	return exitCode;
}
