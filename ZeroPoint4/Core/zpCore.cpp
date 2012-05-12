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
