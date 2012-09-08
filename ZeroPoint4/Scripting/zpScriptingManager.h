#pragma once
#ifndef ZP_SCRIPTING_MANAGER_H
#define ZP_SCRIPTING_MANAGER_H

struct asSMessageInfo;
class asIScriptContext;

class zpScriptingManager : public zpGameManager {
public:
	zpScriptingManager();
	virtual ~zpScriptingManager();
	
	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	void callObjectFunction( void* object, void* function );

	void scriptSleep( zp_uint sleep );
	void scriptYield();
	void scriptCreateCoRoutine( const zpString& functionName );

	void messageCallback( const asSMessageInfo& msg );
	
	static void as_sleep( zp_uint milliseconds );
	static void as_yield();
	static void as_createCoRoutine( const zpString& functionName );

	struct zpScriptingThreadContext {
		zp_long sleepUntil;
		zp_uint currentCoRoutine;
		zpArrayList<asIScriptContext*> coRoutines;
	};

	zp_uint m_currentThread;
	zpArrayList<zpScriptingThreadContext> m_threads;

	friend class zpScriptingComponent;
};

#endif