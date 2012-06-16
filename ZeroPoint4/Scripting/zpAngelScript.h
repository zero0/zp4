#pragma once
#ifndef ZP_ANGELSCRIPT_H
#define ZP_ANGELSCRIPT_H

class asIScriptEngine;

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