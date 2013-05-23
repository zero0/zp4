#include "zpCore.h"

#define ZP_BISON_HEADER_ID	ZP_MAKE_UINT( 'B', 'I', 'S', 'N' )

const zpBison::Value zpBison::null;
zpBison::zpBison()
	: m_buffer()
{}
zpBison::~zpBison()
{
	m_buffer.clear();
}

zp_bool zpBison::readFromFile( const zpString& filename )
{
	zpFile file( filename );
	if( file.open( ZP_FILE_MODE_BINARY_READ ) )
	{
		m_buffer.reset();
		file.readFileBinary( m_buffer );
		file.close();

		parseData();

		return true;
	}

	return false;
}
zp_bool zpBison::readFromBuffer( const zpDataBuffer& buffer )
{
	m_buffer.reset();
	buffer.write( m_buffer );

	parseData();

	return true;
}

const zpBison::Value& zpBison::root() const
{
	return m_root;
}

void zpBison::parseData()
{
	m_root = null;

	if( m_buffer.size() > 0 )
	{
		const zp_byte* data = m_buffer.getData();
		zpBisonHeader header;
		header = *(const zpBisonHeader*)data;
		ZP_ASSERT( header.bison == ZP_BISON_HEADER_ID, "Invalid Bison header" );
		if( header.bison == ZP_BISON_HEADER_ID )
		{
			m_root = zpBison::Value( data, sizeof( zpBisonHeader ) );
		}
	}
}


zpBison::Value::Value( const zp_byte* data, zp_uint offset )
	: m_type( (zpBisonType)*(zp_uint*)( data + offset ) )
	, m_data( data )
	, m_offset( offset )
{}

zpBison::Value::Value()
	: m_type( ZP_BISON_TYPE_NULL )
	, m_data( ZP_NULL )
	, m_offset( 0 )
{}
zpBison::Value::~Value()
{}

zpBisonType zpBison::Value::type() const
{
	return m_type;
}
zp_uint zpBison::Value::size() const
{
	switch( m_type )
	{
	case ZP_BISON_TYPE_STRING:
	case ZP_BISON_TYPE_ARRAY:
	case ZP_BISON_TYPE_OBJECT:
		return *(zp_uint*)( m_data + m_offset + sizeof( zpBisonType ) );
	default:
		return 0;
	}
}
zp_bool zpBison::Value::isEmpty() const
{
	switch( m_type )
	{
	case ZP_BISON_TYPE_NULL:
		return true;
	case ZP_BISON_TYPE_STRING:
	case ZP_BISON_TYPE_ARRAY:
	case ZP_BISON_TYPE_OBJECT:
		return *(zp_uint*)( m_data + m_offset + sizeof( zpBisonType ) ) == 0;
	default:
		return false;
	}
}

zp_bool zpBison::Value::isNull() const
{
	return m_type == ZP_BISON_TYPE_NULL;
}
zp_bool zpBison::Value::isBool() const
{
	return m_type == ZP_BISON_TYPE_BOOL;
}
zp_bool zpBison::Value::isInt() const
{
	return m_type == ZP_BISON_TYPE_INT;
}
zp_bool zpBison::Value::isFloat() const
{
	return m_type == ZP_BISON_TYPE_FLOAT;
}
zp_bool zpBison::Value::isString() const
{
	return m_type == ZP_BISON_TYPE_STRING;
}
zp_bool zpBison::Value::isArray() const
{
	return m_type == ZP_BISON_TYPE_ARRAY;
}
zp_bool zpBison::Value::isObject() const
{
	return m_type == ZP_BISON_TYPE_OBJECT;
}

zp_bool zpBison::Value::isIntegral() const
{
	switch( m_type )
	{
	case ZP_BISON_TYPE_BOOL:
	case ZP_BISON_TYPE_INT:
		return true;
	default:
		return false;
	}
}
zp_bool zpBison::Value::isReal() const
{
	return m_type == ZP_BISON_TYPE_FLOAT;
}
zp_bool zpBison::Value::isNumeric() const
{
	return isIntegral() || isReal();
}

zp_bool zpBison::Value::asBool() const
{
	const zp_byte* val = m_data + m_offset + sizeof( zpBisonType );
	switch( m_type )
	{
	case ZP_BISON_TYPE_NULL:
		return false;
	case ZP_BISON_TYPE_BOOL:
	case ZP_BISON_TYPE_INT:
		return *(zp_int*)val != 0;
	case ZP_BISON_TYPE_FLOAT:
		return *(zp_float*)val != 0.0f;
	case ZP_BISON_TYPE_STRING:
	case ZP_BISON_TYPE_ARRAY:
	case ZP_BISON_TYPE_OBJECT:
		return *(zp_uint*)val != 0;
	default:
		return false;
	}
}
zp_int zpBison::Value::asInt() const
{
	const zp_byte* val = m_data + m_offset + sizeof( zpBisonType );
	switch( m_type )
	{
	case ZP_BISON_TYPE_NULL:
		return 0;
	case ZP_BISON_TYPE_BOOL:
		return *(zp_int*)val != 0 ? 1 : 0;
	case ZP_BISON_TYPE_INT:
		return *(zp_int*)val;
	case ZP_BISON_TYPE_FLOAT:
		return (zp_int)*(zp_float*)val;
	default:
		ZP_ASSERT( false, "" );
		return 0;
	}
}
zp_float zpBison::Value::asFloat() const
{
	const zp_byte* val = m_data + m_offset + sizeof( zpBisonType );
	switch( m_type )
	{
	case ZP_BISON_TYPE_NULL:
		return 0.0f;
	case ZP_BISON_TYPE_BOOL:
		return *(zp_int*)val != 0 ? 1.0f : 0.0f;
	case ZP_BISON_TYPE_INT:
		return (zp_float)*(zp_int*)val;
	case ZP_BISON_TYPE_FLOAT:
		return *(zp_float*)val;
	default:
		ZP_ASSERT( false, "" );
		return 0;
	}
}
zpString zpBison::Value::asString() const
{
	return zpString( asCString() );
}
const zp_char* zpBison::Value::asCString() const
{
	const zp_byte* val = m_data + m_offset + sizeof( zpBisonType );
	switch( m_type )
	{
	case ZP_BISON_TYPE_NULL:
		return "";
	case ZP_BISON_TYPE_BOOL:
		return *(zp_int*)val != 0 ? "true" : "false";
	case ZP_BISON_TYPE_STRING:
		return (const zp_char*)( val + sizeof( zp_uint ) );
	default:
		ZP_ASSERT( false, "" );
		return "";
	}
}

const zpBison::Value zpBison::Value::operator[]( zp_uint index ) const
{
	ZP_ASSERT( isArray(), "" );
	if( isArray() )
	{
		const zp_uint* arr = (const zp_uint*)( m_data + m_offset + sizeof( zpBisonType ) );
		zp_uint count = *arr;
		
		ZP_ASSERT( index < count, "" );
		if( index < count )
		{
			++arr;
			return zpBison::Value( m_data, arr[ index ] );
		}
	}
	
	return null;
}
const zpBison::Value zpBison::Value::operator[]( const zp_char* key ) const
{
	ZP_ASSERT( isObject(), "" );
	if( isObject() )
	{
		const zp_uint* arr = (const zp_uint*)( m_data + m_offset + sizeof( zpBisonType ) );
		zp_uint count = *arr * 2;
		++arr;

		for( zp_uint i = 0; i < count; i += 2 )
		{
			zpBison::Value k( m_data, arr[ i ] );
			if( zp_strcmp( k.asCString(), key ) == 0 )
			{
				return zpBison::Value( m_data, arr[ i + 1 ] );
			}
		}
	}

	return null;
}
const zpBison::Value zpBison::Value::operator[]( const zpString& key ) const
{
	return (*this)[ key.getChars() ];
}

void zpBison::Value::memberNames( zpArrayList< zpBison::Value >& names ) const
{
	ZP_ASSERT( isObject(), "" );
	if( isObject() )
	{
		const zp_uint* arr = (const zp_uint*)( m_data + m_offset + sizeof( zpBisonType ) );
		zp_uint size = *arr;
		names.reserve( size );

		zp_uint count = size * 2;
		++arr;

		for( zp_uint i = 0; i < count; i += 2 )
		{
			names.pushBack( zpBison::Value( m_data, arr[ i ] ) );
		}
	}
}


zp_bool zpBison::compileToFile( const zpString& filename, const zpJson& json )
{
	zpDataBuffer bison;
	compileToBuffer( bison, json );

	zpFile file( filename );
	if( file.open( ZP_FILE_MODE_BINARY_TRUNCATE_WRITE ) )
	{
		file.writeBuffer( bison );
		file.close();

		return true;
	}

	return false;
}
zp_bool zpBison::compileToBuffer( zpDataBuffer& buffer, const zpJson& json )
{
	zpBisonHeader header;
	header.bison = ZP_BISON_HEADER_ID;
	header.fileVersion = 0;
	header.stringHashSalt = 0;

	buffer.write( header );

	compileToBufferInternal( buffer, json );

	return true;
}
zp_bool zpBison::compileToBufferInternal( zpDataBuffer& buffer, const zpJson& json )
{
	switch( json.type() )
	{
	case ZP_JSON_TYPE_NULL:
		buffer.write< zp_uint >( ZP_BISON_TYPE_NULL );
		break;
	case ZP_JSON_TYPE_BOOL:
		buffer.write< zp_uint >( ZP_BISON_TYPE_BOOL );
		buffer.write< zp_int >( json.asBool() ? 1 : 0 );
		break;
	case ZP_JSON_TYPE_INT:
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		buffer.write< zp_uint >( ZP_BISON_TYPE_INT );
		buffer.write< zp_int >( json.asInt() );
		break;
	case ZP_JSON_TYPE_FLOAT:
	case ZP_JSON_TYPE_DOUBLE:
		buffer.write< zp_uint >( ZP_BISON_TYPE_FLOAT );
		buffer.write< zp_float >( json.asFloat() );
		break;
	case ZP_JSON_TYPE_STRING:
		{
			buffer.write< zp_uint >( ZP_BISON_TYPE_STRING );
			const zp_char* str = json.asCString();
			zp_uint size = zp_strlen( str );
			buffer.write< zp_uint >( size );
			buffer.writeBulk( str, size );
			buffer.write< zp_char >( '\0' );
		}
		break;
	case ZP_JSON_TYPE_ARRAY:
		{
			buffer.write< zp_uint >( ZP_BISON_TYPE_ARRAY );
			zp_uint size = json.size();
			buffer.write< zp_uint >( size );
			zp_uint arrayStart = buffer.size();
			
			buffer.writeFill< zp_uint >( 0, size );

			for( zp_uint i = 0; i < size; ++i, arrayStart += sizeof( zp_uint ) )
			{
				buffer.writeAt( buffer.size(), arrayStart );
				compileToBufferInternal( buffer, json[ i ] );
			}
		}
		break;
	case ZP_JSON_TYPE_OBJECT:
		{
			buffer.write< zp_uint >( ZP_BISON_TYPE_OBJECT );
			zp_uint size = json.size();
			buffer.write< zp_uint >( size );
			zp_uint objectStart = buffer.size();

			buffer.writeFill< zp_uint >( 0, size * 2 );

			json.foreachObject( [ &buffer, &objectStart ]( const zpString& key, const zpJson& value )
			{
				buffer.writeAt< zp_uint >( buffer.size(), objectStart );
				buffer.write< zp_uint >( ZP_BISON_TYPE_STRING );
				buffer.write< zp_uint >( key.length() );
				buffer.writeBulk( key.getChars(), key.length() );
				buffer.write< zp_char >( '\0' );
				objectStart += sizeof( zp_uint );

				buffer.writeAt< zp_uint >( buffer.size(), objectStart );
				compileToBufferInternal( buffer, value );
				objectStart += sizeof( zp_uint );				
			} );
		}
		
		
		break;
	}

	return true;
}


zpBisonWriter::zpBisonWriter()
{}
zpBisonWriter::~zpBisonWriter()
{}

const zp_char* zpBisonWriter::fastWrite( const zpBison& bison )
{
	m_string.clear();
	writeBison( m_string, bison.root(), -1 );
	return m_string.getChars();
}
const zp_char* zpBisonWriter::styleWrite( const zpBison& bison )
{
	m_string.clear();
	writeBison( m_string, bison.root(), 0 );
	return m_string.getChars();
}

void zpBisonWriter::writeBison( zpStringBuffer& buffer, const zpBison::Value& bison, zp_int indent )
{
	switch( bison.type() )
	{
	case ZP_BISON_TYPE_NULL:
		buffer.append( "null" );
		break;
	case ZP_BISON_TYPE_BOOL:
		buffer.append( bison.asBool() ? "true" : "false" );
		break;
	case ZP_BISON_TYPE_INT:
		buffer.append( bison.asInt() );
		break;
	case ZP_BISON_TYPE_FLOAT:
		buffer.append( bison.asFloat() );
		break;
	case ZP_BISON_TYPE_STRING:
		buffer.append( '"' );
		buffer.append( bison.asCString() );
		buffer.append( '"' );
		break;
	case ZP_BISON_TYPE_ARRAY:
		{
			buffer.append( '[' );
			writeNewLine( buffer, indent );
			if( bison.size() > 0 )
			{
				zp_int ind = indent > -1 ? indent + 1 : indent;
				writeIndent( buffer, ind );
				writeBison( buffer, bison[ (zp_uint)0 ], ind );

				for( zp_uint i = 1; i < bison.size(); ++i )
				{
					buffer.append( ',' );
					writeNewLine( buffer, ind );
					writeIndent( buffer, ind );
					writeBison( buffer, bison[ i ], ind );
				}
			}
			writeNewLine( buffer, indent );
			writeIndent( buffer, indent );
			buffer.append( ']' );
		}
		break;
	case ZP_BISON_TYPE_OBJECT:
		{
			buffer.append( '{' );
			writeNewLine( buffer, indent );
			if( bison.size() > 0 )
			{
				zp_int ind = indent > -1 ? indent + 1 : indent;

				zpArrayList< zpBison::Value > members;
				bison.memberNames( members );

				writeIndent( buffer, ind );

				buffer.append( '"' );
				buffer.append( members[ 0 ].asCString() );
				buffer.append( '"' );

				buffer.append( ':' );

				writeBison( buffer, bison[ members[ 0 ].asCString() ], ind );

				for( zp_uint i = 1; i < members.size(); ++i )
				{
					buffer.append( ',' );
					writeNewLine( buffer, ind );
					writeIndent( buffer, ind );

					buffer.append( '"' );
					buffer.append( members[ i ].asCString() );
					buffer.append( '"' );

					buffer.append( ':' );

					writeBison( buffer, bison[ members[ i ].asCString() ], ind );
				}
			}
			writeNewLine( buffer, indent );
			writeIndent( buffer, indent );
			buffer.append( '}' );
		}
		break;
	}
}
void zpBisonWriter::writeIndent( zpStringBuffer& buffer, zp_int indent )
{
	for( zp_int i = 0; i < indent; ++i )
	{
		buffer.append( '\t' );
	}
}
void zpBisonWriter::writeNewLine( zpStringBuffer& buffer, zp_int indent )
{
	if( indent > -1 )
	{
		buffer.append( '\n' );
	}
}