#include "zpCore.h"
#include <stdarg.h>
#include <stdio.h>

void zp_printf( const char* text, ... ) {
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
}
void zp_printfln( const char* text, ... ) {
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