#include "zpCore.h"

zpLog::zpLog() {}
zpLog::~zpLog() {}

zpLogOutput& zpLog::message()
{
	return log<zpLogLevel::Message>();
}
zpLogOutput& zpLog::debug()
{
	return log<zpLogLevel::Debug>();
}
zpLogOutput& zpLog::notify()
{
	return log<zpLogLevel::Notify>();
}
zpLogOutput& zpLog::warning()
{
	return log<zpLogLevel::Warning>();
}
zpLogOutput& zpLog::exception()
{
	return log<zpLogLevel::Exception>();
}
zpLogOutput& zpLog::error()
{
	return log<zpLogLevel::Error>();
}
zpLogOutput& zpLog::critical()
{
	return log<zpLogLevel::Critical>();
}
zpLogOutput& zpLog::fatal()
{
	return log<zpLogLevel::Fatal>();
}

#if ZP_USE_LOGGING
zpLogOutput& zpLog::getOutput()
{
	static zpLogOutput output;
	return output;
}
#else
zpLogOutput& zpLog::getOutput()
{
	return *reinterpret_cast<zpLogOutput*>( 0 );
}
#endif

zpLogOutput::zpLogOutput()
	: m_base( 10 )
{}
zpLogOutput::~zpLogOutput()
{}

zpLogOutput& zpLogOutput::operator<<( zp_char c )
{
#if ZP_USE_LOGGING
	zp_printf( "%c", c );
#endif
	return (*this);
}
zpLogOutput& zpLogOutput::operator<<( zp_int c )
{
#if ZP_USE_LOGGING
	zp_printf( m_base == 10 ? "%d" : m_base == 8 ? "%o" : "%x", c );
#endif
	return (*this);
}
zpLogOutput& zpLogOutput::operator<<( zp_long c )
{
#if ZP_USE_LOGGING
	zp_printf( m_base == 10 ? "%ld" : m_base == 8 ? "%lo" : "%lx", c );
#endif
	return (*this);
}
zpLogOutput& zpLogOutput::operator<<( zp_float c )
{
#if ZP_USE_LOGGING
	zp_printf( "%f", c );
#endif
	return (*this);
}
zpLogOutput& zpLogOutput::operator<<( const zp_char* c )
{
#if ZP_USE_LOGGING
	zp_printf( "%s", c );
#endif
	return (*this);
}
zpLogOutput& zpLogOutput::operator<<( const zpString& c )
{
#if ZP_USE_LOGGING
	zp_printf( "%s", c.str() );
#endif
	return (*this);
}
zpLogOutput& zpLogOutput::operator<<( zpLog::Options c )
{
#if ZP_USE_LOGGING
	zpConsole* console = zpConsole::getInstance();

	switch( c ) {
	case zpLog::endl:
		console->resetColor();
		m_base = 10;
	case zpLog::tab:
		zp_printf( "%c", c );
		break;
	case zpLog::bin:
		m_base = 2;
		break;
	case zpLog::oct:
		m_base = 8;
		break;
	case zpLog::dec:
		m_base = 10;
		break;
	case zpLog::hex:
		m_base = 16;
		break;
	case zpLog::reset:
		console->resetColor();
		break;
	case zpLog::red:
		console->setColor( ZP_CC_RED );
		break;
	case zpLog::green:
		console->setColor( ZP_CC_GREEN );
		break;
	case zpLog::blue:
		console->setColor( ZP_CC_BLUE );
		break;
	case zpLog::yellow:
		console->setColor( ZP_CC_YELLOW );
		break;
	case zpLog::magenta:
		console->setColor( ZP_CC_MAGENTA );
		break;
	case zpLog::cyan:
		console->setColor( ZP_CC_CYAN );
		break;
	case zpLog::light_red:
		console->setColor( ZP_CC_LIGHT_RED );
		break;
	case zpLog::light_green:
		console->setColor( ZP_CC_LIGHT_GREEN );
		break;
	case zpLog::light_blue:
		console->setColor( ZP_CC_LIGHT_BLUE );
		break;
	case zpLog::light_yellow:
		console->setColor( ZP_CC_LIGHT_YELLOW );
		break;
	case zpLog::light_magenta:
		console->setColor( ZP_CC_LIGHT_MAGENTA );
		break;
	case zpLog::light_cyan:
		console->setColor( ZP_CC_LIGHT_CYAN );
		break;
	case zpLog::gray:
		console->setColor( ZP_CC_GRAY);
		break;
	case zpLog::dark_gray:
		console->setColor( ZP_CC_DARK_GRAY );
		break;
	case zpLog::white:
		console->setColor( ZP_CC_WHITE );
		break;
	}
#endif
	return (*this);
}
