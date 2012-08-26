#include "zpCore.h"
#include <stdio.h>
#include <stdarg.h>

#if ZP_WIN_32 || ZP_WIN_64
#include <Windows.h>
#endif

#define ZP_FILE_BUFFER_SIZE		64

const zp_char* __zpFileModeToString( zpFileMode mode ) {
	switch( mode ) {
		case ZP_FILE_MODE_READ: return "r";
		case ZP_FILE_MODE_TRUNCATE_WRITE: return "w";
		case ZP_FILE_MODE_APPEND_WRITE: return "a";
		case ZP_FILE_MODE_READ_WRITE: return "r+";
		case ZP_FILE_MODE_READ_TRUNCATE_WRITE: return "w+";
		case ZP_FILE_MODE_READ_APPEND_WRITE: return "a+";

		case ZP_FILE_MODE_BINARY_READ: return "rb";
		case ZP_FILE_MODE_BINARY_TRUNCATE_WRITE: return "wb";
		case ZP_FILE_MODE_BINARY_APPEND_WRITE: return "ab";
		case ZP_FILE_MODE_BINARY_READ_WRITE: return "rb+";
		case ZP_FILE_MODE_BINARY_READ_TRUNCATE_WRITE: return "wb+";
		case ZP_FILE_MODE_BINARY_READ_APPEND_WRITE: return "ab+";

		default: return "";
	}
}

#define ZP_FILE_MODE_BINARY_FLAG	0x1000

zpFile::zpFile() : 
	m_file( ZP_NULL ), 
	m_mode( ZP_FILE_MODE_NONE ), 
	m_size( -1 ), 
	m_filename()
{}
zpFile::zpFile( const zpString& filename, zpFileMode mode ) : 
	m_file( ZP_NULL ), 
	m_mode( mode ), 
	m_size( -1 ), 
	m_filename( filename )
{
	convertToFilePath( m_filename );
}
zpFile::zpFile( const zpFile& file ) : 
	m_file( file.m_file ), 
	m_mode( file.m_mode ), 
	m_size( file.m_size ), 
	m_filename( file.m_filename )
{
	convertToFilePath( m_filename );
}
zpFile::zpFile( zpFile&& file ) : 
	m_file( file.m_file ), 
	m_mode( file.m_mode ), 
	m_size( file.m_size ), 
	m_filename( (zpString&&)file.m_filename )
{
	convertToFilePath( m_filename );
	file.m_file = ZP_NULL;
}
zpFile::~zpFile() {
	close();
}

void zpFile::operator=( const zpFile& file ) {}
void zpFile::operator=( zpFile&& file ) {
	if( this == &file ) return;

	close();
	m_file = file.m_file;
	m_mode = file.m_mode;
	m_size = file.m_size;
	m_filename = (zpString&&)file.m_filename;

	file.m_file = ZP_NULL;
}

const zp_char zpFile::sep = '/';
zpFile zpFile::createTempFile() {
	zpFile tmp;

	return tmp;
}

zpString zpFile::s_currentDirectory;
const zpString& zpFile::getCurrentDirectory() {
	if( s_currentDirectory.isEmpty() ) {
#if ZP_WIN_32 || ZP_WIN_64
		zp_dword size = GetCurrentDirectory( 0, ZP_NULL );
		zp_char* buff = new zp_char[ size + 1 ];
		GetCurrentDirectory( size, buff );
		buff[ size - 1 ] = zpFile::sep;
		buff[ size ] = 0;
		s_currentDirectory = buff;
		delete[] buff;
#endif
	}
	return s_currentDirectory;
}
void zpFile::convertToFilePath( zpString& filepath ) {
	filepath.map( []( zp_char ch ) -> zp_char {
		return ch == '/' || ch == '\\' ? zpFile::sep : ch;
	} );
}


zpFileMode zpFile::getFileMode() const {
	return m_mode;
}
zp_long zpFile::getFileSize() {
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
zp_bool zpFile::isBinaryFile() const {
	return ( m_mode & ZP_FILE_MODE_BINARY_FLAG ) == ZP_FILE_MODE_BINARY_FLAG;
}
zp_bool zpFile::isOpen() const {
	return m_file != ZP_NULL;
}

zp_bool zpFile::open( zpFileMode mode ) {
	if( m_file ) return false;
	if( mode != ZP_FILE_MODE_NONE ) m_mode = mode;
	if( m_mode == ZP_FILE_MODE_NONE ) return false;
	if( m_filename.isEmpty() ) return false;

	zp_uint err = fopen_s( (FILE**)&m_file, m_filename.c_str(), __zpFileModeToString( m_mode ) );
	return 0 == err;
}
void zpFile::close() {
	if( m_file ) {
		fclose( (FILE*)m_file );
		m_file = ZP_NULL;
		m_size = -1;
	}
}

zp_int zpFile::readFile( zpStringBuffer* buffer ) {
	zp_int count = 0;
	if( m_file && buffer && !isBinaryFile() ) {
		zp_char buff[ ZP_FILE_BUFFER_SIZE ];
		FILE* f = (FILE*)m_file;

		zp_uint s;
		while( feof( f ) == 0 && ( s = fread_s( buff, sizeof( buff ), sizeof( zp_char ), sizeof( buff ), f ) ) >= 0 ) {
			buffer->append( buff, s );
			count += s;
		}
	}
	return count;
}
zp_int zpFile::readLine( zpStringBuffer* buffer ) {
	zp_int count = 0;
	if( m_file && buffer && !isBinaryFile() ) {
		zp_char buff[ ZP_FILE_BUFFER_SIZE ];
		FILE* f = (FILE*)m_file;
		
		zp_bool newLineFound = false;
		zp_uint len = 0;
		while( !newLineFound && fgets( buff, sizeof( buff ), f ) != ZP_NULL ) {
			buffer->append( buff );
			len = strlen( buff );
			newLineFound = buff[ len ] == '\0';
			count += len;
		}
	}
	return count;
}

zp_int zpFile::readFileBinary( zpArrayList<zp_byte>* buffer ) {
	zp_int count = 0;
	if( m_file && buffer && isBinaryFile() ) {
		zp_char buff[ ZP_FILE_BUFFER_SIZE ];
		FILE* f = (FILE*)m_file;

		zp_uint s;
		while( ( s = fread_s( buff, sizeof( buff ), sizeof( zp_char ), sizeof( buff ), f ) ) >= 0 && feof( f ) == 0 ) {
			buffer->pushBack( (zp_byte*)buff, s );
			count += s;
		}
	}
	return count;
}
	
zp_bool zpFile::readByte( zp_byte* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_byte c;
		fread( &c, sizeof( zp_byte ), 1, (FILE*)m_file );
		*value = c;
	} else {
		*value = fgetc( (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::readChar( zp_char* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_char c;
		fread( &c, sizeof( zp_char ), 1, (FILE*)m_file );
		*value = c;
	} else {
		*value = fgetc( (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::readShort( zp_short* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_short c;
		fread( &c, sizeof( zp_short ), 1, (FILE*)m_file );
		*value = c;
	} else {
		fscanf_s( (FILE*)m_file, "%dh", value );
	}
	return true;
}
zp_bool zpFile::readInt( zp_int* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_int c;
		fread( &c, sizeof( zp_int ), 1, (FILE*)m_file );
		*value = c;
	} else {
		fscanf_s( (FILE*)m_file, "%d", value );
	}
	return true;
}
zp_bool zpFile::readLong( zp_long* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_long c;
		fread( &c, sizeof( zp_long ), 1, (FILE*)m_file );
		*value = c;
	} else {
		fscanf_s( (FILE*)m_file, "%dl", value );
	}
	return true;
}
zp_bool zpFile::readFloat( zp_float* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_float c;
		fread( &c, sizeof( zp_float ), 1, (FILE*)m_file );
		*value = c;
	} else {
		fscanf_s( (FILE*)m_file, "%f", value );
	}
	return true;
}
zp_bool zpFile::readDouble( zp_double* value ) const {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		zp_double c;
		fread( &c, sizeof( zp_double ), 1, (FILE*)m_file );
		*value = c;
	} else {
		fscanf_s( (FILE*)m_file, "%fL", value );
	}
	return true;
}
zp_int zpFile::readFormat( const zp_char* format, ... ) {
	zp_int count = 0;
	
	if( m_file ) {
		va_list args;
		va_start( args, format );
		//count = vfscanf( (FILE*)m_file, format, args );
		va_end( args );
	}

	return count;
}

zp_bool zpFile::writeByte( zp_byte value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_byte ), 1, (FILE*)m_file );
	} else {
		fputc( value, (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::writeChar( zp_char value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_char ), 1, (FILE*)m_file );
	} else {
		fputc( value, (FILE*)m_file );
	}
	return true;
}
zp_bool zpFile::writeShort( zp_short value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_short ), 1, (FILE*)m_file );
	} else {
		fprintf_s( (FILE*)m_file, "%dh", value );
	}
	return true;
}
zp_bool zpFile::writeInt( zp_int value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_int ), 1, (FILE*)m_file );
	} else {
		fprintf_s( (FILE*)m_file, "%d", value );
	}
	return true;
}
zp_bool zpFile::writeLong( zp_long value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_long ), 1, (FILE*)m_file );
	} else {
		fprintf_s( (FILE*)m_file, "%dl", value );
	}
	return true;
}
zp_bool zpFile::writeFloat( zp_float value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_float ), 1, (FILE*)m_file );
	} else {
		fprintf_s( (FILE*)m_file, "%f", value );
	}
	return true;
}
zp_bool zpFile::writeDouble( zp_double value ) {
	if( !m_file ) return false;
	if( isBinaryFile() ) {
		fwrite( &value, sizeof( zp_double ), 1, (FILE*)m_file );
	} else {
		fprintf_s( (FILE*)m_file, "%fL", value );
	}
	return true;
}
zp_int zpFile::writeFormat( const zp_char* format, ... ) {
	zp_int count = 0;

	if( m_file ) {
		va_list args;
		va_start( args, format );
		count = vfprintf_s( (FILE*)m_file, format, args );
		va_end( args );
	}
	
	return count;
}

void zpFile::flush() {
	if( m_file ) {
		fflush( (FILE*)m_file );
	}
}
void zpFile::seek( zp_long pos, zpFileSeek seek ) {
	if( m_file ) {
		fseek( (FILE*)m_file, (zp_int)pos, (zp_int)seek );
	}
}
zp_long zpFile::tell() {
	if( !m_file ) return -1;
	return ftell( (FILE*)m_file );
}
void zpFile::rewind() {
	if( m_file ) {
		::rewind( (FILE*)m_file );
	}
}

const zpString& zpFile::getFilename() const {
	return m_filename;
}
zpString zpFile::getFileExtension() const {
	if( m_filename.isEmpty() ) return zpString();

	zp_uint pos = m_filename.lastIndexOf( '.' );
	if( pos == zpString::npos ) return zpString();

	return m_filename.substring( pos + 1 );
}
zpString zpFile::getFileDirectory() const {
	if( m_filename.isEmpty() ) return zpString();

	zp_uint pos = m_filename.lastIndexOf( zpFile::sep );
	if( pos == zpString::npos ) return zpString();

	return m_filename.substring( 0, pos );
}