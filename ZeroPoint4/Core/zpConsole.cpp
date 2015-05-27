#include "zpCore.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

zpConsole::zpConsole()
	: m_isCreated( false )
	, m_bufferSize( ZP_CONSOLE_DEFAULT_BUFFER_SIZE )
	, m_currentColor( ZP_CONSOLE_DEFAULT_COLOR )
	, m_defaultColor( ZP_CONSOLE_DEFAULT_COLOR )
	, m_hConsole( ZP_NULL )
{}
zpConsole::~zpConsole() {
	destroy();
}

zpConsole* zpConsole::getInstance() {
	static zpConsole console;
	return &console;
}

zpString zpConsole::getTitle() const
{
	zp_uint n = 0;
	zp_char title[ 255 ];

	if( m_isCreated )
	{
		n = GetConsoleTitle( title, 255 );
	}

	title[ n ] = '\0';

	return zpString( title );
}
void zpConsole::setTitle( const zpString& title )
{
	setTitle( title.str() );
}
void zpConsole::setTitle( const zp_char* title )
{
	if( m_isCreated )
	{
		SetConsoleTitle( title );
	}
}

void zpConsole::create( zp_ushort bufferSize ) {
	if( m_isCreated ) return;

	int hConHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// grab STD handles
	HANDLE m_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	HANDLE m_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	HANDLE m_hStdErr = GetStdHandle( STD_ERROR_HANDLE );

	// set the static console to the standard out
	m_hConsole = m_hStdOut;

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo( m_hStdOut, &coninfo );

	coninfo.dwSize.Y = m_bufferSize;

	SetConsoleScreenBufferSize( m_hStdOut, coninfo.dwSize );

	// redirect unbuffered STDOUT to the console
	hConHandle = _open_osfhandle( (intptr_t)m_hStdOut, _O_TEXT );

	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	hConHandle = _open_osfhandle( (intptr_t)m_hStdIn, _O_TEXT );

	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	hConHandle = _open_osfhandle( (intptr_t)m_hStdErr, _O_TEXT );

	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	//std::ios::sync_with_stdio();

	m_isCreated = true;
}
void zpConsole::destroy() {
	if( !m_isCreated ) return;

	FreeConsole();

	m_isCreated = false;
}

zp_bool zpConsole::isCreated() const {
	return m_isCreated;
}

void zpConsole::setColor( zpConsoleColorType color ) {
	m_currentColor = color;
	if( m_isCreated ) {
		SetConsoleTextAttribute( m_hConsole, m_currentColor );
	}
}
void zpConsole::setDefaultColor( zpConsoleColorType color ) {
	m_defaultColor = color;
}

zpConsoleColorType zpConsole::getColor() const {
	return m_currentColor;
}
zpConsoleColorType zpConsole::getDefaultColor() const {
	return m_defaultColor;
}

void zpConsole::resetColor() {
	setColor( m_defaultColor );
}

void* zpConsole::getConsoleHandle() {
	return m_hConsole;
}

void zp_printfc( zpConsoleColorType color, const char* text, ... ) {
#if ZP_USE_COLOR_CONSOLE
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE handle = zpConsole::getInstance()->getConsoleHandle();

	GetConsoleScreenBufferInfo( handle, &info );	// get the current console info

	SetConsoleTextAttribute( handle, color );	// set the console text color
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
	SetConsoleTextAttribute( handle, info.wAttributes );	// reset the console text color
#else
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
#endif
}
void zp_printfcln( zpConsoleColorType color, const char* text, ... ) {
#if ZP_USE_COLOR_CONSOLE
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE handle = zpConsole::getInstance()->getConsoleHandle();

	GetConsoleScreenBufferInfo( handle, &info );	// get the current console info

	SetConsoleTextAttribute( handle, color );	// set the console text color
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
	SetConsoleTextAttribute( handle, info.wAttributes );	// reset the console text color
#else
	va_list vl;
	va_start( vl, text );
	vprintf_s( text, vl );
	va_end( vl );
#endif
	printf_s( "\n" );
}