#include "zpCore.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

#if ZP_WIN_32 || ZP_WIN_64
#include <Windows.h>
#endif

#define ZP_FILE_BUFFER_SIZE		256

const zp_char* g_zpFileModeToString[] =
{
	"",
	"r",
	"a",
	"w",

	"r+",
	"a+",
	"w+",

	"rb",
	"ab",
	"wb",

	"rb+",
	"ab+",
	"wb+"
};

zpFile::zpFile()
	: m_file( ZP_NULL )
	, m_mode( ZP_FILE_MODE_NONE )
	, m_size( -1 )
	, m_filename()
{}
zpFile::zpFile( const zp_char* filename )
	: m_file( ZP_NULL )
	, m_mode( ZP_FILE_MODE_NONE )
	, m_size( -1 )
	, m_filename( filename )
{
	convertToFilePath( m_filename );
}
zpFile::zpFile( const zpString& filename )
	: m_file( ZP_NULL )
	, m_mode( ZP_FILE_MODE_NONE )
	, m_size( -1 )
	, m_filename( filename )
{
	convertToFilePath( m_filename );
}
zpFile::zpFile( zpFile&& file )
	: m_file( file.m_file )
	, m_mode( file.m_mode )
	, m_size( file.m_size )
	, m_filename( zp_move( file.m_filename ) )
{
	convertToFilePath( m_filename );
	file.m_file = ZP_NULL;
}
zpFile::~zpFile()
{
	close();
}


void zpFile::operator=( zpFile&& file )
{
	if( this == &file ) return;

	close();
	m_file = file.m_file;
	m_mode = file.m_mode;
	m_size = file.m_size;
	m_filename = zp_move( file.m_filename );

	file.m_file = ZP_NULL;
}

const zp_char zpFile::sep = '/';
zpFile zpFile::createTempFile()
{
	zpFile tmp;

	return tmp;
}

const zpString& zpFile::getCurrentDirectory()
{
	static zpString currentDirectory;
	if( currentDirectory.isEmpty() ) {
#if ZP_WIN_32 || ZP_WIN_64
		zp_dword size = GetCurrentDirectory( 0, ZP_NULL );
		zp_char* buff = new zp_char[ size + 1 ];
		GetCurrentDirectory( size, buff );
		buff[ size - 1 ] = zpFile::sep;
		buff[ size ] = '\0';
		currentDirectory = buff;
		convertToFilePath( currentDirectory );
		delete[] buff;
#endif
	}
	return currentDirectory;
}
void zpFile::convertToFilePath( zpString& filepath )
{
	filepath.map( []( zp_char ch ) -> zp_char {
		return ch == '/' || ch == '\\' ? zpFile::sep : ch;
	} );
}
zp_long zpFile::getFileModificationTime( const zp_char* filepath )
{
	zp_long time = 0;
#if ZP_WIN_32 || ZP_WIN_64
	struct stat fileStat;
	zp_int r = stat( filepath, &fileStat );
	time = (zp_long)fileStat.st_mtime;
#endif
	return time;
}


zpFileMode zpFile::getFileMode() const
{
	return m_mode;
}
zp_long zpFile::getFileSize()
{
	if( m_size < 0 && m_file ) {
		fpos_t pos;
		FILE* f = (FILE*)m_file;
		fgetpos( f, &pos );
		fseek( f, 0, SEEK_SET );
		fseek( f, 0, SEEK_END );
		m_size = ftell( f );
		fsetpos( f, &pos );
	}
	return m_size;
}
zp_bool zpFile::isBinaryFile() const
{
	return m_mode >= ZP_FILE_MODE_BINARY_READ;
}
zp_bool zpFile::isOpen() const
{
	return m_file != ZP_NULL;
}

zp_bool zpFile::open( zpFileMode mode )
{
	ZP_ASSERT( mode != ZP_FILE_MODE_NONE, "zpFile: Open mode needed" );
	ZP_ASSERT( m_file == ZP_NULL, "zpFile: File already open" );
	ZP_ASSERT( !m_filename.isEmpty(), "zpFile: Filename not set" );
	zp_uint err = 0;
	const zp_char* fopenMode = g_zpFileModeToString[ mode ];

	m_mode = mode;

#if ZP_USE_SAFE_FUNCTIONS
	err = fopen_s( (FILE**)&m_file, m_filename.str(), fopenMode );
#else
	m_file = (zp_handle)fopen( m_filename.str(), fopenMode );
#endif
	return err == 0;
}
void zpFile::close()
{
	if( m_file ) {
		fclose( (FILE*)m_file );
		m_file = ZP_NULL;
		m_size = -1;
		m_mode = ZP_FILE_MODE_NONE;
	}
}

zp_int zpFile::readFile( zpStringBuffer& buffer )
{
	ZP_ASSERT( m_file != ZP_NULL, "zpFile: File not open" );
	ZP_ASSERT( !isBinaryFile(), "zpFile: Trying to read binary file as ascii" );

	buffer.clear();
	buffer.reserve( (zp_uint)getFileSize() );

	zp_int count = 0;
	zp_char buff[ ZP_FILE_BUFFER_SIZE ];
	FILE* f = (FILE*)m_file;
	zp_uint s;
	while(
		feof( f ) == 0 && 
#if ZP_USE_SAFE_FUNCTIONS
		( s = fread_s( buff, sizeof( buff ), sizeof( zp_char ), sizeof( buff ), f ) ) >= 0
#else
		( s = fread( buff, sizeof( zp_char ), sizeof( buff ), f ) ) >= 0
#endif
		)
	{
		buffer.append( buff, s );
		count += s;
	}

	return count;
}
zp_int zpFile::readLine( zpStringBuffer& buffer )
{
	ZP_ASSERT( m_file != ZP_NULL, "zpFile: File not open" );
	ZP_ASSERT( !isBinaryFile(), "zpFile: Trying to read binary file as ascii" );

	buffer.clear();

	zp_int count = 0;
	zp_char buff[ ZP_FILE_BUFFER_SIZE ];
	FILE* f = (FILE*)m_file;

	zp_bool newLineFound = false;
	zp_uint len = 0;
	while( !newLineFound && fgets( buff, sizeof( buff ), f ) != ZP_NULL )
	{
		len = zp_strlen( buff );
		newLineFound = buff[ len - 1 ] == '\n';
		buffer.append( buff, newLineFound ? len - 1 : len );
		count += newLineFound ? len - 1 : len;
	}

	return count;
}
zp_bool zpFile::isEOF() const
{
	ZP_ASSERT( m_file != ZP_NULL, "zpFile: File not open" );
	FILE* f = (FILE*)m_file;

	return feof( f ) != 0;
}


zp_int zpFile::readFileBinary( zpArrayList<zp_byte>& buffer )
{
	ZP_ASSERT( m_file != ZP_NULL, "zpFile: File not open" );
	ZP_ASSERT( isBinaryFile(), "zpFile: Trying to read ascii file as binary" );
	
	zp_int count = 0;
	zp_char buff[ ZP_FILE_BUFFER_SIZE ];
	FILE* f = (FILE*)m_file;

	buffer.reset();
	buffer.reserve( (zp_uint)getFileSize() );

	zp_uint s;
	while(
		feof( f ) == 0 && 
#if ZP_USE_SAFE_FUNCTIONS
		( s = fread_s( buff, sizeof( buff ), sizeof( zp_char ), sizeof( buff ), f ) ) >= 0
#else
		( s = fread( buff, sizeof( zp_char ), sizeof( buff ), f ) ) >= 0
#endif
		)
	{
		for( zp_uint i = 0; i < s; ++i ) buffer.pushBack( buff[ s ] );
		count += s;
	}

	return count;
}

zp_int zpFile::readFileBinary( zpDataBuffer& buffer )
{
	ZP_ASSERT( m_file != ZP_NULL, "zpFile: File not open" );
	ZP_ASSERT( isBinaryFile(), "zpFile: Trying to read ascii file as binary" );

	zp_int count = 0;
	zp_byte buff[ ZP_FILE_BUFFER_SIZE ];
	FILE* f = (FILE*)m_file;

	buffer.reset();
	buffer.reserve( (zp_uint)getFileSize() );

	zp_uint s;
	while(
		feof( f ) == 0 && 
#if ZP_USE_SAFE_FUNCTIONS
		( s = fread_s( buff, sizeof( buff ), sizeof( zp_char ), sizeof( buff ), f ) ) >= 0
#else
		( s = fread( buff, sizeof( zp_char ), sizeof( buff ), f ) ) >= 0
#endif
		)
	{
		buffer.writeBulk( buff, s );
		count += s;
	}

	return count;
}

zp_bool zpFile::readByte( zp_byte& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_byte ), sizeof( zp_byte ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_byte ), 1, (FILE*)m_file );
#endif
	}
	else
	{
		value = (zp_byte)fgetc( (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::readChar( zp_char& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_char ), sizeof( zp_char ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_char ), 1, (FILE*)m_file );
#endif
	}
	else
	{
		value = (zp_char)fgetc( (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::readShort( zp_short& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_short ), sizeof( zp_short ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_short ), 1, (FILE*)m_file );
#endif
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fscanf_s( (FILE*)m_file, "%dh", value );
#else
		fscanf( (FILE*)m_file, "%dh", value );
#endif
	}
	return true;
}
zp_bool zpFile::readInt( zp_int& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_int ), sizeof( zp_int ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_int ), 1, (FILE*)m_file );
#endif
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fscanf_s( (FILE*)m_file, "%d", value );
#else
		fscanf( (FILE*)m_file, "%d", value );
#endif
	}
	return true;
}
zp_bool zpFile::readLong( zp_long& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_long ), sizeof( zp_long ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_long ), 1, (FILE*)m_file );
#endif
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fscanf_s( (FILE*)m_file, "%dl", value );
#else
		fscanf( (FILE*)m_file, "%dl", value );
#endif
	}
	return true;
}
zp_bool zpFile::readFloat( zp_float& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_float ), sizeof( zp_float ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_float ), 1, (FILE*)m_file );
#endif
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fscanf_s( (FILE*)m_file, "%f", value );
#else
		fscanf( (FILE*)m_file, "%f", value );
#endif
	}
	return true;
}
zp_bool zpFile::readDouble( zp_double& value ) const
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
#if ZP_USE_SAFE_FUNCTIONS
		fread_s( &value, sizeof( zp_double ), sizeof( zp_double ), 1, (FILE*)m_file );
#else
		fread( &value, sizeof( zp_double ), 1, (FILE*)m_file );
#endif
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fscanf_s( (FILE*)m_file, "%fl", value );
#else
		fscanf( (FILE*)m_file, "%fl", value );
#endif
	}
	return true;
}

zp_bool zpFile::writeByte( zp_byte value )
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_byte ), 1, (FILE*)m_file );
	}
	else
	{
		fputc( value, (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::writeChar( zp_char value )
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_char ), 1, (FILE*)m_file );
	}
	else
	{
		fputc( value, (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::writeShort( zp_short value )
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_short ), 1, (FILE*)m_file );
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fprintf_s( (FILE*)m_file, "%dh", value );
#else
		fprintf( (FILE*)m_file, "%dh", value );
#endif
	}
	return true;
}
zp_bool zpFile::writeInt( zp_int value )
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_int ), 1, (FILE*)m_file );
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fprintf_s( (FILE*)m_file, "%d", value );
#else
		fprintf( (FILE*)m_file, "%d", value );
#endif
	}
	return true;
}
zp_bool zpFile::writeLong( zp_long value ) {
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_long ), 1, (FILE*)m_file );
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fprintf_s( (FILE*)m_file, "%dl", value );
#else
		fprintf( (FILE*)m_file, "%dl", value );
#endif
	}
	return true;
}
zp_bool zpFile::writeFloat( zp_float value )
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_float ), 1, (FILE*)m_file );
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fprintf_s( (FILE*)m_file, "%f", value );
#else
		fprintf( (FILE*)m_file, "%f", value );
#endif
	}
	return true;
}
zp_bool zpFile::writeDouble( zp_double value )
{
	if( !m_file ) return false;
	if( isBinaryFile() )
	{
		fwrite( &value, sizeof( zp_double ), 1, (FILE*)m_file );
	}
	else
	{
#if ZP_USE_SAFE_FUNCTIONS
		fprintf_s( (FILE*)m_file, "%fl", value );
#else
		fprintf( (FILE*)m_file, "%fl", value );
#endif
	}
	return true;
}
zp_int zpFile::writeFormat( const zp_char* format, ... )
{
	zp_int count = 0;

	if( m_file ) {
		va_list args;
		va_start( args, format );
#if ZP_USE_SAFE_FUNCTIONS
		count = vfprintf_s( (FILE*)m_file, format, args );
#else
		count = vfprintf( (FILE*)m_file, format, args );
#endif
		va_end( args );
	}
	
	return count;
}

zp_int zpFile::writeBuffer( const zpStringBuffer& buffer )
{
	zp_int count = 0;

	if( m_file )
	{
		count = fputs( buffer.str(), (FILE*)m_file );
	}

	return count;
}

zp_int zpFile::writeBuffer( const zpDataBuffer& buffer )
{
	zp_int count = 0;

	if( m_file )
	{
		count = fwrite( buffer.getData(), sizeof( zp_byte ), buffer.size(), (FILE*)m_file );
	}

	return count;
}

void zpFile::flush()
{
	if( m_file )
	{
		fflush( (FILE*)m_file );
	}
}
void zpFile::seek( zp_long pos, zpFileSeek seek )
{
	if( m_file )
	{
		fseek( (FILE*)m_file, (zp_int)pos, (zp_int)seek );
	}
}
zp_long zpFile::tell()
{
	if( !m_file ) return -1;
	return ftell( (FILE*)m_file );
}
void zpFile::rewind()
{
	if( m_file )
	{
		::rewind( (FILE*)m_file );
	}
}

const zpString& zpFile::getFilename() const
{
	return m_filename;
}
void zpFile::getFileExtension( zpString& out ) const
{
	if( !m_filename.isEmpty() )
	{
		zp_int pos = m_filename.lastIndexOf( '.' );
		if( pos != zpString::npos )
		{
			m_filename.substring( out, pos + 1 );
		}
	}
}
void zpFile::getFileDirectory( zpString& out ) const
{
	if( !m_filename.isEmpty() )
	{
		zp_int pos = m_filename.lastIndexOf( zpFile::sep );
		if( pos != zpString::npos )
		{
			m_filename.substring( out, 0, pos );
		}
	}
}