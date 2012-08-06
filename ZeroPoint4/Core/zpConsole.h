#pragma once
#ifndef ZP_CONSOLE_H
#define ZP_CONSOLE_H

typedef zp_ushort zpConsoleColorType;
enum zpConsoleColor {
	ZP_CC_RED =				0x0004,
	ZP_CC_GREEN =			0x0002,
	ZP_CC_BLUE =			0x0001,

	ZP_CC_YELLOW =			ZP_CC_RED | ZP_CC_GREEN,
	ZP_CC_MAGENTA =			ZP_CC_RED | ZP_CC_BLUE,
	ZP_CC_CYAN =			ZP_CC_GREEN | ZP_CC_BLUE,

	ZP_CC_LIGHT_RED =		0x0008 | ZP_CC_RED,
	ZP_CC_LIGHT_GREEN =		0x0008 | ZP_CC_GREEN,
	ZP_CC_LIGHT_BLUE =		0x0008 | ZP_CC_BLUE,

	ZP_CC_LIGHT_YELLOW =	0x0008 | ZP_CC_YELLOW,
	ZP_CC_LIGHT_MAGENTA =	0x0008 | ZP_CC_MAGENTA,
	ZP_CC_LIGHT_CYAN =		0x0008 | ZP_CC_CYAN,

	ZP_CC_BLACK =			0,
	ZP_CC_GRAY =			ZP_CC_RED | ZP_CC_GREEN | ZP_CC_BLUE,
	ZP_CC_DARK_GRAY =		0x0008 | ZP_CC_BLACK,
	ZP_CC_WHITE =			0x0008 | ZP_CC_GRAY,
};
#define ZP_CC( f, b ) (zpConsoleColorType)( ( (b) << 4 ) | (f) )

#define ZP_CONSOLE_DEFAULT_BUFFER_SIZE	1024
#define ZP_CONSOLE_DEFAULT_COLOR		ZP_CC( ZP_CC_GRAY, ZP_CC_BLACK )

class zpConsole {
public:
	~zpConsole();

	static zpConsole* getInstance();

	const zpString& getTitle() const;
	void setTitle( const zpString& title );

	void create( zp_ushort bufferSize = ZP_CONSOLE_DEFAULT_BUFFER_SIZE );
	void destroy();

	zp_bool isCreated() const;

	void setColor( zpConsoleColorType color );
	void setDefaultColor( zpConsoleColorType color );

	zpConsoleColorType getColor() const;
	zpConsoleColorType getDefaultColor() const;

	void resetColor();

	void* getConsoleHandle();

	//void serialize( ISerializedOutput* const out );
	//void deserialize( ISerializedInput* const in );

private:
	zpConsole();
	
	zp_bool m_isCreated;
	zp_ushort m_bufferSize;
	
	//HANDLE m_hStdOut;
	//HANDLE m_hStdErr;
	//HANDLE m_hStdIn;

	zpConsoleColorType m_currentColor;
	zpConsoleColorType m_defaultColor;

	void* m_hConsole;
	
	zpString m_title;
};

void zp_printfc( zpConsoleColorType color, const char* text, ... );
void zp_printfcln( zpConsoleColorType color, const char* text, ... );

#endif