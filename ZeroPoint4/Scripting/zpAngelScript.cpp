#include "zpScripting.h"
#include "zpAngelScript.h"

#define AS_ASSERT( r )	if( (r) < 0 ) { zp_printfln( "Assert Failed (%d) %s:%d", (r), __FILE__, __LINE__ ); }

#pragma region Message Callback
void __onMessageCallback( const asSMessageInfo& info ) {
	switch( info.type ) {
	case asMSGTYPE_ERROR:
		zp_printfln( "%s [%d,%d] ERR:  %s", info.section, info.row, info.col, info.message );
		break;
	case asMSGTYPE_WARNING:
		zp_printfln( "%s [%d,%d] WARN: %s", info.section, info.row, info.col, info.message );
		break;
	case asMSGTYPE_INFORMATION:
		zp_printfln( "%s [%d,%d] INFO: %s", info.section, info.row, info.col, info.message );
		break;
	}
}
#pragma endregion

#pragma region Register zpString
zpString __zpStringFactory( asUINT length, const char* string ) {
	return zpString( string, length );
}

void __zpStringFactoryGeneric( asIScriptGeneric* gen ) {
	asUINT length = gen->GetArgDWord( 0 );
	const char* string = (const char*)gen->GetArgAddress( 1 );

	//new (gen->GetAddressOfReturnLocation()) zpString( string, length );
	*((zpString*)gen->GetAddressOfReturnLocation()) = zp_move( zpString( string, length ) );
}

void __zpStringConstructor( zpString* self ) {
	*self = zp_move( zpString() );
}
void __zpStringCopyConstructor( zpString* self, const zpString& copy ) {
	*self = zp_move( zpString( copy ) );
}
void __zpStringDeconstructor( zpString* self ) {
	self->~zpString();
}

zpString __zpStringSubstring( const zpString* self, zp_uint start, zp_int end ) {
	return self->substring( start, end );
}
zp_int __zpStringIndexOf( const zpString* self, const zpString& str, zp_uint start ) {
	zp_uint index = self->indexOf( str, start );
	return index == zpString::npos ? -1 : (zp_int)index;
}
zp_int __zpStringLastIndexOf( const zpString* self, const zpString& str, zp_uint end ) {
	zp_uint index = self->lastIndexOf( str, end );
	return index == zpString::npos ? -1 : (zp_int)index;
}

void __zpStringRegister( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "string", sizeof(zpString), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	
	r = engine->RegisterStringFactory( "string", asFUNCTION( __zpStringFactory ), asCALL_CDECL ); AS_ASSERT( r );

	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( __zpStringConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_CONSTRUCT, "void f( const string &in )", asFUNCTION( __zpStringCopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( __zpStringDeconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "string& opAssign( const string &in )", asMETHODPR( zpString, operator=, ( const zpString& ), zpString& ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "uint length() const", asMETHODPR( zpString, length, () const, zp_uint ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "bool isEmpty() const", asMETHODPR( zpString, isEmpty, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "uint8& opIndex( uint index )", asMETHODPR( zpString, operator[], ( zp_uint index ), zp_char& ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "const uint8 opIndex( uint index ) const", asMETHOD( zpString, charAt ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "string substring( uint start, int end = -1 ) const", asFUNCTION( __zpStringSubstring ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "int indexOf( const string& in, uint start = 0 ) const", asFUNCTION( __zpStringIndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "int lastIndexOf( const string& in, uint end = 0 ) const", asFUNCTION( __zpStringLastIndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "string toLower() const", asMETHODPR( zpString, toLower, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "string toUpper() const", asMETHODPR( zpString, toUpper, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "bool startsWith( const string& in ) const", asMETHOD( zpString, startsWith ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "bool endsWith( const string& in ) const", asMETHOD( zpString, endsWith ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "string trim() const", asMETHODPR( zpString, trim, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "int opCmp( const string& in ) const", asMETHOD( zpString, compareTo ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "bool opEquals( const string &in ) const", asFUNCTIONPR( operator==, (const zpString& string1, const zpString& string2), zp_bool ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

}
#pragma endregion

#pragma region Register Array
void __zpArrayRegister( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "array<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE ); AS_ASSERT( r );
	
}
#pragma endregion

zpAngelScript::zpAngelScript() {}
zpAngelScript::~zpAngelScript() {}

zpMemoryAllocator* zpAngelScript::s_allocator = ZP_NULL;
asIScriptEngine* zpAngelScript::s_engine = ZP_NULL;
zp_bool zpAngelScript::createInstance( zpMemoryAllocator* memoryAllocator ) {
	if( s_engine ) return false;

	if( memoryAllocator ) {
		s_allocator = memoryAllocator;
		asSetGlobalMemoryFunctions( allocate, deallocate );
	}

	s_engine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
	if( !s_engine ) return false;
	
	registerStringType();

	return true;
}
void zpAngelScript::destroyInstance() {
	if( s_engine ) {
		s_engine->Release();
		s_engine = ZP_NULL;
	}
}

asIScriptEngine* zpAngelScript::getInstance() {
	return s_engine;
}

void* zpAngelScript::allocate( zp_uint size ) {
	return s_allocator->allocate( size );
}
void zpAngelScript::deallocate( void* ptr ) {
	s_allocator->deallocate( ptr );
}

void zpAngelScript::registerStringType() {
	__zpStringRegister( s_engine );
}
void zpAngelScript::registerArrayType() {
	__zpArrayRegister( s_engine );
}