#include "zpCore.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

void zp_printf( const zp_char* text, ... ) {
	va_list vl;
	va_start( vl, text );
#if ZP_USE_SAFE_FUNCTIONS
	vprintf_s( text, vl );
#else
	vprint( text, v1 );
#endif
	va_end( vl );
}
void zp_printfln( const zp_char* text, ... ) {
	va_list vl;
	va_start( vl, text );
#if ZP_USE_SAFE_FUNCTIONS
	vprintf_s( text, vl );
	printf_s( "\n" );
#else
	vprint( text, v1 );
	printf( "\n" );
#endif
	va_end( vl );
}
void zp_snprintf( zp_char* dest, zp_uint destSize, zp_uint maxCount, const zp_char* format, ... ) {
	va_list vl;
	va_start( vl, format );
#if ZP_USE_SAFE_FUNCTIONS
	vsnprintf_s( dest, destSize, maxCount, format, vl );
#else
	vsnprintf( dest, destSize, format, vl );
#endif
	va_end( vl );
}

void* zp_malloc( zp_uint size ) {
	return malloc( size );
}
void* zp_calloc( zp_uint num, zp_uint size ) {
	return calloc( num, size );
}
void* zp_realloc( void* ptr, zp_uint size ) {
	return realloc( ptr, size );
}
void zp_free( void* ptr ) {
	free( ptr );
}

void* zp_memcpy( void* dest, zp_uint destSize, const void* src, zp_uint size ) {
#if ZP_USE_SAFE_FUNCTIONS
	memcpy_s( dest, destSize, src, size );
	return dest;
#else
	return memcpy( dest, src, size );
#endif
}
void* zp_memmove( void* dest, zp_uint destSize, const void* src, zp_uint size ) {
#if ZP_USE_SAFE_FUNCTIONS
	memmove_s( dest, destSize, src, size );
	return dest;
#else
	return memmove( dest, src, size );
#endif
}
void* zp_memset( void* dest, zp_int value, zp_uint size ) {
	return memset( dest, value, size );
}

zp_char* zp_strcpy( zp_char* destString, zp_uint numElements, const zp_char* srcString ) {
#if ZP_USE_SAFE_FUNCTIONS
	return strcpy_s( destString, numElements, srcString ) == 0 ? destString : ZP_NULL;
#else
	return strcpy( destString, numElements, srcString );
#endif
}
zp_uint zp_strlen( const zp_char* srcString ) {
	return strlen( srcString );
}
zp_int zp_strcmp( const zp_char* str1, const zp_char* str2 ) {
	return strcmp( str1, str2 );
}
zp_char* zp_strstr( zp_char* str, const zp_char* subStr ) {
	return strstr( str, subStr );
}
const zp_char* zp_strstr(  const zp_char* str, const zp_char* subStr ) {
	return strstr( str, subStr );
}

zp_int zp_atoi( const zp_char* str ) {
	return atoi( str );
}
zp_float zp_atof( const zp_char* str ) {
	return (zp_float)atof( str );
}

zp_int zp_rand() {
	return rand();
}
zp_float zp_randf() {
	zp_int base = zp_rand();
	return (zp_float)( zp_rand() % base ) / (zp_float)base;
}
void zp_srand( zp_uint seed ) {
	srand( seed );
}

zp_char zp_to_lower( zp_char ch ) {
	return ( 'A' <= ch && ch <= 'Z' ) ? ch -= 'A' - 'a' : ch;
}
zp_char zp_to_upper( zp_char ch ) {
	return ( 'a' <= ch && ch <= 'z' ) ? ch -= 'a' - 'A' : ch;
}

zp_bool zp_is_ctrl( zp_char ch ) {
	return ch < 0x1F || ch == 0x7F;
}
zp_bool zp_is_whitespace( zp_char ch ) {
	return ( ch >= 0x09 && ch <= 0x0D ) || ch == 0x20;
}
zp_bool zp_is_upper( zp_char ch ) {
	return ch >= 0x41 && ch <= 0x5A;
}
zp_bool zp_is_lower( zp_char ch ) {
	return ch >= 0x61 && ch <= 0x7A;
}
zp_bool zp_is_alpha( zp_char ch ) {
	return zp_is_upper( ch ) || zp_is_lower( ch );
}
zp_bool zp_is_digit( zp_char ch ) {
	return ch >= 0x30 && ch <= 0x39;
}
zp_bool zp_is_xdigit( zp_char ch ) {
	return zp_is_digit( ch ) || ( ch >= 0x41 && ch <= 0x46 ) || ( ch >= 0x61 && ch <= 0x66 );
}
zp_bool zp_is_alpha_numeric( zp_char ch ) {
	return zp_is_alpha( ch ) || zp_is_digit( ch );
}
zp_bool zp_is_punctuation( zp_char ch ) {
	return ( ch >= 0x21 && ch <= 0x2F ) || ( ch >= 0x3A && ch <= 0x40 ) || ( ch >= 0x5B && ch <= 0x60 ) || ( ch >= 0x7B && ch <= 0x7E );
}
zp_bool zp_is_graphic( zp_char ch ) {
	return ch >= 0x21 && ch <= 0x7E;
}
zp_bool zp_is_print( zp_char ch ) {
	return ch >= 0x20 && ch <= 0x7E;
}

zp_uint zp_near_pow2( zp_uint number ) {
	zp_uint i = 1;
	while( i < number ) i <<= 1;
	return i;
}