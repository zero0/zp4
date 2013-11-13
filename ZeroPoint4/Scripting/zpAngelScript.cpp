#include "zpScripting.h"
#include "angelscript.h"

#if ZP_WIN_32
#if ZP_DEBUG
#pragma comment( lib, "angelscriptd.lib" )
#else
#pragma comment( lib, "angelscript.lib" )
#endif
#elif ZP_WIN_64
#if ZP_DEBUG
#pragma comment( lib, "angelscript64d.lib" )
#else
#pragma comment( lib, "angelscript64.lib" )
#endif
#endif

#define AS_ASSERT( r )	if( (r) < asSUCCESS ) { zpLog::debug() << "Assert Failed: (" << r << ") " << __FILE__ << ':' << __LINE__ << zpLog::endl; }

#pragma region Message Callback
void as_MessageCallback( const asSMessageInfo& info )
{
	zpLogOutput* out;
	switch( info.type )
	{
	case asMSGTYPE_ERROR:
		out = &zpLog::error();
		break;
	case asMSGTYPE_WARNING:
		out = &zpLog::warning();
		break;
	case asMSGTYPE_INFORMATION:
	default:
		out = &zpLog::message();
		break;
	}

	*out << info.section << " [" << info.row << ',' << info.col << "] " << info.message << zpLog::endl;
}
#pragma endregion

void as_zp_printf( const zpString& str )
{
	zp_printf( str.str() );
}
void as_zp_printfln( const zpString& str )
{
	zp_printfln( str.str() );
}

void as_Register_Core( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterGlobalFunction( "void print(const string &in)", asFUNCTION( as_zp_printf ), asCALL_CDECL );  AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void println(const string &in)", asFUNCTION( as_zp_printfln ), asCALL_CDECL );  AS_ASSERT( r );
}

#pragma region Register zpString
zpString as_zpString_Factory( asUINT length, const char* string )
{
	return zpString( string, length );
}

void as_zpString_FactoryGeneric( asIScriptGeneric* gen )
{
	asUINT length = gen->GetArgDWord( 0 );
	const char* string = (const char*)gen->GetArgAddress( 1 );

	new (gen->GetAddressOfReturnLocation()) zpString( string, length );
	//*((zpString*)gen->GetAddressOfReturnLocation()) = zp_move( zpString( string, length ) );
}

void as_zpString_Constructor( zpString* self )
{
	new (self) zpString();
}
void as_zpString_CopyConstructor( zpString* self, const zpString& copy )
{
	new (self) zpString( copy );
}
void as_zpString_Deconstructor( zpString* self )
{
	self->~zpString();
}

//zpString as_zpString_Substring( const zpString* self, zp_uint start, zp_int end )
//{
//	return self->substring( start, end );
//}
zp_int as_zpString_IndexOf( const zpString* self, const zpString& str, zp_uint start )
{
	zp_uint index = self->indexOf( str, start );
	return index == zpString::npos ? -1 : (zp_int)index;
}
zp_int as_zpString_LastIndexOf( const zpString* self, const zpString& str, zp_uint end )
{
	zp_uint index = self->lastIndexOf( str, end );
	return index == zpString::npos ? -1 : (zp_int)index;
}

void as_zpString_Register( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterObjectType( "string", sizeof( zpString ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	
	r = engine->RegisterStringFactory( "string", asFUNCTION( as_zpString_Factory ), asCALL_CDECL ); AS_ASSERT( r );

	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpString_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_CONSTRUCT, "void f( const string &in )", asFUNCTION( as_zpString_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpString_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "void opAssign( const string &in )", asMETHODPR( zpString, operator=, ( const zpString& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "uint length() const", asMETHODPR( zpString, length, () const, zp_uint ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "bool isEmpty() const", asMETHODPR( zpString, isEmpty, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "uint8& opIndex( uint index )", asMETHODPR( zpString, operator[], ( zp_uint index ), zp_char& ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "const uint8 opIndex( uint index ) const", asMETHOD( zpString, charAt ), asCALL_THISCALL ); AS_ASSERT( r );

	//r = engine->RegisterObjectMethod( "string", "string substring( uint start, int end = -1 ) const", asFUNCTION( as_zpString_Substring ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "int indexOf( const string& in, uint start = 0 ) const", asFUNCTION( as_zpString_IndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "int lastIndexOf( const string& in, uint end = 0 ) const", asFUNCTION( as_zpString_LastIndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	//r = engine->RegisterObjectMethod( "string", "string toLower() const", asMETHODPR( zpString, toLower, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "string", "string toUpper() const", asMETHODPR( zpString, toUpper, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );
	
	//r = engine->RegisterObjectMethod( "string", "bool startsWith( const string& in ) const", asMETHOD( zpString, startsWith ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "string", "bool endsWith( const string& in ) const", asMETHOD( zpString, endsWith ), asCALL_THISCALL ); AS_ASSERT( r );
	
	//r = engine->RegisterObjectMethod( "string", "string trim() const", asMETHODPR( zpString, trim, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "int opCmp( const string& in ) const", asMETHOD( zpString, compareTo ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "bool opEquals( const string &in ) const", asFUNCTIONPR( operator==, (const zpString& string1, const zpString& string2), zp_bool ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register Array
void as_zpArray_Register( asIScriptEngine* engine )
{
	//zp_int r;

	//r = engine->RegisterObjectType( "array<class T>", sizeof( zpArrayList ), asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE ); AS_ASSERT( r );
}
#pragma endregion
#if 0
#pragma region Register zp_real
void as_zp_real_Constructor( zpScalar* self ) {
	*self = zpScalarZero();
	//zp_real r = zp_real_zero();
	//memcpy( self, &r, sizeof( zp_real ) );
}
void as_zp_real_ConstructorX( zpScalar* self, float value ) {
	*self = zpScalarFromFloat( value );
	//zp_real r = zp_real_from_float( value );
	//memcpy( self, &r, sizeof( zp_real ) );
}
void as_zp_real_CopyConstructor( zpScalar* self, zpScalar real ) {
	*self = real;
	//memcpy( self, &real, sizeof( zp_real ) );
}
void as_zp_real_Deconstructor( zpScalar* self ) {
	//*self = zp_real_zero();
}

zpScalar as_zp_real_Add( zpScalar* self, zpScalar b ) {
	return zpScalarAdd( *self, b );
}
zpScalar as_zp_real_Sub( zpScalar* self, zpScalar b ) {
	return zpScalarSub( *self, b );
}
zpScalar as_zp_real_Mul( zpScalar* self, zpScalar b ) {
	return zpScalarMul( *self, b );
}
zpScalar as_zp_real_Div( zpScalar* self, zpScalar b ) {
	return zpScalarDiv( *self, b );
}

zpScalar as_zp_real_Addf( zpScalar* self, zp_float b ) {
	return zpScalarAdd( *self, zpScalarFromFloat( b ) );
}
zpScalar as_zp_real_Subf( zpScalar* self, zp_float b ) {
	return zpScalarSub( *self, zpScalarFromFloat( b ) );
}
zpScalar as_zp_real_Mulf( zpScalar* self, zp_float b ) {
	return zpScalarMul( *self, zpScalarFromFloat( b ) );
}
zpScalar as_zp_real_Divf( zpScalar* self, zp_float b ) {
	return zpScalarDiv( *self, zpScalarFromFloat( b ) );
}

zpScalar as_zp_real_AddAssign( zpScalar* self, zpScalar b ) {
	*self = zpScalarAdd( *self, b );
	return *self;
}
zpScalar as_zp_real_SubAssign( zpScalar* self, zpScalar b ) {
	*self =  zpScalarSub( *self, b );
	return *self;
}
zpScalar as_zp_real_MulAssign( zpScalar* self, zpScalar b ) {
	*self =  zpScalarMul( *self, b );
	return *self;
}
zpScalar as_zp_real_DivAssign( zpScalar* self, zpScalar b ) {
	*self =  zpScalarDiv( *self, b );
	return *self;
}

zpScalar as_zp_real_AddAssignf( zpScalar* self, zp_float b ) {
	*self = zpScalarAdd( *self, zpScalarFromFloat( b ) );
	return *self;
}
zpScalar as_zp_real_SubAssignf( zpScalar* self, zp_float b ) {
	*self =  zpScalarSub( *self, zpScalarFromFloat( b ) );
	return *self;
}
zpScalar as_zp_real_MulAssignf( zpScalar* self, zp_float b ) {
	*self =  zpScalarMul( *self, zpScalarFromFloat( b ) );
	return *self;
}
zpScalar as_zp_real_DivAssignf( zpScalar* self, zp_float b ) {
	*self =  zpScalarDiv( *self, zpScalarFromFloat( b ) );
	return *self;
}

zpScalar as_zp_real_Neg( zpScalar* self ) {
	return zpScalarNeg( *self );
}

zp_bool as_zp_real_Eq( zpScalar* self, zpScalar b ) {
	return zpScalarEq( *self, b );
}
zp_bool as_zp_real_Eqf( zpScalar* self, zp_float b ) {
	return zpScalarEq( *self, zpScalarFromFloat( b ) );
}

zp_float as_zp_real_AsFloat( zpScalar* self ) {
	return zpScalarToFloat( *self );
}

void as_zp_real_Register( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "real", sizeof( zpScalar ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "real", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zp_real_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "real", asBEHAVE_CONSTRUCT, "void f( const float )", asFUNCTION( as_zp_real_ConstructorX ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "real", asBEHAVE_CONSTRUCT, "void f( real )", asFUNCTION( as_zp_real_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "real", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zp_real_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "real", "void opAssign( real )", asFUNCTION( as_zp_real_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "real", "real opAdd( real )", asFUNCTION( as_zp_real_Add ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opSub( real )", asFUNCTION( as_zp_real_Sub ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opMul( real )", asFUNCTION( as_zp_real_Mul ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opDiv( real )", asFUNCTION( as_zp_real_Div ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "real", "real opAdd( float )", asFUNCTION( as_zp_real_Addf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opSub( float )", asFUNCTION( as_zp_real_Subf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opMul( float )", asFUNCTION( as_zp_real_Mulf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opDiv( float )", asFUNCTION( as_zp_real_Divf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "real", "real opAddAssign( real )", asFUNCTION( as_zp_real_AddAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opSubAssign( real )", asFUNCTION( as_zp_real_SubAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opMulAssign( real )", asFUNCTION( as_zp_real_MulAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opDivAssign( real )", asFUNCTION( as_zp_real_DivAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "real", "real opAddAssign( float )", asFUNCTION( as_zp_real_AddAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opSubAssign( float )", asFUNCTION( as_zp_real_SubAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opMulAssign( float )", asFUNCTION( as_zp_real_MulAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opDivAssign( float )", asFUNCTION( as_zp_real_DivAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "real", "real opNeg()", asFUNCTION( as_zp_real_Neg ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "real", "real opEquals( real )", asFUNCTION( as_zp_real_Eq ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "real", "real opEquals( float )", asFUNCTION( as_zp_real_Eqf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "real", "float asFloat() const", asFUNCTION( as_zp_real_AsFloat ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpVector4f
void as_zpVector4f_Constructor( zpVector4f* self ) {
	*self = zp_move( zpVector4f() );
}
void as_zpVector4f_ConstructorXYZ( zpVector4f* self, zp_float x, zp_float y, zp_float z ) {
	*self = zp_move( zpVector4f( x, y, z ) );
}
void as_zpVector4f_ConstructorXYZW( zpVector4f* self, zp_float x, zp_float y, zp_float z, zp_float w ) {
	*self = zp_move( zpVector4f( x, y, z, w ) );
}
void as_zpVector4f_CopyConstructor( zpVector4f* self, const zpVector4f& copy ) {
	*self = zp_move( zpVector4f( copy ) );
}
void as_zpVector4f_Deconstructor( zpVector4f* self ) {
	self->~zpVector4f();
}

void as_zpVector4f_SetX( zpVector4f* self, zp_float value ) {
	self->setX( zpScalarFromFloat( value ) );
}
void as_zpVector4f_SetY( zpVector4f* self, zp_float value ) {
	self->setY( zpScalarFromFloat( value ) );
}
void as_zpVector4f_SetZ( zpVector4f* self, zp_float value ) {
	self->setZ( zpScalarFromFloat( value ) );
}
void as_zpVector4f_SetW( zpVector4f* self, zp_float value ) {
	self->setW( zpScalarFromFloat( value ) );
}

void as_zpVector4f_Register( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "Vector4f", sizeof( zpVector4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Vector4f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpVector4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Vector4f", asBEHAVE_CONSTRUCT, "void f( float, float, float )", asFUNCTION( as_zpVector4f_ConstructorXYZ ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Vector4f", asBEHAVE_CONSTRUCT, "void f( float, float, float, float )", asFUNCTION( as_zpVector4f_ConstructorXYZW ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Vector4f", asBEHAVE_CONSTRUCT, "void f( const Vector4f& in )", asFUNCTION( as_zpVector4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Vector4f", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpVector4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "void opAssign( const Vector4f& in )", asMETHODPR( zpVector4f, operator=, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "real get_x() const", asMETHODPR( zpVector4f, getX, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "real get_y() const", asMETHODPR( zpVector4f, getY, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "real get_z() const", asMETHODPR( zpVector4f, getZ, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "real get_w() const", asMETHODPR( zpVector4f, getW, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "void set_x( real )", asMETHODPR( zpVector4f, setX, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void set_y( real )", asMETHODPR( zpVector4f, setY, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void set_z( real )", asMETHODPR( zpVector4f, setZ, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void set_w( real )", asMETHODPR( zpVector4f, setW, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "void zero3()", asMETHODPR( zpVector4f, zero3, (), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void zero4()", asMETHODPR( zpVector4f, zero4, (), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "bool isZero3() const", asMETHODPR( zpVector4f, isZero3, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "bool isZero4() const", asMETHODPR( zpVector4f, isZero4, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "real dot3( const Vector4f& in ) const", asMETHODPR( zpVector4f, dot3, ( const zpVector4f& ) const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "real dot4( const Vector4f& in ) const", asMETHODPR( zpVector4f, dot4, ( const zpVector4f& ) const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "Vector4f cross3( const Vector4f& in ) const", asMETHODPR( zpVector4f, cross3, ( const zpVector4f& ) const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "Vector4f cross4( const Vector4f& in ) const", asMETHODPR( zpVector4f, cross4, ( const zpVector4f& ) const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "real magnitude3() const", asMETHODPR( zpVector4f, magnitude3, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "real magnitude4() const", asMETHODPR( zpVector4f, magnitude4, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "real magnitudeSquared3() const", asMETHODPR( zpVector4f, magnitudeSquared3, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "real magnitudeSquared4() const", asMETHODPR( zpVector4f, magnitudeSquared4, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "bool opEquals( const Vector4f& in ) const", asMETHODPR( zpVector4f, operator==, ( const zpVector4f& ) const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpMatrix4f
void as_zpMatrix4f_Constructor( zpMatrix4f* self ) {
	*self = zp_move( zpMatrix4f() );
}
void as_zpMatrix4f_ConstructorXYZW( zpMatrix4f* self, zp_float x, zp_float y, zp_float z, zp_float w ) {
	*self = zp_move( zpMatrix4f() );
}
void as_zpMatrix4f_CopyConstructor( zpMatrix4f* self, const zpMatrix4f& copy ) {
	*self = zp_move( zpMatrix4f( copy ) );
}
void as_zpMatrix4f_Deconstructor( zpMatrix4f* self ) {
	self->~zpMatrix4f();
}

void as_zpMatrix4f_Register( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "Matrix4f", sizeof( zpMatrix4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Matrix4f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpMatrix4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Matrix4f", asBEHAVE_CONSTRUCT, "void f( const Matrix4f& in )", asFUNCTION( as_zpMatrix4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "Matrix4f", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpMatrix4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "void opAssign( const Matrix4f& in )", asMETHODPR( zpMatrix4f, operator=, ( const zpMatrix4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "float& at( uint, uint )", asMETHODPR( zpMatrix4f, operator(), ( zp_uint, zp_uint ), zp_float& ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "void getPosition( Vector4f& out ) const", asMETHODPR( zpMatrix4f, getPosition, ( zpVector4f& ) const, void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void setPosition( const Vector4f& in )", asMETHODPR( zpMatrix4f, setPosition, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "void translate( const Vector4f& in )", asMETHODPR( zpMatrix4f, translate, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void rotateX( real )", asMETHODPR( zpMatrix4f, rotateX, ( zpScalar ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void rotateY( real )", asMETHODPR( zpMatrix4f, rotateY, ( zpScalar ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void rotateZ( real )", asMETHODPR( zpMatrix4f, rotateZ, ( zpScalar ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void scale( real )", asMETHODPR( zpMatrix4f, scale, ( zpScalar ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void scale( const Vector4f& in )", asMETHODPR( zpMatrix4f, scale, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "bool isIdentity() const", asMETHODPR( zpMatrix4f, isIdentity, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "void mul( const Vector4f& in, Vector4f& out ) const", asMETHODPR( zpMatrix4f, mul, ( const zpVector4f&, zpVector4f& ) const, void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void mul( const Matrix4f& in, Matrix4f& out ) const", asMETHODPR( zpMatrix4f, mul, ( const zpMatrix4f&, zpMatrix4f& ) const, void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "void lootAt( const Vector4f& in, const Vector4f& in, const Vector4f& in )", asMETHODPR( zpMatrix4f, lookAt, ( const zpVector4f&, const zpVector4f&, const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void lootTo( const Vector4f& in, const Vector4f& in, const Vector4f& in )", asMETHODPR( zpMatrix4f, lookTo, ( const zpVector4f&, const zpVector4f&, const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void perspective( float, float, float, float )", asMETHODPR( zpMatrix4f, perspective, ( zp_float, zp_float, zp_float, zp_float ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void ortho( float, float, float, float )", asMETHODPR( zpMatrix4f, ortho, ( zp_float, zp_float, zp_float, zp_float ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "void transpose()", asMETHODPR( zpMatrix4f, transpose, (), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "bool opEquals( const Matrix4f& in ) const", asMETHODPR( zpMatrix4f, operator==, ( const zpMatrix4f& ) const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

}
#pragma endregion


#pragma region Register zpGameObject

void as_zpGameObject_Register( asIScriptEngine* engine ) {
	//if( !engine ) return;
	//zp_int r;
	//zpGameObject::
	//r = engine->RegisterObjectType( "GameObject", sizeof( zpGameObject ), asOBJ_REF ); AS_ASSERT( r );
	//r = engine->RegisterObjectBehaviour( "GameObject", asBEHAVE_ADDREF, "void f()", asMETHODPR( zpGameObject, addReference, () const, void ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectBehaviour( "GameObject", asBEHAVE_RELEASE, "void f()", asMETHODPR( zpGameObject, removeReference, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "GameObject", "const string& getName() const", asMETHODPR( zpGameObject, getName, () const, const zpString& ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "GameObject", "const Matrix4f& getLocalTransform() const", asMETHODPR( zpGameObject, getLocalTransform, () const, const zpMatrix4f& ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "GameObject", "const Matrix4f& getWorldTransform() const", asMETHODPR( zpGameObject, getWorldTransform, () const, const zpMatrix4f& ), asCALL_THISCALL ); AS_ASSERT( r );

}
#pragma endregion
#endif
zpAngelScript::zpAngelScript()
	: m_engine( ZP_NULL )
	, m_immidiateContext( ZP_NULL )
	, m_currentThread( 0 )
{}
zpAngelScript::~zpAngelScript()
{}

zp_bool zpAngelScript::createEngine()
{
	if( m_engine ) return false;

	asSetGlobalMemoryFunctions( allocate, deallocate );

	m_engine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
	if( !m_engine ) return false;
	
	asIScriptEngine* engine = (asIScriptEngine*)m_engine;

	engine->SetMessageCallback( asFUNCTION( as_MessageCallback ), 0, asCALL_CDECL );

	as_zpString_Register( engine );

	as_Register_Core( engine );
	//as_zp_real_Register( s_engine );
	//as_zpArray_Register( s_engine );
	//as_zpVector4f_Register( s_engine );
	//as_zpMatrix4f_Register( s_engine );

	//as_zpGameObject_Register( s_engine );

	m_immidiateContext = engine->CreateContext();

	m_threadContexts.resize( ZP_SCRIPT_THREAD_COUNT );
	m_freeThreads.reserve( ZP_SCRIPT_THREAD_COUNT );
	m_usedThreads.reserve( ZP_SCRIPT_THREAD_COUNT );

	zpScriptThreadContext* begin = m_threadContexts.begin();
	zpScriptThreadContext* end = m_threadContexts.end();
	for( ; begin != end; ++begin )
	{
		m_freeThreads.pushBack( begin );
	}
	
	return true;
}
void zpAngelScript::destroyEngine()
{
	garbageCollect();

	if( m_immidiateContext )
	{
		( (asIScriptContext*)m_immidiateContext )->Release();
		m_immidiateContext = ZP_NULL;
	}

	if( m_engine )
	{
		( (asIScriptEngine*)m_engine )->Release();
		m_engine = ZP_NULL;
	}
}

void zpAngelScript::processThreads()
{
	asIScriptEngine* engine = (asIScriptEngine*)m_engine;
	zp_long time = zpTime::getInstance()->getTime();

	zp_uint gc1, gc2;
	zp_int r;

	for( m_currentThread = 0; m_currentThread < (zp_int)m_usedThreads.size(); ++m_currentThread )
	{
		zpScriptThreadContext* thread = m_usedThreads[ m_currentThread ];
		if( thread->sleepUntil < time )
		{
			engine->GetGCStatistics( &gc1 );

			asIScriptContext* context = (asIScriptContext*)thread->coRoutines[ thread->currentCoRoutine ];
			r = context->Execute();

			engine->GetGCStatistics( &gc2 );

			if( r != asEXECUTION_SUSPENDED )
			{
				context->Release();

				thread->coRoutines.erase( thread->currentCoRoutine );

				if( thread->currentCoRoutine >= thread->coRoutines.size() )
				{
					thread->currentCoRoutine = 0;
				}

				if( thread->coRoutines.isEmpty() )
				{
					m_freeThreads.pushBack( thread );
					m_usedThreads.erase( m_currentThread );
					--m_currentThread;
				}
			}

			if( gc2 > gc1 )
			{
				engine->GarbageCollect( asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE );
			}

			engine->GarbageCollect( asGC_ONE_STEP | asGC_DETECT_GARBAGE );
		}
	}
}

zpAngelScript* zpAngelScript::s_instance = ZP_NULL;
void zpAngelScript::createInstance()
{
	s_instance = new zpAngelScript;
}
zpAngelScript* zpAngelScript::getInstance()
{
	return s_instance;
}
void zpAngelScript::destroyInstance()
{
	ZP_SAFE_DELETE( s_instance );
}

zp_handle zpAngelScript::getEngine() const
{
	return m_engine;
}

zp_handle zpAngelScript::createScriptObject( zp_handle objectType )
{
	asIScriptEngine* engine = (asIScriptEngine*)m_engine;
	asIObjectType* oType = (asIObjectType*)objectType;

	zp_handle scriptObject = engine->CreateScriptObject( oType );
	//engine->AddRefScriptObject( scriptObject, oType );

	return scriptObject;
}
void zpAngelScript::destroyScriptObject( zp_handle scriptObject, zp_handle objectType )
{
	asIScriptEngine* engine = (asIScriptEngine*)m_engine;
	asIObjectType* oType = (asIObjectType*)objectType;

	engine->ReleaseScriptObject( scriptObject, oType );
}

void zpAngelScript::callObjectMethod( zp_handle object, zp_handle method )
{
	if( object != ZP_NULL && method != ZP_NULL )
	{
		asIScriptEngine* engine = (asIScriptEngine*)m_engine;
		asIScriptContext* context = engine->CreateContext();

		context->Prepare( (asIScriptFunction*)method );
		context->SetObject( object );

		zpScriptThreadContext* threadContext;
		if( !m_freeThreads.isEmpty() )
		{
			threadContext = m_freeThreads.back();
			m_freeThreads.popBack();
		}

		threadContext->sleepUntil = 0;
		threadContext->currentCoRoutine = 0;
		threadContext->coRoutines.pushBack( context );
		context->SetUserData( threadContext );

		m_usedThreads.pushBack( threadContext );
	}
}
void zpAngelScript::callObjectMethodImmidiate( zp_handle object, zp_handle method )
{
	if( object != ZP_NULL && method != ZP_NULL  )
	{
		asIScriptContext* context = (asIScriptContext*)m_immidiateContext;
		
		context->Unprepare();
		context->Prepare( (asIScriptFunction*)method );
		context->SetObject( object );
		context->Execute();
	}
}

void zpAngelScript::sleep( zp_uint milliseconds )
{
	asIScriptContext* context = asGetActiveContext();
	if( context )
	{
		zpScriptThreadContext* threadContext = (zpScriptThreadContext*)context->GetUserData();
		if( threadContext )
		{
			zp_long time = zpTime::getInstance()->getTime();

			threadContext->sleepUntil = time + ( milliseconds * 1000 );
			threadContext->currentCoRoutine = ( threadContext->currentCoRoutine + 1 ) % threadContext->coRoutines.size();
		}
	}
}
void zpAngelScript::yield()
{
	asIScriptContext* context = asGetActiveContext();
	if( context )
	{
		context->Suspend();

		zpScriptThreadContext* threadContext = (zpScriptThreadContext*)context->GetUserData();
		if( threadContext )
		{
			threadContext->currentCoRoutine = ( threadContext->currentCoRoutine + 1 ) % threadContext->coRoutines.size();
		}
	}
}
void zpAngelScript::createCoRoutine( const zp_char* methodName )
{
	asIScriptContext* context = asGetActiveContext();
	if( context )
	{
		zpScriptThreadContext* threadContext = (zpScriptThreadContext*)context->GetUserData();
		if( threadContext )
		{
			asIScriptEngine* engine = (asIScriptEngine*)m_engine;

			const zp_char* moduleName = context->GetFunction()->GetModuleName();
			asIScriptFunction* method = engine->GetModule( moduleName )->GetFunctionByName( methodName );

			asIScriptContext* methodContext = engine->CreateContext();
			methodContext->Prepare( method );

			threadContext->coRoutines.pushBack( methodContext );
			methodContext->SetUserData( threadContext );
		}
	}
}

void zpAngelScript::garbageCollect()
{
	asIScriptEngine* engine = (asIScriptEngine*)m_engine;
	engine->GarbageCollect( asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE );
}

void* zpAngelScript::allocate( zp_uint size )
{
#if ZP_USE_MEMORY_SYSTEM
	return zpMemorySystem::getInstance()->allocate( size );
#else
	return zp_malloc( size );
#endif
}
void zpAngelScript::deallocate( void* ptr )
{
#if ZP_USE_MEMORY_SYSTEM
	zpMemorySystem::getInstance()->deallocate( ptr );
#else
	return zp_free( ptr );
#endif
}