#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include "Blackjack.h"


#define BLACKJACK_CONFIG	"BlackjackConfig.jsonb"
#define BLACKJACK_OPTIONS	"BlackjackOptions.jsonb"

struct Character
{
	zp_uint num;
};

void bProtoDBCreateCharacter( void* data, zp_uint stride, const zpBison::Value& def )
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
		m_count = 60;

		zpProtoDBManager* p = app->getProtoDBManager();

		//p->initialize( 10 );
		//ZP_PROTODB_CATEGORY( p, Character, bProtoDBCreateCharacter );
		
		//p->setup();
	}
	void onLeavePhase( zpApplication* app )
	{
		zp_printfln( "Leave ProtoDB Phase..." );
		zpProtoDBManager* p = app->getProtoDBManager();
		//p->shutdown();
	}

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app )
	{
		--m_count;
		return m_count == 0 ? ZP_APPLICATION_PHASE_COMPLETE : ZP_APPLICATION_PHASE_NORMAL;
	}

private:
	zp_int m_count;
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

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app )
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

	zpApplicationPhaseResult onUpdatePhase( zpApplication* app ) { return ZP_APPLICATION_PHASE_NORMAL; }

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
	}
	void onLeaveState( zpApplication* app )
	{
		app->getRenderPipeline()->releaseCamera( m_editorCamera );
		m_editorCamera = ZP_NULL;

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

		zpVector4f right;
		zpMath::Cross3( right, forward, up );

		zp_int w = mouse->getScrollWheelDelta();
		const zpVector2i& mouseDelta = mouse->getDelta();

		if( w != 0 )
		{
			if( keyboard->isKeyDown( ZP_KEY_CODE_SHIFT ) )
			{
				w *= 6;
			}

			zpVector4f f;
			zpMath::Mul( f, forward, zpScalar( (zp_float)w ) );
			zpMath::Add( f, pos, f );

			m_editorCamera->setPosition( f );
		}

		if( keyboard->isKeyDown( ZP_KEY_CODE_CONTROL ) )
		{
			zp_bool leftButton = mouse->isButtonDown( ZP_MOUSE_BUTTON_LEFT );
			zp_bool rightButton = mouse->isButtonDown( ZP_MOUSE_BUTTON_RIGHT );
			if( leftButton && !rightButton )
			{
				zpVector4f lootTo( forward );

				zpVector4f r, u;
				zpMath::Mul( r, right, zpScalar( realTime * mouseDelta.getX() ) );
				zpMath::Mul( u, up, zpScalar( realTime * mouseDelta.getY() ) );
				zpMath::Add( r, r, u );
				zpMath::Add( r, r, pos );

				m_editorCamera->setPosition( r );
				m_editorCamera->setLookTo( lootTo );
			}
			else if( rightButton && !leftButton )
			{
				zpVector4f lootAt( m_editorCamera->getLookAt() );

				zpVector4f camPos( pos );
				zpMath::Sub( camPos, camPos, lootAt );
				zpMath::RotateY( camPos, camPos, zpScalar( ZP_DEG_TO_RAD( mouseDelta.getX() ) ) );
				zpMath::RotateX( camPos, camPos, zpScalar( ZP_DEG_TO_RAD( -mouseDelta.getY() ) ) );
				zpMath::Add( camPos, camPos, lootAt );

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
