#pragma once
#ifndef ZP_LOG_H
#define ZP_LOG_H

enum zpLogLevelType
{
    ZP_LOG_LEVEL_MESSAGE =    0x01,
    ZP_LOG_LEVEL_DEBUG =    0x02,
    ZP_LOG_LEVEL_NOTIFY =    0x04,
    ZP_LOG_LEVEL_WARNING =    0x08,
    ZP_LOG_LEVEL_EXCEPTION =0x10,
    ZP_LOG_LEVEL_ERROR =    0x20,
    ZP_LOG_LEVEL_CRITICAL =    0x40,
    ZP_LOG_LEVEL_FATAL =    0x80,
};

enum zpLogOutputType
{
    ZP_LOG_OUTPUT_NONE,
    ZP_LOG_OUTPUT_STD,
    ZP_LOG_OUTPUT_FILE,
    ZP_LOG_OUTPUT_BUFFER,
};

#define ZP_LOG_LEVEL_GENERATE( NAME, TXT, COLOR )                \
struct NAME {                                                    \
    static ZP_FORCE_INLINE zpLogLevelType getLogLevelType() {    \
        enum LowerLogLevel {                                    \
            ZP_LOG_LEVEL_Message =    ZP_LOG_LEVEL_MESSAGE,        \
            ZP_LOG_LEVEL_Debug =    ZP_LOG_LEVEL_DEBUG,            \
            ZP_LOG_LEVEL_Notify =    ZP_LOG_LEVEL_NOTIFY,        \
            ZP_LOG_LEVEL_Warning =    ZP_LOG_LEVEL_WARNING,        \
            ZP_LOG_LEVEL_Exception =ZP_LOG_LEVEL_EXCEPTION,        \
            ZP_LOG_LEVEL_Error =    ZP_LOG_LEVEL_ERROR,            \
            ZP_LOG_LEVEL_Critical =    ZP_LOG_LEVEL_CRITICAL,        \
            ZP_LOG_LEVEL_Fatal =    ZP_LOG_LEVEL_FATAL            \
        };                                                        \
        return (zpLogLevelType)ZP_LOG_LEVEL_ ## NAME;            \
    }                                                            \
    static ZP_FORCE_INLINE const zp_char* getLogLevelText() {    \
        return TXT;                                                \
    }                                                            \
    static ZP_FORCE_INLINE zpConsoleColorType getLogLevelColor() {    \
        return COLOR;                                            \
    }                                                            \
};



struct zpLogLevel
{
    ZP_LOG_LEVEL_GENERATE( Message,        "[ MSG ]   ", ZP_CC( ZP_CC_WHITE, ZP_CC_BLACK ) );
    ZP_LOG_LEVEL_GENERATE( Debug,        "[ DEBUG ] ", ZP_CC( ZP_CC_LIGHT_BLUE, ZP_CC_BLACK ) );
    ZP_LOG_LEVEL_GENERATE( Notify,        "[ NOTIF ] ", ZP_CC( ZP_CC_LIGHT_YELLOW, ZP_CC_BLACK ) );
    ZP_LOG_LEVEL_GENERATE( Warning,        "[ WARN ]  ", ZP_CC( ZP_CC_YELLOW, ZP_CC_BLACK ) );
    ZP_LOG_LEVEL_GENERATE( Exception,    "[ EXCEP ] ", ZP_CC( ZP_CC_MAGENTA, ZP_CC_BLACK ) );
    ZP_LOG_LEVEL_GENERATE( Error,        "[ ERROR ] ", ZP_CC( ZP_CC_RED, ZP_CC_BLACK ) );
    ZP_LOG_LEVEL_GENERATE( Critical,    "[ CRIT ]  ", ZP_CC( ZP_CC_RED, ZP_CC_GRAY ) );
    ZP_LOG_LEVEL_GENERATE( Fatal,        "[ FATAL ] ", ZP_CC( ZP_CC_WHITE, ZP_CC_RED ) );
};

class zpLogOutput;

class zpLog
{
public:
    enum Options : zp_char
    {
        endl =    '\n',
        tab =    '\t',

        bin =    'a',
        oct,
        dec,
        hex,

        reset =    'A',
        red,
        green,
        blue,
        yellow,
        magenta,
        cyan,
        light_red,
        light_green,
        light_blue,
        light_yellow,
        light_magenta,
        light_cyan,
        gray,
        dark_gray,
        white,
    };

    ~zpLog();
    
    template<typename L>
    static zpLogOutput& log()
    {
#if ZP_USE_LOGGING
        zpConsole::getInstance()->setColor( L::getLogLevelColor() );
#endif
        return getOutput() << L::getLogLevelText();
    }

    static zpLogOutput& message();
    static zpLogOutput& debug();
    static zpLogOutput& notify();
    static zpLogOutput& warning();
    static zpLogOutput& exception();
    static zpLogOutput& error();
    static zpLogOutput& critical();
    static zpLogOutput& fatal();
    
private:
    static zpLogOutput& getOutput();

    zpLog();
};

template<typename L, typename R>
class zpLogger
{};

class zpLogOutput
{
public:
    zpLogOutput();
    ~zpLogOutput();

    zpLogOutput& operator<<( zp_char c );
    zpLogOutput& operator<<( zp_int c );
    zpLogOutput& operator<<( zp_long c );
    zpLogOutput& operator<<( zp_float c );
    zpLogOutput& operator<<( const zp_char* c );
    zpLogOutput& operator<<( const zpString& c );
    zpLogOutput& operator<<( zpLog::Options c );

private:
    zpLogOutputType m_type;
    zp_uint m_base;
};

#endif
