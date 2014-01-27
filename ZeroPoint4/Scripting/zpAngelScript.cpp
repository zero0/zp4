#include "zpScripting.h"
#include "angelscript.h"

#include "Common/zpCommon.h"
#include "Rendering/zpRendering.h"

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

#pragma region Register Core
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

void as_zp_printf( const zpString& str )
{
	zp_printf( str.str() );
}
void as_zp_printfln( const zpString& str )
{
	zp_printfln( str.str() );
}

void as_Register_zpString( asIScriptEngine* engine );
void as_Register_zpArray( asIScriptEngine* engine );
void as_Register_zpScalar( asIScriptEngine* engine );
void as_Register_zpVector4f( asIScriptEngine* engine );
void as_Register_zpVector2f( asIScriptEngine* engine );
void as_Register_zpColor4f( asIScriptEngine* engine );
void as_Register_zpMatrix4f( asIScriptEngine* engine );

void as_Register_Core( asIScriptEngine* engine )
{
	as_Register_zpString( engine );
	as_Register_zpArray( engine );
	as_Register_zpScalar( engine );
	as_Register_zpVector4f( engine );
	as_Register_zpVector2f( engine );
	as_Register_zpColor4f( engine );
	as_Register_zpMatrix4f( engine );

	zp_int r;
	r = engine->RegisterGlobalFunction( "void print(const string &in)", asFUNCTION( as_zp_printf ), asCALL_CDECL );  AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void println(const string &in)", asFUNCTION( as_zp_printfln ), asCALL_CDECL );  AS_ASSERT( r );
}
#pragma endregion

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

void as_Register_zpString( asIScriptEngine* engine )
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
void as_Register_zpArray( asIScriptEngine* engine )
{
	//zp_int r;

	//r = engine->RegisterObjectType( "array<class T>", sizeof( zpArrayList ), asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpScalar
void as_zpScalar_Constructor( zpScalar* self )
{
	*self = zpScalar();
}
void as_zpScalar_ConstructorX( zpScalar* self, float value )
{
	*self = zpScalar( value );
}
void as_zpScalar_CopyConstructor( zpScalar* self, const zpScalar& scalar )
{
	*self = scalar;
}
void as_zpScalar_Deconstructor( zpScalar* self )
{
	self->~zpScalar();
}

zpScalar as_zpScalar_Add( zpScalar* self, const zpScalar& b )
{
	zpScalar s;
	zpMath::Add( s, *self, b );
	return s;
}
zpScalar as_zpScalar_Sub( zpScalar* self, const zpScalar& b )
{
	zpScalar s;
	zpMath::Sub( s, *self, b );
	return s;
}
zpScalar as_zpScalar_Mul( zpScalar* self, const zpScalar& b )
{
	zpScalar s;
	zpMath::Mul( s, *self, b );
	return s;
}
zpScalar as_zpScalar_Div( zpScalar* self, const zpScalar& b )
{
	zpScalar s;
	zpMath::Div( s, *self, b );
	return s;
}

zpScalar as_zpScalar_Addf( zpScalar* self, zp_float b )
{
	zpScalar s;
	zpMath::Add( s, *self, zpScalar( b ) );
	return s;
}
zpScalar as_zpScalar_Subf( zpScalar* self, zp_float b )
{
	zpScalar s;
	zpMath::Sub( s, *self, zpScalar( b ) );
	return s;
}
zpScalar as_zpScalar_Mulf( zpScalar* self, zp_float b )
{
	zpScalar s;
	zpMath::Mul( s, *self, zpScalar( b ) );
	return s;
}
zpScalar as_zpScalar_Divf( zpScalar* self, zp_float b )
{
	zpScalar s;
	zpMath::Div( s, *self, zpScalar( b ) );
	return s;
}

zpScalar as_zpScalar_AddAssign( zpScalar* self, const zpScalar& b )
{
	zpMath::Add( *self, *self, b );
	return *self;
}
zpScalar as_zpScalar_SubAssign( zpScalar* self, const zpScalar& b )
{
	zpMath::Sub( *self, *self, b );
	return *self;
}
zpScalar as_zpScalar_MulAssign( zpScalar* self, const zpScalar& b )
{
	zpMath::Mul( *self, *self, b );
	return *self;
}
zpScalar as_zpScalar_DivAssign( zpScalar* self, const zpScalar& b )
{
	zpMath::Div( *self, *self, b );
	return *self;
}

zpScalar as_zpScalar_AddAssignf( zpScalar* self, zp_float b )
{
	zpMath::Add( *self, *self, zpScalar( b ) );
	return *self;
}
zpScalar as_zpScalar_SubAssignf( zpScalar* self, zp_float b )
{
	zpMath::Sub( *self, *self, zpScalar( b ) );
	return *self;
}
zpScalar as_zpScalar_MulAssignf( zpScalar* self, zp_float b )
{
	zpMath::Mul( *self, *self, zpScalar( b ) );
	return *self;
}
zpScalar as_zpScalar_DivAssignf( zpScalar* self, zp_float b )
{
	zpMath::Div( *self, *self, zpScalar( b ) );
	return *self;
}

zpScalar as_zpScalar_Neg( zpScalar* self )
{
	zpScalar s;
	zpMath::Neg( s, *self );
	return s;
}

zp_bool as_zpScalar_Eq( zpScalar* self, zpScalar b )
{
	return zpMath::Cmp( *self, b ) == 0;
}
zp_bool as_zpScalar_Eqf( zpScalar* self, zp_float b )
{
	return zpMath::Cmp( *self, zpScalar( b ) ) == 0;
}

zp_float as_zpScalar_AsFloat( zpScalar* self )
{
	return self->getFloat();
}

void as_Register_zpScalar( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterObjectType( "scalar", sizeof( zpScalar ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "scalar", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpScalar_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "scalar", asBEHAVE_CONSTRUCT, "void f( const float )", asFUNCTION( as_zpScalar_ConstructorX ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "scalar", asBEHAVE_CONSTRUCT, "void f( const scalar& in )", asFUNCTION( as_zpScalar_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "scalar", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpScalar_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "scalar", asBEHAVE_VALUE_CAST, "float f() const", asFUNCTION( as_zpScalar_AsFloat ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "scalar", "void opAssign( const scalar& in )", asFUNCTION( as_zpScalar_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "scalar", "scalar opAdd( const scalar& in )", asFUNCTION( as_zpScalar_Add ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opSub( const scalar& in )", asFUNCTION( as_zpScalar_Sub ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opMul( const scalar& in )", asFUNCTION( as_zpScalar_Mul ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opDiv( const scalar& in )", asFUNCTION( as_zpScalar_Div ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "scalar", "scalar opAdd( float )", asFUNCTION( as_zpScalar_Addf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opSub( float )", asFUNCTION( as_zpScalar_Subf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opMul( float )", asFUNCTION( as_zpScalar_Mulf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opDiv( float )", asFUNCTION( as_zpScalar_Divf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "scalar", "scalar opAddAssign( const scalar& in )", asFUNCTION( as_zpScalar_AddAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opSubAssign( const scalar& in )", asFUNCTION( as_zpScalar_SubAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opMulAssign( const scalar& in )", asFUNCTION( as_zpScalar_MulAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opDivAssign( const scalar& in )", asFUNCTION( as_zpScalar_DivAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "scalar", "scalar opAddAssign( float )", asFUNCTION( as_zpScalar_AddAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opSubAssign( float )", asFUNCTION( as_zpScalar_SubAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opMulAssign( float )", asFUNCTION( as_zpScalar_MulAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opDivAssign( float )", asFUNCTION( as_zpScalar_DivAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "scalar", "scalar opNeg()", asFUNCTION( as_zpScalar_Neg ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "scalar", "scalar opEquals( const scalar& in )", asFUNCTION( as_zpScalar_Eq ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "scalar", "scalar opEquals( float )", asFUNCTION( as_zpScalar_Eqf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "scalar", "float asFloat() const", asFUNCTION( as_zpScalar_AsFloat ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpVector4f
void as_zpVector4f_Constructor( zpVector4f* self )
{
	*self = zp_move( zpVector4f() );
}
void as_zpVector4f_ConstructorXY( zpVector4f* self, zp_float x, zp_float y )
{
	*self = zp_move( zpVector4f( x, y ) );
}
void as_zpVector4f_ConstructorXYZ( zpVector4f* self, zp_float x, zp_float y, zp_float z )
{
	*self = zp_move( zpVector4f( x, y, z ) );
}
void as_zpVector4f_ConstructorXYZW( zpVector4f* self, zp_float x, zp_float y, zp_float z, zp_float w )
{
	*self = zp_move( zpVector4f( x, y, z, w ) );
}
void as_zpVector4f_CopyConstructor( zpVector4f* self, const zpVector4f& copy )
{
	*self = copy;
}
void as_zpVector4f_Deconstructor( zpVector4f* self )
{
	self->~zpVector4f();
}

void as_zpVector4f_SetX( zpVector4f* self, zp_float value )
{
	self->setX( zpScalar( value ) );
}
void as_zpVector4f_SetY( zpVector4f* self, zp_float value )
{
	self->setY( zpScalar( value ) );
}
void as_zpVector4f_SetZ( zpVector4f* self, zp_float value )
{
	self->setZ( zpScalar( value ) );
}
void as_zpVector4f_SetW( zpVector4f* self, zp_float value )
{
	self->setW( zpScalar( value ) );
}

zpScalar as_zpVector4f_Dot2( const zpVector4f& a, const zpVector4f& b )
{
	zpScalar s;
	zpMath::Dot2( s, a, b );
	return s;
}
zpScalar as_zpVector4f_Dot3( const zpVector4f& a, const zpVector4f& b )
{
	zpScalar s;
	zpMath::Dot3( s, a, b );
	return s;
}
zpScalar as_zpVector4f_Dot4( const zpVector4f& a, const zpVector4f& b )
{
	zpScalar s;
	zpMath::Dot4( s, a, b );
	return s;
}
zpVector4f as_zpVector4f_Cross3( const zpVector4f& a, const zpVector4f& b )
{
	zpVector4f s;
	zpMath::Cross3( s, a, b );
	return s;
}

zpScalar as_zpVector4f_Length2( zpVector4f* self )
{
	zpScalar s;
	zpMath::Length2( s, *self );
	return s;
}
zpScalar as_zpVector4f_Length3( zpVector4f* self )
{
	zpScalar s;
	zpMath::Length3( s, *self );
	return s;
}
zpScalar as_zpVector4f_LengthSqr2( zpVector4f* self )
{
	zpScalar s;
	zpMath::LengthSquared2( s, *self );
	return s;
}
zpScalar as_zpVector4f_LengthSqr3( zpVector4f* self )
{
	zpScalar s;
	zpMath::LengthSquared3( s, *self );
	return s;
}

void as_Register_zpVector4f( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterObjectType(      "vec4", sizeof( zpVector4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpVector4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec4", asBEHAVE_CONSTRUCT, "void f( float, float )", asFUNCTION( as_zpVector4f_ConstructorXY ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec4", asBEHAVE_CONSTRUCT, "void f( float, float, float )", asFUNCTION( as_zpVector4f_ConstructorXYZ ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec4", asBEHAVE_CONSTRUCT, "void f( float, float, float, float )", asFUNCTION( as_zpVector4f_ConstructorXYZW ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec4", asBEHAVE_CONSTRUCT, "void f( const vec4& in )", asFUNCTION( as_zpVector4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpVector4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec4", "void opAssign( const vec4& in )", asMETHODPR( zpVector4f, operator=, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec4", "scalar get_x() const", asMETHODPR( zpVector4f, getX, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "scalar get_y() const", asMETHODPR( zpVector4f, getY, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "scalar get_z() const", asMETHODPR( zpVector4f, getZ, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "scalar get_w() const", asMETHODPR( zpVector4f, getW, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec4", "void set_x( const scalar& in )", asMETHODPR( zpVector4f, setX, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "void set_y( const scalar& in )", asMETHODPR( zpVector4f, setY, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "void set_z( const scalar& in )", asMETHODPR( zpVector4f, setZ, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "void set_w( const scalar& in )", asMETHODPR( zpVector4f, setW, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec4", "scalar get_length2() const", asFUNCTION( as_zpVector4f_Length2 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "scalar get_length3() const", asFUNCTION( as_zpVector4f_Length3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec4", "scalar get_lengthSqr2() const", asFUNCTION( as_zpVector4f_LengthSqr2 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec4", "scalar get_lengthSqr3() const", asFUNCTION( as_zpVector4f_LengthSqr3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "scalar dot2( const vec4& in, const vec4& in )", asFUNCTION( as_zpVector4f_Dot2 ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "scalar dot3( const vec4& in, const vec4& in )", asFUNCTION( as_zpVector4f_Dot3 ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "scalar dot4( const vec4& in, const vec4& in )", asFUNCTION( as_zpVector4f_Dot4 ), asCALL_CDECL ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "vec4 cross3( const vec4& in, const vec4& in )", asFUNCTION( as_zpVector4f_Cross3 ), asCALL_CDECL ); AS_ASSERT( r );

	//
	//r = engine->RegisterObjectMethod( "Vector4f", "void zero3()", asMETHODPR( zpVector4f, zero3, (), void ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "Vector4f", "void zero4()", asMETHODPR( zpVector4f, zero4, (), void ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "Vector4f", "bool isZero3() const", asMETHODPR( zpVector4f, isZero3, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "Vector4f", "bool isZero4() const", asMETHODPR( zpVector4f, isZero4, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "Vector4f", "real dot3( const Vector4f& in ) const", asMETHODPR( zpVector4f, dot3, ( const zpVector4f& ) const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "Vector4f", "real dot4( const Vector4f& in ) const", asMETHODPR( zpVector4f, dot4, ( const zpVector4f& ) const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "Vector4f", "Vector4f cross3( const Vector4f& in ) const", asMETHODPR( zpVector4f, cross3, ( const zpVector4f& ) const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "Vector4f", "Vector4f cross4( const Vector4f& in ) const", asMETHODPR( zpVector4f, cross4, ( const zpVector4f& ) const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "Vector4f", "real magnitude3() const", asMETHODPR( zpVector4f, magnitude3, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "Vector4f", "real magnitude4() const", asMETHODPR( zpVector4f, magnitude4, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "Vector4f", "real magnitudeSquared3() const", asMETHODPR( zpVector4f, magnitudeSquared3, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( "Vector4f", "real magnitudeSquared4() const", asMETHODPR( zpVector4f, magnitudeSquared4, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	//
	//r = engine->RegisterObjectMethod( "Vector4f", "bool opEquals( const Vector4f& in ) const", asMETHODPR( zpVector4f, operator==, ( const zpVector4f& ) const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpVector2
void as_zpVector2f_Constructor( zpVector2f* self )
{
	*self = zp_move( zpVector2f() );
}
void as_zpVector2f_ConstructorXY( zpVector2f* self, zp_float x, zp_float y )
{
	*self = zp_move( zpVector2f( x, y ) );
}
void as_zpVector2f_CopyConstructor( zpVector2f* self, const zpVector2f& copy )
{
	*self = copy;
}
void as_zpVector2f_Deconstructor( zpVector2f* self )
{
	self->~zpVector2f();
}

void as_Register_zpVector2f( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterObjectType(      "vec2f", sizeof( zpVector2f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec2f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpVector2f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec2f", asBEHAVE_CONSTRUCT, "void f( float, float )", asFUNCTION( as_zpVector2f_ConstructorXY ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec2f", asBEHAVE_CONSTRUCT, "void f( const vec2f& in )", asFUNCTION( as_zpVector2f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "vec2f", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpVector2f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec2f", "void opAssign( const vec2f& in )", asMETHODPR( zpVector2f, operator=, ( const zpVector2f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec2f", "float get_x() const", asMETHODPR( zpVector2f, getX, () const, zp_float ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec2f", "float get_y() const", asMETHODPR( zpVector2f, getY, () const, zp_float ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "vec2f", "void set_x( float )", asMETHODPR( zpVector2f, setX, ( zp_float ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "vec2f", "void set_y( float )", asMETHODPR( zpVector2f, setY, ( zp_float ), void ), asCALL_THISCALL ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpColor4f
void as_zpColor4f_Constructor( zpColor4f* self )
{
	*self = zp_move( zpColor4f() );
}
void as_zpColor4f_ConstructorRGB( zpColor4f* self, float r, float g, float b )
{
	*self = zp_move( zpColor4f( r, g, b ) );
}
void as_zpColor4f_ConstructorRGBA( zpColor4f* self, float r, float g, float b, float a )
{
	*self = zp_move( zpColor4f( r, g, b, a ) );
}
void as_zpColor4f_CopyConstructor( zpColor4f* self, const zpColor4f& color )
{
	*self = color;
}
void as_zpColor4f_Deconstructor( zpColor4f* self )
{
	self->~zpColor4f();
}

void as_Register_zpColor4f( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterObjectType( "color", sizeof( zpColor4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "color", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpColor4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "color", asBEHAVE_CONSTRUCT, "void f( float, float, float )", asFUNCTION( as_zpColor4f_ConstructorRGB ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "color", asBEHAVE_CONSTRUCT, "void f( float, float, float, float )", asFUNCTION( as_zpColor4f_ConstructorRGBA ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "color", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpColor4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "color", "void opAssign( const color& in )", asFUNCTION( as_zpColor4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpMatrix4f
void as_zpMatrix4f_Constructor( zpMatrix4f* self )
{
	*self = zp_move( zpMatrix4f() );
}
void as_zpMatrix4f_CopyConstructor( zpMatrix4f* self, const zpMatrix4f& copy )
{
	*self = copy;
}
void as_zpMatrix4f_Deconstructor( zpMatrix4f* self )
{
	self->~zpMatrix4f();
}

zpVector4f as_zpMatrix_MulVector( const zpVector4f& a, const zpMatrix4f& b )
{
	zpVector4f s;
	zpMath::Mul( s, a, b );
	return s;
}
zpMatrix4f as_zpMatrix_MulMatrix( const zpMatrix4f& a, const zpMatrix4f& b )
{
	zpMatrix4f s;
	zpMath::Mul( s, a, b );
	return s;
}
zpMatrix4f as_zpMatrix_MulScalar( const zpScalar& a, const zpMatrix4f& b )
{
	zpMatrix4f s;
	zpMath::Mul( s, a, b );
	return s;
}

void as_Register_zpMatrix4f( asIScriptEngine* engine )
{
	zp_int r;

	r = engine->RegisterObjectType(      "matrix", sizeof( zpMatrix4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "matrix", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpMatrix4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "matrix", asBEHAVE_CONSTRUCT, "void f( const matrix& in )", asFUNCTION( as_zpMatrix4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "matrix", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpMatrix4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "matrix", "void opAssign( const matrix& in )", asMETHODPR( zpMatrix4f, operator=, ( const zpMatrix4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "matrix", "void setIdentity()", asMETHODPR( zpMatrix4f, setIdentity, (), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "matrix", "void setRow( uint, const vec4& in )", asMETHODPR( zpMatrix4f, setRow, ( zp_uint, const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "matrix", "const vec4& getRow( uint ) const", asMETHODPR( zpMatrix4f, getRow, ( zp_uint ) const, const zpVector4f& ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "vec4 mul( const vec4& in, const matrix& in )", asFUNCTION( as_zpMatrix_MulVector ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "matrix mul( const matrix& in, const matrix& in )", asFUNCTION( as_zpMatrix_MulMatrix ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "matrix mul( const scalar& in, const matrix& in )", asFUNCTION( as_zpMatrix_MulScalar ), asCALL_CDECL ); AS_ASSERT( r );
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

#pragma region Register Rendering
void as_Register_Rendering( asIScriptEngine* engine, zpApplication* app )
{
	zp_int r;
	zpRenderingContext* i = app->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();

	r = engine->RegisterEnum( "RenderingLayer" ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "Opaque",				ZP_RENDERING_LAYER_OPAQUE ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "OpaqueDebug",			ZP_RENDERING_LAYER_OPAQUE_DEBUG ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "Skybox",				ZP_RENDERING_LAYER_SKYBOX ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "Transparent",			ZP_RENDERING_LAYER_TRANSPARENT ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "TransparentDebug",	ZP_RENDERING_LAYER_TRANSPARENT_DEBUG ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "UI",					ZP_RENDERING_LAYER_UI ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "RenderingLayer", "UIDebug",				ZP_RENDERING_LAYER_UI_DEBUG ); AS_ASSERT( r );

	r = engine->RegisterEnum( "Topology" ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "Topology", "Unknown",		ZP_TOPOLOGY_UNKNOWN ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "Topology", "PointList",		ZP_TOPOLOGY_POINT_LIST ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "Topology", "LineList",		ZP_TOPOLOGY_LINE_LIST ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "Topology", "LineStrip",		ZP_TOPOLOGY_LINE_STRIP ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "Topology", "TriangleList",	ZP_TOPOLOGY_TRIANGLE_LIST ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "Topology", "TriangleStrip",	ZP_TOPOLOGY_TRIANGLE_STRIP ); AS_ASSERT( r );

	r = engine->RegisterEnum( "VertexFormat" ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "VertexFormat", "VertexColor",		ZP_VERTEX_FORMAT_VERTEX_COLOR ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "VertexFormat", "VertexUV",			ZP_VERTEX_FORMAT_VERTEX_UV ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "VertexFormat", "VertexNormalUV",	ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( "VertexFormat", "VertexNormalUV2",	ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2 ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void BeginDraw( RenderingLayer, Topology, VertexFormat, int )", asMETHODPR( zpRenderingContext, beginDrawImmediate, ( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat, zpMaterialResourceInstance* material ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void EndDraw()", asMETHODPR( zpRenderingContext, endDrawImmediate, (), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void AddVertex( const vec4& in, const color& in )", asMETHODPR( zpRenderingContext, addVertex, ( const zpVector4f&, const zpColor4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void AddTriangleIndex( int, int, int )", asMETHODPR( zpRenderingContext, addTriangleIndex, ( zp_ushort, zp_ushort, zp_ushort ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
}
#pragma endregion

zpAngelScript::zpAngelScript()
	: m_engine( ZP_NULL )
	, m_immidiateContext( ZP_NULL )
	, m_currentThread( 0 )
{}
zpAngelScript::~zpAngelScript()
{}

zp_bool zpAngelScript::createEngine( zpApplication* app )
{
	if( m_engine ) return false;
	m_application = app;

	asSetGlobalMemoryFunctions( allocate, deallocate );

	m_engine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
	if( !m_engine ) return false;
	
	asIScriptEngine* engine = (asIScriptEngine*)m_engine;

	engine->SetMessageCallback( asFUNCTION( as_MessageCallback ), 0, asCALL_CDECL );

	as_Register_Core( engine );
	as_Register_Rendering( engine, app );
	//as_zpScalar_Register( s_engine );
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