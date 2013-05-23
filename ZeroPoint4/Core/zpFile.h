#pragma once
#ifndef ZP_FILE_H
#define ZP_FILE_H

enum zpFileMode {
	ZP_FILE_MODE_NONE =						0,

	ZP_FILE_MODE_ASCII_READ,
	ZP_FILE_MODE_ASCII_APPEND_WRITE,
	ZP_FILE_MODE_ASCII_TRUNCATE_WRITE,

	ZP_FILE_MODE_ASCII_READ_WRITE,
	ZP_FILE_MODE_ASCII_READ_APPEND_WRITE,
	ZP_FILE_MODE_ASCII_READ_TRUNCATE_WRITE,

	ZP_FILE_MODE_BINARY_READ,
	ZP_FILE_MODE_BINARY_APPEND_WRITE,
	ZP_FILE_MODE_BINARY_TRUNCATE_WRITE,

	ZP_FILE_MODE_BINARY_READ_WRITE,
	ZP_FILE_MODE_BINARY_READ_APPEND_WRITE,
	ZP_FILE_MODE_BINARY_READ_TRUNCATE_WRITE,
};

enum zpFileSeek {
	ZP_FILE_SEEK_SET =		0,
	ZP_FILE_SEEK_CURRENT =	1,
	ZP_FILE_SEEK_END =		2,
};

class zpFile {
public:
	zpFile();
	explicit zpFile( const zpString& filename);
	zpFile( zpFile&& file );
	~zpFile();

	void operator=( zpFile&& file );

	static const zp_char sep;
	static zpFile createTempFile();
	static const zpString& getCurrentDirectory();
	static void convertToFilePath( zpString& filepath );

	zpFileMode getFileMode() const;
	zp_long getFileSize();
	zp_bool isBinaryFile() const;
	zp_bool isOpen() const;

	zp_bool open( zpFileMode mode );
	void close();

	zp_int readFile( zpStringBuffer& buffer );
	zp_int readLine( zpStringBuffer& buffer );

	zp_int readFileBinary( zpArrayList<zp_byte>& buffer );
	zp_int readFileBinary( zpDataBuffer& buffer );
	
	zp_bool readByte( zp_byte& value ) const;
	zp_bool readChar( zp_char& value ) const;
	zp_bool readShort( zp_short& value ) const;
	zp_bool readInt( zp_int& value ) const;
	zp_bool readLong( zp_long& value ) const;
	zp_bool readFloat( zp_float& value ) const;
	zp_bool readDouble( zp_double& value ) const;

	template<typename T>
	zp_int readData( T& value ) const
	{
		ZP_ASSERT( isBinaryFile(), "zpFile: Can only read data from a binary file" );
		zp_int count = 0;
#if ZP_USE_SAFE_FUNCTIONS
		count = fread_s( &value, sizeof( T ), sizeof( T ), 1, (FILE*)m_file );
#else
		count = fread( &value, sizeof( T ), 1, (FILE*)m_file );
#endif
		return count;
	}

	zp_bool writeByte( zp_byte value );
	zp_bool writeChar( zp_char value );
	zp_bool writeShort( zp_short value );
	zp_bool writeInt( zp_int value );
	zp_bool writeLong( zp_long value );
	zp_bool writeFloat( zp_float value );
	zp_bool writeDouble( zp_double value );
	zp_int writeFormat( const zp_char* format, ... );
	zp_int writeBuffer( const zpStringBuffer& buffer );
	zp_int writeBuffer( const zpDataBuffer& buffer );

	template<typename T>
	zp_int writeData( const T& value )
	{
		ZP_ASSERT( isBinaryFile(), "zpFile: Can only write data to a binary file" );
		zp_int count = fwrite( &value, sizeof( T ), 1, (FILE*)m_file );
		return count;
	}

	void flush();
	void seek( zp_long pos, zpFileSeek seek = ZP_FILE_SEEK_SET );
	zp_long tell();
	void rewind();

	const zpString& getFilename() const;
	void getFileExtension( zpString& out ) const;
	void getFileDirectory( zpString& out ) const;	

private:
	zpFile( const zpFile& file );
	void operator=( const zpFile& file );

	zp_handle m_file;
	zpFileMode m_mode;
	zp_long m_size;

	zpString m_filename;
};

#endif