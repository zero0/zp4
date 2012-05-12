#include "zpCore.h"
#include <stdarg.h>
#include <stdio.h>

void ZP_INLINE zp_printf( const char* text, ... ) {
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
}
void ZP_INLINE zp_vprintf( const char* text, va_list args ) {
	vprintf_s( text, args );
}
void ZP_INLINE zp_printfln( const char* text, ... ) {
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
	printf_s( "\n" );
}
void ZP_INLINE zp_vprintfln( const char* text, va_list args ) {
	vprintf_s( text, args );
	printf_s( "\n" );
}