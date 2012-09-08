#include "zpScripting.h"
#include "zpAngelScript.h"

#define ZP_SCRIPTING_MANAGER_DEFAULT_NUM_THREADS	10

void as_print( const zpString& str ) {
	zp_printfln( "%s", str.c_str() );
}
void as_printc( zp_uint color, const zpString& str ) {
	zp_printfcln( (zpConsoleColorType)color, "%s", str.c_str() );
}

void zpScriptingManager::as_sleep( zp_uint milliseconds ) {
	( (zpScriptingManager*)zpAngelScript::getInstance()->GetUserData() )->scriptSleep( milliseconds );
}

void zpScriptingManager::as_yield() {
	( (zpScriptingManager*)zpAngelScript::getInstance()->GetUserData() )->scriptYield();
}

void zpScriptingManager::as_createCoRoutine( const zpString& functionName ) {
	( (zpScriptingManager*)zpAngelScript::getInstance()->GetUserData() )->scriptCreateCoRoutine( functionName );
}

zpScriptingManager::zpScriptingManager() : 
	m_currentThread( 0 ),
	m_threads()
{}
zpScriptingManager::~zpScriptingManager() {}

void zpScriptingManager::receiveMessage( const zpMessage& message ) {}

void zpScriptingManager::serialize( zpSerializedOutput* out ) {}
void zpScriptingManager::deserialize( zpSerializedInput* in ) {}

void zpScriptingManager::onCreate() {
	m_threads.ensureCapacity( ZP_SCRIPTING_MANAGER_DEFAULT_NUM_THREADS );

	zpAngelScript::createInstance();
	asIScriptEngine* engine = zpAngelScript::getInstance();
	zp_int r;

	r = engine->SetMessageCallback( asMETHOD( zpScriptingManager, messageCallback ), this, asCALL_THISCALL );
	engine->SetUserData( this );

	// register print functions
	r = engine->RegisterGlobalFunction( "void print( string &in )", asFUNCTION( as_print ), asCALL_CDECL );
	r = engine->RegisterGlobalFunction( "void printc( uint, string &in )", asFUNCTION( as_printc ), asCALL_CDECL );

	// register threading/co-routine functions
	r = engine->RegisterGlobalFunction( "void sleep( uint )", asFUNCTION( as_sleep ), asCALL_CDECL );
	r = engine->RegisterGlobalFunction( "void yield()", asFUNCTION( as_yield ), asCALL_CDECL );
	r = engine->RegisterGlobalFunction( "void createCoRoutine( const string &in )", asFUNCTION( as_createCoRoutine ), asCALL_CDECL );
}
void zpScriptingManager::onDestroy() {
	m_threads.foreach( []( zpScriptingThreadContext& thread ) {
		thread.coRoutines.foreach( []( asIScriptContext* context ) {
			if( context ) {
				context->Abort();
				context->Release();
			}
		} );
		thread.coRoutines.clear();
	} );
	m_threads.clear();

	zpAngelScript::destroyInstance();
}

void zpScriptingManager::onUpdate() {
	zp_long time = zpTime::getInstance()->getTime();
	asIScriptEngine* engine = zpAngelScript::getInstance();

	zp_uint gc1, gc2;
	zp_int r;

	for( m_currentThread = 0; m_currentThread < m_threads.size(); ++m_currentThread ) {
		zpScriptingThreadContext& thread = m_threads[ m_currentThread ];
		if( thread.sleepUntil < time ) {
			engine->GetGCStatistics( &gc1 );

			asIScriptContext* context = thread.coRoutines[ thread.currentCoRoutine ];
			r = context->Execute();

			engine->GetGCStatistics( &gc2 );

			if( r != asEXECUTION_SUSPENDED ) {
				context->Release();

				thread.coRoutines.erase( thread.currentCoRoutine );
				if( thread.coRoutines.isEmpty() ) {
					m_threads.erase( m_currentThread );
					--m_currentThread;
				}
			}

			if( gc2 > gc1 ) {
				engine->GarbageCollect( asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE );
			}

			engine->GarbageCollect( asGC_ONE_STEP | asGC_DETECT_GARBAGE );
		}
	}
}

void zpScriptingManager::onEnabled() {}
void zpScriptingManager::onDisabled() {}

void zpScriptingManager::callObjectFunction( void* object, void* function ) {
	if( !object || !function ) return;

	asIScriptContext* context = zpAngelScript::getInstance()->CreateContext();

	context->Prepare( (asIScriptFunction*)function );
	context->SetObject( object );

	zpScriptingThreadContext stc;
	stc.sleepUntil = 0;
	stc.currentCoRoutine = 0;
	stc.coRoutines.pushBack( context );

	m_threads.pushBack( stc );
	context->SetUserData( &m_threads.back() );
}

void zpScriptingManager::scriptSleep( zp_uint milliseconds ) {
	asIScriptContext* context = asGetActiveContext();
	if( context ) {
		context->Suspend();

		( (zpScriptingThreadContext*)context->GetUserData() )->sleepUntil = zpTime::getInstance()->getTime() + milliseconds;
	}
}
void zpScriptingManager::scriptYield() {
	asIScriptContext* context = asGetActiveContext();
	if( context ) {
		zpScriptingThreadContext& thread = m_threads[ m_currentThread ];
		thread.currentCoRoutine = ( thread.currentCoRoutine + 1 ) % thread.coRoutines.size();
		
		context->Suspend();
	}
}
void zpScriptingManager::scriptCreateCoRoutine( const zpString& functionName ) {
	asIScriptContext* context = asGetActiveContext();
	if( context ) {
		zp_int r;
		zpString module( context->GetFunction()->GetModuleName() );

		zpStringBuffer decl;
		decl << "void " << functionName << "()";

		zp_int funcId = zpAngelScript::getInstance()->GetModule( module.c_str() )->GetFunctionIdByDecl( decl.toString().c_str() );
		if( funcId < 0 ) {
			return;
		}

		asIScriptContext* newContext = zpAngelScript::getInstance()->CreateContext();
		if( !newContext ) {
			return;
		}

		r = newContext->Prepare( funcId );
		if( r < 0 ) {
			newContext->Release();
			return;
		}

		zpScriptingThreadContext* thread = (zpScriptingThreadContext*)context->GetUserData();
		if( thread ) {
			thread->coRoutines.pushBack( newContext );
			newContext->SetUserData( thread );
		} else {
			zpScriptingThreadContext stc;
			stc.sleepUntil = 0;
			stc.currentCoRoutine = 0;
			stc.coRoutines.pushBack( newContext );
			stc.coRoutines.pushBack( context );

			m_threads.pushBack( stc );
			newContext->SetUserData( &m_threads.back() );
			context->SetUserData( &m_threads.back() );
		}
	}
}

void zpScriptingManager::messageCallback( const asSMessageInfo& msg ) {
	zp_printfln( "%s (%d, %d) [%s]: %s", msg.section, msg.row, msg.col, msg.type == asMSGTYPE_ERROR ? "ERR " : msg.type == asMSGTYPE_WARNING ? "WARN" : "INFO", msg.message );
}