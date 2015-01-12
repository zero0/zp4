#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include "Blackjack.h"


#define BLACKJACK_CONFIG	"Blackjack.jsonb"

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

		p->initialize( 10 );
		ZP_PROTODB_CATEGORY( p, Character, bProtoDBCreateCharacter );
		
		p->setup();
	}
	void onLeavePhase( zpApplication* app )
	{
		zp_printfln( "Leave ProtoDB Phase..." );
		zpProtoDBManager* p = app->getProtoDBManager();
		p->shutdown();
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

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	zp_int exitCode = 0;

	{
		bProtoDBPhase protoDBPhase;
		zpPhaseLoadWorld loadWorld;
		bPlayPhase playPhase;

		zpApplication app;
		
		zpString cmdLine( lpCmdLine );
		zpArrayList< zpString > args;
		cmdLine.split( ' ', args );
		
		app.setOptionsFilename( BLACKJACK_CONFIG );
		
		//app.addPhase( &loadWorld );
		app.addPhase( &protoDBPhase );
		app.addPhase( &playPhase );

		app.initialize( args );
		app.setup();
		
		app.run();

		app.teardown();
		exitCode = app.shutdown();
	}

	return exitCode;
}
