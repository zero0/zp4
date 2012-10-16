#pragma once
#ifndef ZP_ANGELSCRIPT_H
#define ZP_ANGELSCRIPT_H

class asIScriptEngine;

typedef void (*zpAngelScriptRegisterFunction)( asIScriptEngine* engine );

class zpAngelScript {
public:
	~zpAngelScript();

	static zp_bool createInstance();
	static void destroyInstance();

	static asIScriptEngine* getInstance();

private:
	zpAngelScript();

	static void* allocate( zp_uint size );
	static void deallocate( void* ptr );

	static asIScriptEngine* s_engine;
};

#endif