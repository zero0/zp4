#include "zpCore.h"
#include <stdarg.h>
#include <stdio.h>

void zp_printf( const zp_char* text, ... ) {
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
}
void zp_printfln( const zp_char* text, ... ) {
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
	printf_s( "\n" );
}

zp_uint zp_near_pow2( zp_uint number ) {
	zp_uint i = 1;
	while( i < number ) i <<= 1;
	return i;
}

zp_char zp_to_lower( zp_char ch ) {
	return ( 'A' <= ch && ch <= 'Z' ) ? ch -= 'A' - 'a' : ch;
}

zp_char zp_to_upper( zp_char ch ) {
	return ( 'a' <= ch && ch <= 'z' ) ? ch -= 'a' - 'A' : ch;
}
zp_bool zp_is_whitespace( zp_char ch ) {
	switch( ch ) {
	case 0x09:
	case 0x0A:
	case 0x0B:
	case 0x0C:
	case 0x0D:
	case 0x20:
		return true;
	default:
		return false;
	}
}