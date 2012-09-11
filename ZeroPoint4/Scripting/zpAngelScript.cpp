#include "zpScripting.h"
#include "zpAngelScript.h"

#define AS_ASSERT( r )	if( (r) < asSUCCESS ) { zpLog::debug() << "Assert Failed: (" << r << ") " << __FILE__ << ':' << __LINE__ << zpLog::endl; }

#pragma region Message Callback
void __onMessageCallback( const asSMessageInfo& info ) {
	zpLogOutput* out;
	switch( info.type ) {
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

#pragma region Register zpString
zpString as_zpString_Factory( asUINT length, const char* string ) {
	return zpString( string, length );
}

void as_zpString_FactoryGeneric( asIScriptGeneric* gen ) {
	asUINT length = gen->GetArgDWord( 0 );
	const char* string = (const char*)gen->GetArgAddress( 1 );

	//new (gen->GetAddressOfReturnLocation()) zpString( string, length );
	*((zpString*)gen->GetAddressOfReturnLocation()) = zp_move( zpString( string, length ) );
}

void as_zpString_Constructor( zpString* self ) {
	*self = zp_move( zpString() );
}
void as_zpString_CopyConstructor( zpString* self, const zpString& copy ) {
	*self = zp_move( zpString( copy ) );
}
void as_zpString_Deconstructor( zpString* self ) {
	self->~zpString();
}

zpString as_zpString_Substring( const zpString* self, zp_uint start, zp_int end ) {
	return self->substring( start, end );
}
zp_int as_zpString_IndexOf( const zpString* self, const zpString& str, zp_uint start ) {
	zp_uint index = self->indexOf( str, start );
	return index == zpString::npos ? -1 : (zp_int)index;
}
zp_int as_zpString_LastIndexOf( const zpString* self, const zpString& str, zp_uint end ) {
	zp_uint index = self->lastIndexOf( str, end );
	return index == zpString::npos ? -1 : (zp_int)index;
}

void as_zpString_Register( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "string", sizeof( zpString ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
	
	r = engine->RegisterStringFactory( "string", asFUNCTION( as_zpString_Factory ), asCALL_CDECL ); AS_ASSERT( r );

	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( as_zpString_Constructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_CONSTRUCT, "void f( const string &in )", asFUNCTION( as_zpString_CopyConstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectBehaviour( "string", asBEHAVE_DESTRUCT, "void f()", asFUNCTION( as_zpString_Deconstructor ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "string& opAssign( const string &in )", asMETHODPR( zpString, operator=, ( const zpString& ), zpString& ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "string", "uint length() const", asMETHODPR( zpString, length, () const, zp_uint ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "bool isEmpty() const", asMETHODPR( zpString, isEmpty, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "uint8& opIndex( uint index )", asMETHODPR( zpString, operator[], ( zp_uint index ), zp_char& ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "const uint8 opIndex( uint index ) const", asMETHOD( zpString, charAt ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "string", "string substring( uint start, int end = -1 ) const", asFUNCTION( as_zpString_Substring ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "int indexOf( const string& in, uint start = 0 ) const", asFUNCTION( as_zpString_IndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "string", "int lastIndexOf( const string& in, uint end = 0 ) const", asFUNCTION( as_zpString_LastIndexOf ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	
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
void as_zpArray_Register( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "array<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE ); AS_ASSERT( r );

}
#pragma endregion

#pragma region Register zpGameObject

void as_zpGameObject_Register( asIScriptEngine* engine ) {

}
#pragma endregion

#pragma region Register zp_real
void as_zp_real_Constructor( zp_real* self ) {
	*self = zp_real_zero();
}
void as_zp_real_ConstructorX( zp_real* self, float value ) {
	*self = zp_real_from_float( value );
}
void as_zp_real_CopyConstructor( zp_real* self, zp_real real ) {
	*self = real;
}
void as_zp_real_Deconstructor( zp_real* self ) {
}

zp_real as_zp_real_Add( zp_real* self, zp_real b ) {
	return zp_real_add( *self, b );
}
zp_real as_zp_real_Sub( zp_real* self, zp_real b ) {
	return zp_real_sub( *self, b );
}
zp_real as_zp_real_Mul( zp_real* self, zp_real b ) {
	return zp_real_mul( *self, b );
}
zp_real as_zp_real_Div( zp_real* self, zp_real b ) {
	return zp_real_div( *self, b );
}

zp_real as_zp_real_Addf( zp_real* self, zp_float b ) {
	return zp_real_add( *self, zp_real_from_float( b ) );
}
zp_real as_zp_real_Subf( zp_real* self, zp_float b ) {
	return zp_real_sub( *self, zp_real_from_float( b ) );
}
zp_real as_zp_real_Mulf( zp_real* self, zp_float b ) {
	return zp_real_mul( *self, zp_real_from_float( b ) );
}
zp_real as_zp_real_Divf( zp_real* self, zp_float b ) {
	return zp_real_div( *self, zp_real_from_float( b ) );
}

zp_real as_zp_real_AddAssign( zp_real* self, zp_real b ) {
	*self = zp_real_add( *self, b );
	return *self;
}
zp_real as_zp_real_SubAssign( zp_real* self, zp_real b ) {
	*self =  zp_real_sub( *self, b );
	return *self;
}
zp_real as_zp_real_MulAssign( zp_real* self, zp_real b ) {
	*self =  zp_real_mul( *self, b );
	return *self;
}
zp_real as_zp_real_DivAssign( zp_real* self, zp_float b ) {
	*self =  zp_real_div( *self, b );
	return *self;
}

zp_real as_zp_real_AddAssignf( zp_real* self, zp_float b ) {
	*self = zp_real_add( *self, zp_real_from_float( b ) );
	return *self;
}
zp_real as_zp_real_SubAssignf( zp_real* self, zp_float b ) {
	*self =  zp_real_sub( *self, zp_real_from_float( b ) );
	return *self;
}
zp_real as_zp_real_MulAssignf( zp_real* self, zp_float b ) {
	*self =  zp_real_mul( *self, zp_real_from_float( b ) );
	return *self;
}
zp_real as_zp_real_DivAssignf( zp_real* self, zp_float b ) {
	*self =  zp_real_div( *self, zp_real_from_float( b ) );
	return *self;
}

zp_real as_zp_real_Neg( zp_real* self ) {
	return zp_real_neg( *self );
}

zp_real as_zp_real_Eq( zp_real* self, zp_real b ) {
	return zp_real_eq( *self, b );
}
zp_real as_zp_real_Eqf( zp_real* self, zp_float b ) {
	return zp_real_eq( *self, zp_real_from_float( b ) );
}

void as_zp_real_Register( asIScriptEngine* engine ) {
	if( !engine ) return;
	zp_int r;

	r = engine->RegisterObjectType( "real", sizeof( zp_real ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK ); AS_ASSERT( r );
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

zp_float as_zpVector4f_GetX( zpVector4f* self ) {
	return zp_real_to_float( self->getX() );
}
zp_float as_zpVector4f_GetY( zpVector4f* self ) {
	return zp_real_to_float( self->getY() );
}
zp_float as_zpVector4f_GetZ( zpVector4f* self ) {
	return zp_real_to_float( self->getZ() );
}
zp_float as_zpVector4f_GetW( zpVector4f* self ) {
	return zp_real_to_float( self->getW() );
}

void as_zpVector4f_SetX( zpVector4f* self, zp_float x ) {
	self->setX( zp_real_from_float( x ) );
}
void as_zpVector4f_SetY( zpVector4f* self, zp_float y ) {
	self->setY( zp_real_from_float( y ) );
}
void as_zpVector4f_SetZ( zpVector4f* self, zp_float z ) {
	self->setZ( zp_real_from_float( z ) );
}
void as_zpVector4f_SetW( zpVector4f* self, zp_float w ) {
	self->setW( zp_real_from_float( w ) );
}

zp_float as_zpVector4f_Dot3( zpVector4f* self, const zpVector4f& vector ) {
	return zp_real_to_float( self->dot3( vector ) );
}
zp_float as_zpVector4f_Dot4( zpVector4f* self, const zpVector4f& vector ) {
	return zp_real_to_float( self->dot4( vector ) );
}

zp_float as_zpVector4f_Magnitude3( zpVector4f* self ) {
	return zp_real_to_float( self->magnitude3() );
}
zp_float as_zpVector4f_Magnitude4( zpVector4f* self ) {
	return zp_real_to_float( self->magnitude4() );
}
zp_float as_zpVector4f_MagnitudeSquared3( zpVector4f* self ) {
	return zp_real_to_float( self->magnitudeSquared3() );
}
zp_float as_zpVector4f_MagnitudeSquared4( zpVector4f* self ) {
	return zp_real_to_float( self->magnitudeSquared4() );
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

	r = engine->RegisterObjectMethod( "Vector4f", "float get_x() const", asFUNCTION( as_zpVector4f_GetX ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "float get_y() const", asFUNCTION( as_zpVector4f_GetY ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "float get_z() const", asFUNCTION( as_zpVector4f_GetZ ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "float get_w() const", asFUNCTION( as_zpVector4f_GetW ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "void set_x( float )", asFUNCTION( as_zpVector4f_GetX ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void set_y( float )", asFUNCTION( as_zpVector4f_GetY ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void set_z( float )", asFUNCTION( as_zpVector4f_GetZ ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void set_w( float )", asFUNCTION( as_zpVector4f_GetW ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "void zero3()", asMETHODPR( zpVector4f, zero3, (), void ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "void zero4()", asMETHODPR( zpVector4f, zero4, (), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "bool isZero3() const", asMETHODPR( zpVector4f, isZero3, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "bool isZero4() const", asMETHODPR( zpVector4f, isZero4, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "float dot3( const Vector4f& in ) const", asFUNCTION( as_zpVector4f_Dot3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "float dot4( const Vector4f& in ) const", asFUNCTION( as_zpVector4f_Dot4 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "Vector4f cross3( const Vector4f& in ) const", asMETHODPR( zpVector4f, cross3, ( const zpVector4f& ) const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "Vector4f cross4( const Vector4f& in ) const", asMETHODPR( zpVector4f, cross4, ( const zpVector4f& ) const, zpVector4f ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "float magnitude3() const", asFUNCTION( as_zpVector4f_Magnitude3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "float magnitude4() const", asFUNCTION( as_zpVector4f_Magnitude4 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Vector4f", "float magnitudeSquared3() const", asFUNCTION( as_zpVector4f_MagnitudeSquared3 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Vector4f", "float magnitudeSquared4() const", asFUNCTION( as_zpVector4f_MagnitudeSquared4 ), asCALL_CDECL_OBJFIRST ); AS_ASSERT( r );

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

	r = engine->RegisterObjectMethod( "Matrix4f", "Matrix4f opMul( const Matrix4f& in ) const", asMETHODPR( zpMatrix4f, operator*, ( const zpMatrix4f& ) const, zpMatrix4f ), asCALL_THISCALL ); AS_ASSERT( r );
	r = engine->RegisterObjectMethod( "Matrix4f", "void opMulAssign( const Matrix4f& in ) const", asMETHODPR( zpMatrix4f, operator*=, ( const zpMatrix4f& ), void ), asCALL_THISCALL ); AS_ASSERT( r );

	r = engine->RegisterObjectMethod( "Matrix4f", "bool isIdentity() const", asMETHODPR( zpMatrix4f, isIdentity, () const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
	
	r = engine->RegisterObjectMethod( "Matrix4f", "bool opEquals( const Matrix4f& in ) const", asMETHODPR( zpMatrix4f, operator==, ( const zpMatrix4f& ) const, zp_bool ), asCALL_THISCALL ); AS_ASSERT( r );
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
	
	as_zp_real_Register( s_engine );
	as_zpString_Register( s_engine );
	//as_zpArray_Register( s_engine );
	as_zpVector4f_Register( s_engine );
	as_zpMatrix4f_Register( s_engine );

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
