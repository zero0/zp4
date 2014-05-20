#include "Blackjack.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

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
	~bProtoDBPhase() {}

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

	zp_bool onPhaseUpdate( zpApplication* app )
	{
		--m_count;
		return m_count == 0;
	}

private:
	zp_int m_count;
};
class bPlayPhase : public zpApplicationPhase
{
public:
	~bPlayPhase() {}

	void onEnterPhase( zpApplication* app )
	{
		zp_printfln( "Enter Play Phase..." );
	}
	void onLeavePhase( zpApplication* app )
	{
		zp_printfln( "Leave Play Phase..." );
	}

	zp_bool onPhaseUpdate( zpApplication* app )
	{
		if( app->getInputManager()->getKeyboard()->isKeyPressed( ZP_KEY_CODE_P ) )
		{
			app->popCurrentPhase();
		}
		return false;
	}
};

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	zp_int exitCode = 0;

	{
		zpApplication app;
		
		zpString cmdLine( lpCmdLine );
		zpArrayList< zpString > args;
		cmdLine.split( ' ', args );
		
		app.setOptionsFilename( BLACKJACK_CONFIG );
		
		app.addPhase< bProtoDBPhase >();
		app.addPhase< bPlayPhase >();

		app.initialize( args );
		app.run();
		exitCode = app.shutdown();
	}

	return exitCode;
}
