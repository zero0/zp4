#pragma once
#ifndef ZP_ANGELSCRIPT_H
#define ZP_ANGELSCRIPT_H

enum
{
	ZP_SCRIPT_THREAD_COUNT = 32,
	ZP_SCRIPT_COROUTINE_COUNT = 8,
};

class zpAngelScript
{
public:
	~zpAngelScript();

	static zpAngelScript* getInstance();
	
	zp_handle getEngine() const;

	zp_bool createEngine();
	void destroyEngine();

	void processThreads();

	zp_handle createScriptObject( zp_handle objectType );
	void destroyScriptObject( zp_handle object, zp_handle objectType );

	void callObjectMethod( zp_handle object, zp_handle method );

	void sleep( zp_uint milliseconds );
	void yield();
	void createCoRoutine( const zp_char* methodName );

private:
	static void* allocate( zp_uint size );
	static void deallocate( void* ptr );

	zpAngelScript();

	zp_handle m_engine;

	struct zpScriptThreadContext
	{
		zp_long sleepUntil;
		zp_uint currentCoRoutine;
		zpFixedArrayList< zp_handle, ZP_SCRIPT_COROUTINE_COUNT > coRoutines;
	};

	zp_int m_currentThread;
	zpFixedArrayList< zpScriptThreadContext, ZP_SCRIPT_THREAD_COUNT > m_threadContexts;
	zpArrayList< zpScriptThreadContext* > m_usedThreads;
	zpArrayList< zpScriptThreadContext* > m_freeThreads;
};

#endif