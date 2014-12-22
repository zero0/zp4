#include "zpScripting.h"
#include "angelscript.h"

#include "Common/zpCommon.h"
#include "Rendering/zpRendering.h"

#include <new>

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

#define ZP_SCRIPT_STRING "string"
#define ZP_SCRIPT_SCALAR "scalar"
#define ZP_SCRIPT_VECTOR2 "Vector2"
#define ZP_SCRIPT_VECTOR4 "Vector4"
#define ZP_SCRIPT_COLOR "Color"
#define ZP_SCRIPT_MATRIX4 "Matrix4"
#define ZP_SCRIPT_BOUNDBOXAABB "BoundingBox"

#define ZP_SCRIPT_OBJECT "Object"

#define ZP_SCRIPT_MATERIAL "Material"
#define ZP_SCRIPT_FONT "Font"
#define ZP_SCRIPT_RENDERING_QUEUE "RenderingQueue"
#define ZP_SCRIPT_TOPOLOGY "Topology"
#define ZP_SCRIPT_VERTEX_FORMAT "VertexFormat"
#define ZP_SCRIPT_FONT_ALIGNMENT "FontAlignment"

#define AS_ASSERT( r )	ZP_ASSERT( (r) >= asSUCCESS, "AngelScript Assert Failed (%d): %s:%d", (r), __FILE__, __LINE__ )
//if( (r) < asSUCCESS ) { zpLog::debug() << "Assert Failed: (" << r << ") " << __FILE__ << ':' << __LINE__ << zpLog::endl; }

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
void as_Register_zpBoundingAABB( asIScriptEngine* engine );

void as_Register_Core( asIScriptEngine* engine )
{
	as_Register_zpString( engine );
	as_Register_zpArray( engine );
	as_Register_zpScalar( engine );
	as_Register_zpVector4f( engine );
	as_Register_zpVector2f( engine );
	as_Register_zpColor4f( engine );
	as_Register_zpMatrix4f( engine );
	as_Register_zpBoundingAABB( engine );

	zp_int r;
	r = engine->RegisterGlobalFunction( "void print(const " ZP_SCRIPT_STRING " &in)", asFUNCTION( as_zp_printf ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void println(const " ZP_SCRIPT_STRING " &in)", asFUNCTION( as_zp_printfln ), asCALL_CDECL ); AS_ASSERT( r );
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

	r = engine->RegisterObjectType( ZP_SCRIPT_STRING, sizeof( zpString ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	
	r = engine->RegisterStringFactory( ZP_SCRIPT_STRING, asFUNCTION( as_zpString_Factory ), asCALL_CDECL ); AS_ASSERT( r );

	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_STRING, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpString_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_STRING, asBEHAVE_CONSTRUCT, "void f( const " ZP_SCRIPT_STRING " &in )", asFUNCTION( as_zpString_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_STRING, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpString_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "void opAssign( const " ZP_SCRIPT_STRING " &in )", asMETHODPR( zpString, operator=, ( const zpString& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "uint length() const", asMETHODPR( zpString, length, () const, zp_uint ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "bool isEmpty() const", asMETHODPR( zpString, isEmpty, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "uint8& opIndex( uint index )", asMETHODPR( zpString, operator[], ( zp_uint index ), zp_char& ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "const uint8 opIndex( uint index ) const", asMETHOD( zpString, charAt ), asCALL_THISCALL ); AS_ASSERT( r );

	//r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "string substring( uint start, int end = -1 ) const", asFUNCTION( as_zpString_Substring ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "int indexOf( const " ZP_SCRIPT_STRING "& in, uint start = 0 ) const", asFUNCTION( as_zpString_IndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "int lastIndexOf( const " ZP_SCRIPT_STRING "& in, uint end = 0 ) const", asFUNCTION( as_zpString_LastIndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	//r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "string toLower() const", asMETHODPR( zpString, toLower, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "string toUpper() const", asMETHODPR( zpString, toUpper, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );
	
	//r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "bool startsWith( const string& in ) const", asMETHOD( zpString, startsWith ), asCALL_THISCALL ); AS_ASSERT( r );
	//r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "bool endsWith( const string& in ) const", asMETHOD( zpString, endsWith ), asCALL_THISCALL ); AS_ASSERT( r );
	
	//r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "string trim() const", asMETHODPR( zpString, trim, () const, zpString ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "int opCmp( const " ZP_SCRIPT_STRING "& in ) const", asMETHOD( zpString, compareTo ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_STRING, "bool opEquals( const " ZP_SCRIPT_STRING " &in ) const", asFUNCTIONPR( operator==, (const zpString& string1, const zpString& string2), zp_bool ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
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

	r = engine->RegisterObjectType( ZP_SCRIPT_SCALAR, sizeof( zpScalar ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_SCALAR, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpScalar_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_SCALAR, asBEHAVE_CONSTRUCT, "void f( const float )", asFUNCTION( as_zpScalar_ConstructorX ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_SCALAR, asBEHAVE_CONSTRUCT, "void f( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_SCALAR, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpScalar_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_SCALAR, asBEHAVE_VALUE_CAST, "float f() const", asFUNCTION( as_zpScalar_AsFloat ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "void opAssign( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opAdd( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_Add ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opSub( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_Sub ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opMul( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_Mul ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opDiv( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_Div ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opAdd( float )", asFUNCTION( as_zpScalar_Addf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opSub( float )", asFUNCTION( as_zpScalar_Subf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opMul( float )", asFUNCTION( as_zpScalar_Mulf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opDiv( float )", asFUNCTION( as_zpScalar_Divf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opAddAssign( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_AddAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opSubAssign( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_SubAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opMulAssign( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_MulAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opDivAssign( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_DivAssign ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opAddAssign( float )", asFUNCTION( as_zpScalar_AddAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opSubAssign( float )", asFUNCTION( as_zpScalar_SubAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opMulAssign( float )", asFUNCTION( as_zpScalar_MulAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opDivAssign( float )", asFUNCTION( as_zpScalar_DivAssignf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opNeg()", asFUNCTION( as_zpScalar_Neg ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opEquals( const " ZP_SCRIPT_SCALAR "& in )", asFUNCTION( as_zpScalar_Eq ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "" ZP_SCRIPT_SCALAR " opEquals( float )", asFUNCTION( as_zpScalar_Eqf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_SCALAR, "float asFloat() const", asFUNCTION( as_zpScalar_AsFloat ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
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

	r = engine->RegisterObjectType(      ZP_SCRIPT_VECTOR4, sizeof( zpVector4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR4, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpVector4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR4, asBEHAVE_CONSTRUCT, "void f( float, float )", asFUNCTION( as_zpVector4f_ConstructorXY ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR4, asBEHAVE_CONSTRUCT, "void f( float, float, float )", asFUNCTION( as_zpVector4f_ConstructorXYZ ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR4, asBEHAVE_CONSTRUCT, "void f( float, float, float, float )", asFUNCTION( as_zpVector4f_ConstructorXYZW ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR4, asBEHAVE_CONSTRUCT, "void f( const " ZP_SCRIPT_VECTOR4 "& in )", asFUNCTION( as_zpVector4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR4, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpVector4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "void opAssign( const " ZP_SCRIPT_VECTOR4 "& in )", asMETHODPR( zpVector4f, operator=, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_x() const", asMETHODPR( zpVector4f, getX, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_y() const", asMETHODPR( zpVector4f, getY, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_z() const", asMETHODPR( zpVector4f, getZ, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_w() const", asMETHODPR( zpVector4f, getW, () const, zpScalar ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "void set_x( const " ZP_SCRIPT_SCALAR "& in )", asMETHODPR( zpVector4f, setX, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "void set_y( const " ZP_SCRIPT_SCALAR "& in )", asMETHODPR( zpVector4f, setY, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "void set_z( const " ZP_SCRIPT_SCALAR "& in )", asMETHODPR( zpVector4f, setZ, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "void set_w( const " ZP_SCRIPT_SCALAR "& in )", asMETHODPR( zpVector4f, setW, ( const zpScalar& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_length2() const", asFUNCTION( as_zpVector4f_Length2 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_length3() const", asFUNCTION( as_zpVector4f_Length3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_lengthSqr2() const", asFUNCTION( as_zpVector4f_LengthSqr2 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR4, "" ZP_SCRIPT_SCALAR " get_lengthSqr3() const", asFUNCTION( as_zpVector4f_LengthSqr3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_SCALAR " dot2( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR4 "& in )", asFUNCTION( as_zpVector4f_Dot2 ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_SCALAR " dot3( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR4 "& in )", asFUNCTION( as_zpVector4f_Dot3 ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_SCALAR " dot4( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR4 "& in )", asFUNCTION( as_zpVector4f_Dot4 ), asCALL_CDECL ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_VECTOR4 " cross3( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR4 "& in )", asFUNCTION( as_zpVector4f_Cross3 ), asCALL_CDECL ); AS_ASSERT( r );

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

	r = engine->RegisterObjectType(      ZP_SCRIPT_VECTOR2, sizeof( zpVector2f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR2, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpVector2f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR2, asBEHAVE_CONSTRUCT, "void f( float, float )", asFUNCTION( as_zpVector2f_ConstructorXY ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR2, asBEHAVE_CONSTRUCT, "void f( const " ZP_SCRIPT_VECTOR2 "& in )", asFUNCTION( as_zpVector2f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_VECTOR2, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpVector2f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR2, "void opAssign( const " ZP_SCRIPT_VECTOR2 "& in )", asMETHODPR( zpVector2f, operator=, ( const zpVector2f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR2, "float get_x() const", asMETHODPR( zpVector2f, getX, () const, zp_float ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR2, "float get_y() const", asMETHODPR( zpVector2f, getY, () const, zp_float ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR2, "void set_x( float )", asMETHODPR( zpVector2f, setX, ( zp_float ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_VECTOR2, "void set_y( float )", asMETHODPR( zpVector2f, setY, ( zp_float ), void ), asCALL_THISCALL ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpColor4f
void as_zpColor4f_Constructor( zpColor4f* self )
{
	*self = zp_move( zpColor4f() );
}
void as_zpColor4f_ConstructorRGB( zpColor4f* self, zp_float r, zp_float g, zp_float b )
{
	*self = zp_move( zpColor4f( r, g, b ) );
}
void as_zpColor4f_ConstructorRGBA( zpColor4f* self, zp_float r, zp_float g, zp_float b, zp_float a )
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

	r = engine->RegisterObjectType(      ZP_SCRIPT_COLOR, sizeof( zpColor4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_COLOR, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpColor4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_COLOR, asBEHAVE_CONSTRUCT, "void f( float, float, float )", asFUNCTION( as_zpColor4f_ConstructorRGB ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_COLOR, asBEHAVE_CONSTRUCT, "void f( float, float, float, float )", asFUNCTION( as_zpColor4f_ConstructorRGBA ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_COLOR, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpColor4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_COLOR, "void opAssign( const " ZP_SCRIPT_COLOR "& in )", asFUNCTION( as_zpColor4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
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

	r = engine->RegisterObjectType(      ZP_SCRIPT_MATRIX4, sizeof( zpMatrix4f ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_MATRIX4, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpMatrix4f_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_MATRIX4, asBEHAVE_CONSTRUCT, "void f( const " ZP_SCRIPT_MATRIX4 "& in )", asFUNCTION( as_zpMatrix4f_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_MATRIX4, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpMatrix4f_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_MATRIX4, "void opAssign( const " ZP_SCRIPT_MATRIX4 "& in )", asMETHODPR( zpMatrix4f, operator=, ( const zpMatrix4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_MATRIX4, "void setIdentity()", asMETHODPR( zpMatrix4f, setIdentity, (), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_MATRIX4, "void setRow( uint, const " ZP_SCRIPT_VECTOR4 "& in )", asMETHODPR( zpMatrix4f, setRow, ( zp_uint, const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_MATRIX4, "const " ZP_SCRIPT_VECTOR4 "& getRow( uint ) const", asMETHODPR( zpMatrix4f, getRow, ( zp_uint ) const, const zpVector4f& ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_VECTOR4 " mul( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_MATRIX4 "& in )", asFUNCTION( as_zpMatrix_MulVector ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_MATRIX4 " mul( const " ZP_SCRIPT_MATRIX4 "& in, const " ZP_SCRIPT_MATRIX4 "& in )", asFUNCTION( as_zpMatrix_MulMatrix ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "" ZP_SCRIPT_MATRIX4 " mul( const " ZP_SCRIPT_SCALAR "& in, const " ZP_SCRIPT_MATRIX4 "& in )", asFUNCTION( as_zpMatrix_MulScalar ), asCALL_CDECL ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpBoundingAABB
void as_zpBoundingAABB_Constructor( zpBoundingAABB* self )
{
	*self = zp_move( zpBoundingAABB() );
}
void as_zpBoundingAABB_CopyConstructor( zpBoundingAABB* self, const zpBoundingAABB& copy )
{
	*self = copy;
}
void as_zpBoundingAABB_Deconstructor( zpBoundingAABB* self )
{
	self->~zpBoundingAABB();
}

void as_Register_zpBoundingAABB( asIScriptEngine* engine )
{
	zp_int r;
	zpBoundingAABB f;

	r = engine->RegisterObjectType(      ZP_SCRIPT_BOUNDBOXAABB, sizeof( zpBoundingAABB ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_BOUNDBOXAABB, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpBoundingAABB_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_BOUNDBOXAABB, asBEHAVE_CONSTRUCT, "void f( const " ZP_SCRIPT_BOUNDBOXAABB "& in )", asFUNCTION( as_zpBoundingAABB_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_BOUNDBOXAABB, asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpBoundingAABB_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_BOUNDBOXAABB, "void opAssign( const " ZP_SCRIPT_BOUNDBOXAABB "& in )", asMETHODPR( zpBoundingAABB, operator=, ( const zpBoundingAABB& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( ZP_SCRIPT_BOUNDBOXAABB, "" ZP_SCRIPT_VECTOR4 " get_min() const", asMETHODPR( zpBoundingAABB, getMin, () const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_BOUNDBOXAABB, "void set_min( const " ZP_SCRIPT_VECTOR4 "& in )", asMETHODPR( zpBoundingAABB, setMin, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_BOUNDBOXAABB, "" ZP_SCRIPT_VECTOR4 " get_max() const", asMETHODPR( zpBoundingAABB, getMax, () const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( ZP_SCRIPT_BOUNDBOXAABB, "void set_max( const " ZP_SCRIPT_VECTOR4 "& in )", asMETHODPR( zpBoundingAABB, setMax, ( const zpVector4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );
}
#pragma endregion

#pragma region Register zpGameObject
//zpRefObject< zpObject >* as_Object_Factory()
//{
//	return new zpRefObject< zpObject >;
//}
//
//void as_zpGameObject_Register( asIScriptEngine* engine, zpApplication* app )
//{
//	zp_int r;
//
//	r = engine->RegisterObjectType(      ZP_SCRIPT_OBJECT, sizeof( zpRefObject< zpObject > ), asOBJ_REF ); AS_ASSERT( r );
//	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_OBJECT, asBEHAVE_FACTORY, "" ZP_SCRIPT_OBJECT "@ f()", asFUNCTION( as_Object_Factory ), asCALL_CDECL ); AS_ASSERT( r );
//	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_OBJECT, asBEHAVE_ADDREF, "void f()", asMETHOD( zpRefObject< zpObject >, addRef ), asCALL_THISCALL ); AS_ASSERT( r );
//	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_OBJECT, asBEHAVE_RELEASE, "void f()", asMETHOD( zpRefObject< zpObject >, release ), asCALL_THISCALL ); AS_ASSERT( r );
//
//}
#pragma endregion

#pragma region Register Rendering
template< typename T >
class zpRefObject
{
public:
	zpRefObject()
		: m_refCount( 1 )
	{}
	~zpRefObject()
	{}

	void addRef()
	{
		++m_refCount;
	}
	void release()
	{
		--m_refCount;
		if( m_refCount == 0 ) delete this;
	}

	operator T*()
	{
		return &m_value;
	}
	operator const T*() const
	{
		return &m_value;
	}

	operator T&()
	{
		return m_value;
	}
	operator const T&() const
	{
		return m_value;
	}

private:
	T m_value;
	zp_int m_refCount;
};

zpRefObject< zpMaterialResourceInstance >* as_Material_Factory()
{
	return new zpRefObject< zpMaterialResourceInstance >;
}

zpRefObject< zpFontResourceInstance >* as_Font_Factory()
{
	return new zpRefObject< zpFontResourceInstance >;
}

void as_Register_Rendering( asIScriptEngine* engine, zpApplication* app )
{
	zp_int r;
	zpRenderingContext* i = app->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();
	zpMaterialContentManager* mat = app->getRenderPipeline()->getMaterialContentManager();
	zpFontContentManager* font = app->getRenderPipeline()->getFontContentManager();

	// Material
	r = engine->RegisterObjectType(      ZP_SCRIPT_MATERIAL, sizeof( zpRefObject< zpMaterialResourceInstance > ), asOBJ_REF ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_MATERIAL, asBEHAVE_FACTORY, "" ZP_SCRIPT_MATERIAL "@ f()", asFUNCTION( as_Material_Factory ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_MATERIAL, asBEHAVE_ADDREF, "void f()", asMETHOD( zpRefObject< zpMaterialResourceInstance >, addRef ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_MATERIAL, asBEHAVE_RELEASE, "void f()", asMETHOD( zpRefObject< zpMaterialResourceInstance >, release ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "bool GetMaterial( const " ZP_SCRIPT_STRING "& in, " ZP_SCRIPT_MATERIAL "& )", asMETHODPR( zpMaterialContentManager, getResource, ( const zpString&, zpMaterialResourceInstance& ), zp_bool ), asCALL_THISCALL_ASGLOBAL, mat ); AS_ASSERT( r );

	// Font
	r = engine->RegisterObjectType(      ZP_SCRIPT_FONT, sizeof( zpRefObject< zpFontResourceInstance > ), asOBJ_REF ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_FONT, asBEHAVE_FACTORY, "" ZP_SCRIPT_FONT "@ f()", asFUNCTION( as_Font_Factory ), asCALL_CDECL ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_FONT, asBEHAVE_ADDREF, "void f()", asMETHOD( zpRefObject< zpFontResourceInstance >, addRef ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( ZP_SCRIPT_FONT, asBEHAVE_RELEASE, "void f()", asMETHOD( zpRefObject< zpFontResourceInstance >, release ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "bool GetFont( const " ZP_SCRIPT_STRING "& in, " ZP_SCRIPT_FONT "& )", asMETHODPR( zpFontContentManager, getResource, ( const zpString&, zpFontResourceInstance& ), zp_bool ), asCALL_THISCALL_ASGLOBAL, font ); AS_ASSERT( r );

	// Rendering Queue
	r = engine->RegisterEnum(      ZP_SCRIPT_RENDERING_QUEUE ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "Opaque",				ZP_RENDERING_QUEUE_OPAQUE ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "OpaqueDebug",			ZP_RENDERING_QUEUE_OPAQUE_DEBUG ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "Skybox",				ZP_RENDERING_QUEUE_SKYBOX ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "Transparent",			ZP_RENDERING_QUEUE_TRANSPARENT ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "TransparentDebug",	ZP_RENDERING_QUEUE_TRANSPARENT_DEBUG ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "Overlay",				ZP_RENDERING_QUEUE_OVERLAY ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "UI",					ZP_RENDERING_QUEUE_UI ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_RENDERING_QUEUE, "UIDebug",				ZP_RENDERING_QUEUE_UI_DEBUG ); AS_ASSERT( r );

	// Topology
	r = engine->RegisterEnum(      ZP_SCRIPT_TOPOLOGY ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_TOPOLOGY, "Unknown",		ZP_TOPOLOGY_UNKNOWN ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_TOPOLOGY, "PointList",		ZP_TOPOLOGY_POINT_LIST ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_TOPOLOGY, "LineList",		ZP_TOPOLOGY_LINE_LIST ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_TOPOLOGY, "LineStrip",		ZP_TOPOLOGY_LINE_STRIP ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_TOPOLOGY, "TriangleList",	ZP_TOPOLOGY_TRIANGLE_LIST ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_TOPOLOGY, "TriangleStrip",	ZP_TOPOLOGY_TRIANGLE_STRIP ); AS_ASSERT( r );

	// Vertex Format
	r = engine->RegisterEnum(      ZP_SCRIPT_VERTEX_FORMAT ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_VERTEX_FORMAT, "VertexColor",		ZP_VERTEX_FORMAT_VERTEX_COLOR ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_VERTEX_FORMAT, "VertexUV",			ZP_VERTEX_FORMAT_VERTEX_UV ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_VERTEX_FORMAT, "VertexColorUV",		ZP_VERTEX_FORMAT_VERTEX_COLOR_UV ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_VERTEX_FORMAT, "VertexNormalUV",	ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_VERTEX_FORMAT, "VertexNormalUV2",	ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2 ); AS_ASSERT( r );

	// Immediate Rendering
	r = engine->RegisterGlobalFunction( "void BeginDraw( uint, " ZP_SCRIPT_VERTEX_FORMAT ", " ZP_SCRIPT_TOPOLOGY ", " ZP_SCRIPT_VERTEX_FORMAT ", const " ZP_SCRIPT_MATERIAL "& )", asMETHODPR( zpRenderingContext, beginDrawImmediate, ( zp_uint, zpRenderingQueue, zpTopology, zpVertexFormat, const zpMaterialResourceInstance* ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void EndDraw()", asMETHODPR( zpRenderingContext, endDrawImmediate, (), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void SetBoundingBox( const " ZP_SCRIPT_BOUNDBOXAABB "& in )", asMETHODPR( zpRenderingContext, setBoundingBox, ( const zpBoundingAABB& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void SetBoundingBoxCenter( const " ZP_SCRIPT_VECTOR4 "& in )", asMETHODPR( zpRenderingContext, setBoundingBoxCenter, ( const zpVector4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void SetMatrix( const " ZP_SCRIPT_MATRIX4 "& in )", asMETHODPR( zpRenderingContext, setMatrix, ( const zpMatrix4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void SetSortBias( int )", asMETHODPR( zpRenderingContext, setSortBias, ( zp_ushort ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void AddVertex( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_COLOR "& in )", asMETHODPR( zpRenderingContext, addVertex, ( const zpVector4f&, const zpColor4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddVertex( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR2 "& in )", asMETHODPR( zpRenderingContext, addVertex, ( const zpVector4f&, const zpVector2f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddVertex( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR2 "& in, const " ZP_SCRIPT_COLOR "& in )", asMETHODPR( zpRenderingContext, addVertex, ( const zpVector4f&, const zpVector2f&, const zpColor4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddVertex( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR2 "& in )", asMETHODPR( zpRenderingContext, addVertex, ( const zpVector4f&, const zpVector4f&, const zpVector2f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddVertex( const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR4 "& in, const " ZP_SCRIPT_VECTOR2 "& in, const " ZP_SCRIPT_VECTOR2 "& in )", asMETHODPR( zpRenderingContext, addVertex, ( const zpVector4f&, const zpVector4f&, const zpVector2f&, const zpVector2f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void AddLineIndex( int, int )", asMETHODPR( zpRenderingContext, addLineIndex, ( zp_ushort, zp_ushort ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddTriangleIndex( int, int, int )", asMETHODPR( zpRenderingContext, addTriangleIndex, ( zp_ushort, zp_ushort, zp_ushort ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddQuadIndex( int, int, int, int )", asMETHODPR( zpRenderingContext, addQuadIndex, ( zp_ushort, zp_ushort, zp_ushort, zp_ushort ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	// Font Alignment
	r = engine->RegisterEnum(      ZP_SCRIPT_FONT_ALIGNMENT ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_FONT_ALIGNMENT, "Left",		ZP_FONT_ALIGNMENT_LEFT ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_FONT_ALIGNMENT, "Center",	ZP_FONT_ALIGNMENT_CENTER ); AS_ASSERT( r );
	r = engine->RegisterEnumValue( ZP_SCRIPT_FONT_ALIGNMENT, "Right",	ZP_FONT_ALIGNMENT_RIGHT ); AS_ASSERT( r );

	// Font Rendering
	r = engine->RegisterGlobalFunction( "void BeginFont( uint, " ZP_SCRIPT_RENDERING_QUEUE ", const " ZP_SCRIPT_FONT "& )", asMETHODPR( zpRenderingContext, beginDrawFont, ( zp_uint, zpRenderingQueue, const zpFontResourceInstance* ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void EndFont()", asMETHODPR( zpRenderingContext, endDrawFont, (), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

	r = engine->RegisterGlobalFunction( "void AddText( const " ZP_SCRIPT_STRING "& in, float, const " ZP_SCRIPT_VECTOR2 "& in, FontAlignment, const " ZP_SCRIPT_COLOR "& in )", asMETHODPR( zpRenderingContext, addText, ( const zpString&, zp_float, const zpVector2f&, zpFontAlignment, const zpColor4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );
	r = engine->RegisterGlobalFunction( "void AddText( const " ZP_SCRIPT_STRING "& in, float, const " ZP_SCRIPT_VECTOR2 "& in, FontAlignment, const " ZP_SCRIPT_COLOR "& in, const " ZP_SCRIPT_COLOR "& in )", asMETHODPR( zpRenderingContext, addText, ( const zpString&, zp_float, const zpVector2f&, zpFontAlignment, const zpColor4f&, const zpColor4f& ), void ), asCALL_THISCALL_ASGLOBAL, i ); AS_ASSERT( r );

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

	zp_int r;
	r = engine->SetMessageCallback( asFUNCTION( as_MessageCallback ), 0, asCALL_CDECL ); AS_ASSERT( r );

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