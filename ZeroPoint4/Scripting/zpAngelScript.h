#pragma once
#ifndef ZP_ANGELSCRIPT_H
#define ZP_ANGELSCRIPT_H

#include <angelscript.h>

#if ZP_DEBUG
#pragma comment( lib, "angelscriptd.lib" )
#else
#pragma comment( lib, "angelscript.lib" )
#endif

typedef void (*zpAngelScriptRegisterFunction)( asIScriptEngine* engine );

class zpAngelScript {
public:
	~zpAngelScript();

	static zp_bool createInstance( zpMemoryAllocator* memoryAllocator = ZP_NULL );
	static void destroyInstance();

	static asIScriptEngine* getInstance();

private:
	zpAngelScript();

	static void* allocate( zp_uint size );
	static void deallocate( void* ptr );

	static asIScriptEngine* s_engine;
	static zpMemoryAllocator* s_allocator;
};

#endif