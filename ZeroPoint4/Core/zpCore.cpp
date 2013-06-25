#include "zpCore.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

enum
{
	ZP_PRINT_BUFFER_SIZE = 1024,
};

#if ZP_USE_ASSERTIONS

#if ZP_WIN_32 || ZP_WIN_64
#include <Windows.h>

void zp_assert( const zp_char* file, zp_int line, const zp_char* msg, ... )
{
	zp_char message[1024];
	zp_char text[256];
	zp_char title[256];

	va_list vl;
	va_start( vl, msg );
#if ZP_USE_SAFE_FUNCTIONS
	vsnprintf_s( message, sizeof( message ), sizeof( message ), msg, vl );
#else
	vsnprintf( message, sizeof( message ), msg, vl );
#endif
	va_end( vl );

	zp_snprintf( text, sizeof( text ), sizeof( text ), "%s\n\nDebug Break (Abort)  Continue (Retry)  Ignore", message );
		
	zpString filename( file );
	zpFile::convertToFilePath( filename );
	zp_int s = filename.lastIndexOf( zpFile::sep );
	s = s == zpString::npos ? 0 : s;

	const zp_char* f = filename.str() + s;
	zp_snprintf( title, sizeof( title ), sizeof( title ), "ZeroPoint Assert Failed at %s:%d", f, line );

	zp_int result = MessageBox( ZP_NULL, text, title, MB_ABORTRETRYIGNORE | MB_ICONERROR );
	
	switch( result ) {
	case IDOK:
	case IDYES:
	case IDABORT:
		__debugbreak();
		break;
	case IDNO:
	case IDRETRY:
		// continue
		break;
	case IDCANCEL:
	case IDIGNORE:
		// ignore
		break;
	}
}

void zp_assert_warning( const zp_char* file, zp_int line, const zp_char* msg, ... )
{
	zp_char message[1024];
	zp_char text[256];
	zp_char title[256];

	va_list vl;
	va_start( vl, msg );
#if ZP_USE_SAFE_FUNCTIONS
	vsnprintf_s( message, sizeof( message ), sizeof( message ), msg, vl );
#else
	vsnprintf( message, sizeof( message ), msg, vl );
#endif
	va_end( vl );

	zp_snprintf( text, sizeof( text ), sizeof( text ), "%s\n\nDebug Break (Retry)  Continue (Cancel)", message );

	zpString filename( file );
	zpFile::convertToFilePath( filename );
	zp_int s = filename.lastIndexOf( zpFile::sep );
	s = s == zpString::npos ? 0 : s;

	const zp_char* f = filename.str() + s;
	zp_snprintf( title, sizeof( title ), sizeof( title ), "ZeroPoint Assert Warning at %s:%d", f, line );

	zp_int result = MessageBox( ZP_NULL, text, title, MB_RETRYCANCEL | MB_ICONWARNING );

	switch( result )
	{
	case IDRETRY:
		__debugbreak();
		break;
	case IDCANCEL:
		// ignore
		break;
	}

}

#endif

#endif

#if ZP_USE_PRINT
void zp_printf( const zp_char* text, ... )
{
	zp_char buff[ ZP_PRINT_BUFFER_SIZE ];
	
	va_list vl;
	va_start( vl, text );
#if ZP_USE_SAFE_FUNCTIONS
	vsprintf_s( buff, text, vl );
	printf_s( buff );
#else
	vsprintf( buff, text, vl );
	printf( buff );
#endif
	va_end( vl );

	OutputDebugString( buff );
}
void zp_printfln( const zp_char* text, ... )
{
	zp_char buff[ ZP_PRINT_BUFFER_SIZE ];

	va_list vl;
	va_start( vl, text );
#if ZP_USE_SAFE_FUNCTIONS
	zp_int end = vsprintf_s( buff, text, vl );
	sprintf_s( buff + end, sizeof( buff ) - end, "\n" );
	printf_s( buff );
#else
	zp_int end = vsprintf( buff, text, vl );
	sprintf( buff + end, "\n" );
	printf( buff );
#endif
	va_end( vl );

	OutputDebugString( buff );
}
#endif

void zp_snprintf( zp_char* dest, zp_uint destSize, zp_uint maxCount, const zp_char* format, ... )
{
	va_list vl;
	va_start( vl, format );
#if ZP_USE_SAFE_FUNCTIONS
	vsnprintf_s( dest, destSize, maxCount, format, vl );
#else
	vsnprintf( dest, destSize, format, vl );
#endif
	va_end( vl );
}

void* zp_malloc( zp_uint size )
{
	return malloc( size );
}
void* zp_calloc( zp_uint num, zp_uint size )
{
	return calloc( num, size );
}
void* zp_realloc( void* ptr, zp_uint size )
{
	return realloc( ptr, size );
}

void* zp_aligned_malloc( zp_uint size, zp_uint alignment )
{
	return _aligned_malloc( size, alignment );
}
void* zp_aligned_calloc( zp_uint size, zp_uint count, zp_uint alignment )
{
	return _aligned_recalloc( ZP_NULL, count, size, alignment );
}
void* zp_aligned_realloc( void* ptr, zp_uint size, zp_uint alignment )
{
	return _aligned_realloc( ptr, size, alignment );
}
zp_uint zp_aligned_memsize( void* ptr, zp_uint alignment )
{
	return _aligned_msize( ptr, alignment, 0 );
}

void zp_free( void* ptr ) {
	free( ptr );
}
void zp_aligned_free( void* ptr ) {
	_aligned_free( ptr );
}

void* zp_memcpy( void* dest, zp_uint destSize, const void* src, zp_uint size )
{
#if ZP_USE_SAFE_FUNCTIONS
	memcpy_s( dest, destSize, src, size );
	return dest;
#else
	return memcpy( dest, src, size );
#endif
}
void* zp_memmove( void* dest, zp_uint destSize, const void* src, zp_uint size )
{
#if ZP_USE_SAFE_FUNCTIONS
	memmove_s( dest, destSize, src, size );
	return dest;
#else
	return memmove( dest, src, size );
#endif
}
void* zp_memset( void* dest, zp_int value, zp_uint size )
{
	return memset( dest, value, size );
}

zp_char* zp_strcpy( zp_char* destString, zp_uint numElements, const zp_char* srcString )
{
#if ZP_USE_SAFE_FUNCTIONS
	return strcpy_s( destString, numElements, srcString ) == 0 ? destString : ZP_NULL;
#else
	return strcpy( destString, numElements, srcString );
#endif
}
zp_char* zp_strncpy( zp_char* destString, zp_uint numElements, const zp_char* srcString, zp_uint maxCount )
{
#if ZP_USE_SAFE_FUNCTIONS
	strncpy_s( destString, numElements, srcString, maxCount );
	return destString;
#else
	ZP_UNUSED( numElements );
	return strncpy( destString, srcString, maxCount );
#endif
}
zp_uint zp_strlen( const zp_char* srcString )
{
	return srcString ? strlen( srcString ) : 0;
}
zp_int zp_strcmp( const zp_char* str1, const zp_char* str2 )
{
	return strcmp( str1, str2 );
}
zp_char* zp_strstr( zp_char* str, const zp_char* subStr )
{
	return strstr( str, subStr );
}
const zp_char* zp_strstr(  const zp_char* str, const zp_char* subStr )
{
	return strstr( str, subStr );
}

zp_int zp_atoi( const zp_char* str )
{
	return atoi( str );
}
zp_float zp_atof( const zp_char* str )
{
	return (zp_float)atof( str );
}
zp_float zp_atof( const zp_char* str, zp_char** end )
{
	return (zp_float)strtod( str, end );
}
zp_long zp_atol( const zp_char* str )
{
	return atol( str );
}
zp_long zp_atol( const zp_char* str, zp_char** end, zp_int base )
{
	return strtol( str, end, base );
}

zp_int zp_rand()
{
	return rand();
}
zp_float zp_randf()
{
	zp_int base = zp_rand();
	return (zp_float)( zp_rand() % base ) / (zp_float)base;
}
void zp_srand( zp_uint seed ) {
	srand( seed );
}

zp_char zp_to_lower( zp_char ch )
{
	return ( 'A' <= ch && ch <= 'Z' ) ? ch -= 'A' - 'a' : ch;
}
zp_char zp_to_upper( zp_char ch )
{
	return ( 'a' <= ch && ch <= 'z' ) ? ch -= 'a' - 'A' : ch;
}

zp_bool zp_is_ctrl( zp_char ch )
{
	return ch < 0x1F || ch == 0x7F;
}
zp_bool zp_is_whitespace( zp_char ch )
{
	return ( ch >= 0x09 && ch <= 0x0D ) || ch == 0x20;
}
zp_bool zp_is_upper( zp_char ch )
{
	return ch >= 0x41 && ch <= 0x5A;
}
zp_bool zp_is_lower( zp_char ch )
{
	return ch >= 0x61 && ch <= 0x7A;
}
zp_bool zp_is_alpha( zp_char ch )
{
	return zp_is_upper( ch ) || zp_is_lower( ch );
}
zp_bool zp_is_digit( zp_char ch )
{
	return ch >= 0x30 && ch <= 0x39;
}
zp_bool zp_is_xdigit( zp_char ch )
{
	return zp_is_digit( ch ) || ( ch >= 0x41 && ch <= 0x46 ) || ( ch >= 0x61 && ch <= 0x66 );
}
zp_bool zp_is_alpha_numeric( zp_char ch )
{
	return zp_is_alpha( ch ) || zp_is_digit( ch );
}
zp_bool zp_is_punctuation( zp_char ch )
{
	return ( ch >= 0x21 && ch <= 0x2F ) || ( ch >= 0x3A && ch <= 0x40 ) || ( ch >= 0x5B && ch <= 0x60 ) || ( ch >= 0x7B && ch <= 0x7E );
}
zp_bool zp_is_graphic( zp_char ch )
{
	return ch >= 0x21 && ch <= 0x7E;
}
zp_bool zp_is_print( zp_char ch )
{
	return ch >= 0x20 && ch <= 0x7E;
}

zp_uint zp_near_pow2( zp_uint number )
{
	zp_uint i = 1;
	while( i < number ) i <<= 1;
	return i;
}

void zp_sleep( zp_int milliseconds )
{
#if ZP_WIN_32 || ZP_WIN_64
	Sleep( milliseconds );
#endif
}

zp_hash zp_fnv1_32_data( const void* d, zp_uint l, zp_hash h )
{
	const zp_byte* s = (const zp_byte*)d;
	
	while( l-- )
	{
		h *= (zp_hash)0x01000193;
		h ^= (zp_hash)*s++;
	}

	return h;
}
zp_hash zp_fnv1_32_string( const zp_char* c, zp_hash h )
{
	const zp_byte* s = (const zp_byte*)c;
	
	while( *s )
	{
		h *= (zp_hash)0x01000193;
		h ^= (zp_hash)*s++;
	}

	return h;
}

const zp_char toBase64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const zp_byte fromBase64Chars[] = {
	0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40
	0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, // 80
	16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49, // 120
	50,51,
};
const zp_char padCharacter = '=';

zp_bool zp_base64_encode( const void* data, zp_uint length, zpStringBuffer& outEncode )
{
	const zp_byte* encode = (const zp_byte*)data;
	zp_uint padding = length % 3;
	zp_uint count = length / 3;
	zp_dword n;

	outEncode.clear();
	outEncode.reserve( ( count + !!padding ) * 4 );

	for( zp_uint i = 0; i < count; ++i )
	{
		n  = (*encode++) << 16;
		n += (*encode++) << 8;
		n += (*encode++) << 0;

		outEncode.append( toBase64Chars[ ( n & 0x00FC0000 ) >> 18 ] );
		outEncode.append( toBase64Chars[ ( n & 0x0003F000 ) >> 12 ] );
		outEncode.append( toBase64Chars[ ( n & 0x00000FC0 ) >> 6  ] );
		outEncode.append( toBase64Chars[ ( n & 0x0000003F ) >> 0  ] );
	}

	switch( padding )
	{
	case 1:
		{
			n  = (*encode++) << 16;
			outEncode.append( toBase64Chars[ ( n & 0x00FC0000 ) >> 18 ] );
			outEncode.append( toBase64Chars[ ( n & 0x0003F000 ) >> 12 ] );
			outEncode.append( padCharacter );
			outEncode.append( padCharacter );
		}
		break;
	case 2:
		{
			n  = (*encode++) << 16;
			n += (*encode++) << 8;
			outEncode.append( toBase64Chars[ ( n & 0x00FC0000 ) >> 18 ] );
			outEncode.append( toBase64Chars[ ( n & 0x0003F000 ) >> 12 ] );
			outEncode.append( toBase64Chars[ ( n & 0x00000FC0 ) >> 6  ] );
			outEncode.append( padCharacter );
		}
		break;
	default:
		break;
	}

	return true;
}
zp_bool zp_base64_decode( const zp_char* data, zp_uint length, zpDataBuffer& outDecode )
{
	if( ( length % 4 ) != 0 || length == 0 )
	{
		return false;
	}

	zp_uint padding = 0;
	padding += data[ length - 1 ] == padCharacter ? 1 : 0;
	padding += data[ length - 2 ] == padCharacter ? 1 : 0;

	outDecode.reset();
	outDecode.reserve( ( ( length / 4 ) * 3 ) - padding );

	zp_uint count = length - 4;
	zp_dword n;
	for( zp_uint i = 0; i <= count; i += 4 )
	{
		n  = fromBase64Chars[ data[ i + 0 ] ] << 18;
		n += fromBase64Chars[ data[ i + 1 ] ] << 12;
		n += fromBase64Chars[ data[ i + 2 ] ] << 6;
		n += fromBase64Chars[ data[ i + 3 ] ] << 0;

		outDecode.write< zp_char >( ( n >> 16 ) & 0xFF );
		outDecode.write< zp_char >( ( n >> 8  ) & 0xFF );
		outDecode.write< zp_char >( ( n >> 0  ) & 0xFF );
	}

	switch( padding )
	{
	case 1:
		{
			n  = fromBase64Chars[ data[ count + 0 ] ] << 18;
			n += fromBase64Chars[ data[ count + 1 ] ] << 12;
			n += fromBase64Chars[ data[ count + 2 ] ] << 6;

			outDecode.write< zp_char >( ( n >> 16 ) & 0xFF );
			outDecode.write< zp_char >( ( n >> 8  ) & 0xFF );
		}
		break;
	case 2:
		{
			n  = fromBase64Chars[ data[ count + 0 ] ] << 18;
			n += fromBase64Chars[ data[ count + 1 ] ] << 12;

			outDecode.write< zp_char >( ( n >> 16 ) & 0xFF );
		}
		break;
	default:
		break;
	}

	return true;
}

void zpCoreRegisterSerializables() {
	//zpRegisterSerializable::registerSerializable<zpGame>();
	//zpRegisterSerializable::registerSerializable<zpWindow>();
	//
	//zpRegisterSerializable::registerSerializable<zpWorld>();
	//zpRegisterSerializable::registerSerializable<zpGameObject>();
	//
	//zpRegisterSerializable::registerSerializable<zpProperties>();
}