#include "zpCore.h"

enum ops
{
	json_null = '\0',
	json_obj_open = '{',
	json_obj_close = '}',
	json_arr_open = '[',
	json_arr_close = ']',
	json_fslash = '/',
	json_bslash = '\\',
	json_true_open = 't',
	json_false_open = 'f',
	json_null_open = 'n',
	json_member_sep = ':',
	json_arr_sep = ',',


	json_dash = '-',
	json_space = ' ',
	json_quote = '\'',
	json_dquote = '"',
	json_newline = '\n',
	json_cret = '\r',
	json_tab = '\t',
};

const zpJson zpJson::null;
zpJson::zpJson()
	: m_type( ZP_JSON_TYPE_NULL )
	, m_data( 0 )
{}
zpJson::zpJson( zpJsonType type )
	: m_type( type )
	, m_data( 0 )
{}
zpJson::zpJson( zp_bool value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_bool( value )
{}
zpJson::zpJson( zp_int value )
	: m_type( ZP_JSON_TYPE_INT )
	, m_int( value )
{}
zpJson::zpJson( zp_uint value )
	: m_type( ZP_JSON_TYPE_UINT )
	, m_uint( value )
{}
zpJson::zpJson( zp_long value )
	: m_type( ZP_JSON_TYPE_LONG )
	, m_long( value )
{}
zpJson::zpJson( zp_ulong value )
	: m_type( ZP_JSON_TYPE_ULONG )
	, m_ulong( value )
{}
zpJson::zpJson( zp_float value )
	: m_type( ZP_JSON_TYPE_FLOAT )
	, m_float( value )
{}
zpJson::zpJson( zp_double value )
	: m_type( ZP_JSON_TYPE_DOUBLE )
	, m_double( value )
{}
zpJson::zpJson( const zp_char* value )
	: m_type( ZP_JSON_TYPE_STRING )
	, m_string( new zpString( value ) )
{}
zpJson::zpJson( const zpString& value )
	: m_type( ZP_JSON_TYPE_STRING )
	, m_string( new zpString( value ) )
{}
zpJson::zpJson( const zpJson& json )
	: m_type( json.m_type )
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_STRING:
		m_string = new zpString( *json.m_string );
		break;
	case ZP_JSON_TYPE_ARRAY:
		m_array = new zpArrayList< zpJson >( *json.m_array );
		break;
	case ZP_JSON_TYPE_OBJECT:
		m_object = new zpHashMap< zpString, zpJson >( *json.m_object );
		break;
	default:
		m_data = json.m_data;
		break;
	}
}
zpJson::zpJson( zpJson&& json )
	: m_type( json.m_type )
	, m_data( json.m_data )
{
	json.m_type = ZP_JSON_TYPE_NULL;
	json.m_data = 0;
}
zpJson::~zpJson()
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_STRING:
		ZP_SAFE_DELETE( m_string );
		break;
	case ZP_JSON_TYPE_ARRAY:
		ZP_SAFE_DELETE( m_array );
		break;
	case ZP_JSON_TYPE_OBJECT:
		ZP_SAFE_DELETE( m_object );
		break;
	default:
		break;
	}
}

void zpJson::operator=( const zpJson& json )
{
	m_type = json.m_type;
	switch( m_type )
	{
	case ZP_JSON_TYPE_STRING:
		m_string = new zpString( *json.m_string );
		break;
	case ZP_JSON_TYPE_ARRAY:
		m_array = new zpArrayList< zpJson >( *json.m_array );
		break;
	case ZP_JSON_TYPE_OBJECT:
		m_object = new zpHashMap< zpString, zpJson >( *json.m_object );
		break;
	default:
		m_data = json.m_data;
		break;
	}
}
void zpJson::operator=( zpJson&& json )
{
	m_type = json.m_type;
	m_data = json.m_data;

	json.m_type = ZP_JSON_TYPE_NULL;
	json.m_data = 0;
}

zpJsonType zpJson::type() const
{
	return m_type;
}
zp_uint zpJson::size() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_ARRAY:
		return m_array->size();
	case ZP_JSON_TYPE_OBJECT:
		return m_object->size();
	default:
		return 0;
	}
}
zp_bool zpJson::isEmpty() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return true;
	case ZP_JSON_TYPE_ARRAY:
		return m_array->isEmpty();
	case ZP_JSON_TYPE_OBJECT:
		return m_object->isEmpty();
	default:
		return false;
	}
}

zp_bool zpJson::isNull() const
{
	return m_type == ZP_JSON_TYPE_NULL;
}
zp_bool zpJson::isBool() const
{
	return m_type == ZP_JSON_TYPE_BOOL;
}
zp_bool zpJson::isInt() const
{
	return m_type == ZP_JSON_TYPE_INT;
}
zp_bool zpJson::isUInt() const
{
	return m_type == ZP_JSON_TYPE_UINT;
}
zp_bool zpJson::isLong() const
{
	return m_type == ZP_JSON_TYPE_LONG;
}
zp_bool zpJson::isULong() const
{
	return m_type == ZP_JSON_TYPE_ULONG;
}
zp_bool zpJson::isFloat() const
{
	return m_type == ZP_JSON_TYPE_FLOAT;
}
zp_bool zpJson::isDouble() const
{
	return m_type == ZP_JSON_TYPE_DOUBLE;
}
zp_bool zpJson::isString() const
{
	return m_type == ZP_JSON_TYPE_STRING;
}
zp_bool zpJson::isArray() const
{
	return m_type == ZP_JSON_TYPE_NULL || m_type == ZP_JSON_TYPE_ARRAY;
}
zp_bool zpJson::isObject() const
{
	return m_type == ZP_JSON_TYPE_NULL || m_type == ZP_JSON_TYPE_OBJECT;
}

zp_bool zpJson::isIntegral() const
{
	return m_type >= ZP_JSON_TYPE_BOOL && m_type <= ZP_JSON_TYPE_ULONG;
}
zp_bool zpJson::isReal() const
{
	return m_type == ZP_JSON_TYPE_FLOAT || m_type == ZP_JSON_TYPE_DOUBLE;
}
zp_bool zpJson::isNumeric() const
{
	return isIntegral() || isReal();
}

zp_bool zpJson::asBool() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return false;
	case ZP_JSON_TYPE_BOOL:
		return m_bool;
	case ZP_JSON_TYPE_INT:
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return m_data != 0;
	case ZP_JSON_TYPE_FLOAT:
		return m_float != 0.0f;
	case ZP_JSON_TYPE_DOUBLE:
		return m_double != 0.0;
	case ZP_JSON_TYPE_STRING:
		return m_string && !m_string->isEmpty();
	case ZP_JSON_TYPE_ARRAY:
		return m_array && !m_array->isEmpty();
	case ZP_JSON_TYPE_OBJECT:
		return m_object && !m_object->isEmpty();
	default:
		return false;
	}
}
zp_int zpJson::asInt() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return 0;
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? 1 : 0;
	case ZP_JSON_TYPE_INT:
		return (zp_int)m_int;
	case ZP_JSON_TYPE_UINT:
		return (zp_int)m_uint;
	case ZP_JSON_TYPE_LONG:
		return (zp_int)m_long;
	case ZP_JSON_TYPE_ULONG:
		return (zp_int)m_ulong;
	case ZP_JSON_TYPE_FLOAT:
		return (zp_int)m_float;
	case ZP_JSON_TYPE_DOUBLE:
		return (zp_int)m_double;
	default:
		ZP_ASSERT( false, "" );
		return 0;
	}
}
zp_uint zpJson::asUInt() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return 0;
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? 1 : 0;
	case ZP_JSON_TYPE_INT:
		return (zp_uint)m_int;
	case ZP_JSON_TYPE_UINT:
		return (zp_uint)m_uint;
	case ZP_JSON_TYPE_LONG:
		return (zp_uint)m_long;
	case ZP_JSON_TYPE_ULONG:
		return (zp_uint)m_ulong;
	case ZP_JSON_TYPE_FLOAT:
		return (zp_uint)m_float;
	case ZP_JSON_TYPE_DOUBLE:
		return (zp_uint)m_double;
	default:
		ZP_ASSERT( false, "" );
		return 0;
	}
}
zp_long zpJson::asLong() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return 0;
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? 1 : 0;
	case ZP_JSON_TYPE_INT:
		return (zp_long)m_int;
	case ZP_JSON_TYPE_UINT:
		return (zp_long)m_uint;
	case ZP_JSON_TYPE_LONG:
		return (zp_long)m_long;
	case ZP_JSON_TYPE_ULONG:
		return (zp_long)m_ulong;
	case ZP_JSON_TYPE_FLOAT:
		return (zp_long)m_float;
	case ZP_JSON_TYPE_DOUBLE:
		return (zp_long)m_double;
	default:
		ZP_ASSERT( false, "" );
		return 0;
	}
}
zp_ulong zpJson::asULong() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return 0;
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? 1 : 0;
	case ZP_JSON_TYPE_INT:
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return (zp_ulong)m_data;
	case ZP_JSON_TYPE_FLOAT:
		return (zp_ulong)m_float;
	case ZP_JSON_TYPE_DOUBLE:
		return (zp_ulong)m_double;
	default:
		ZP_ASSERT( false, "" );
		return 0;
	}
}
zp_float zpJson::asFloat() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return 0.0f;
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? 1.0f : 0.0f;
	case ZP_JSON_TYPE_INT:
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return (zp_float)m_data;
	case ZP_JSON_TYPE_FLOAT:
		return m_float;
	case ZP_JSON_TYPE_DOUBLE:
		return (zp_float)m_double;
	default:
		ZP_ASSERT( false, "" );
		return 0.0f;
	}
}
zp_double zpJson::asDouble() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return 0.0;
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? 1.0 : 0.0;
	case ZP_JSON_TYPE_INT:
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return (zp_double)m_data;
	case ZP_JSON_TYPE_FLOAT:
		return (zp_double)m_float;
	case ZP_JSON_TYPE_DOUBLE:
		return m_double;
	default:
		ZP_ASSERT( false, "" );
		return 0.0;
	}
}
zpString zpJson::asString() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return zpString( "" );
	case ZP_JSON_TYPE_BOOL:
		return zpString( m_bool ? "true" : "false" );
	case ZP_JSON_TYPE_STRING:
		return *m_string;
	default:
		ZP_ASSERT( false, "" );
		return zpString( "" );
	}
}
const zp_char* zpJson::asCString() const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_NULL:
		return "";
	case ZP_JSON_TYPE_BOOL:
		return m_bool ? "true" : "false";
	case ZP_JSON_TYPE_STRING:
		return m_string->getChars();
	default:
		ZP_ASSERT( false, "" );
		return "";
	}
}

zpJson& zpJson::operator[]( zp_uint index )
{
	ZP_ASSERT( isArray(), "" );

	if( m_type == ZP_JSON_TYPE_NULL )
	{
		m_type = ZP_JSON_TYPE_ARRAY;
		m_array = new zpArrayList< zpJson >();
	}

	while( m_array->size() < ( index + 1 ) )
	{
		m_array->pushBackEmpty();
	}
	return (*m_array)[ index ];
}
const zpJson& zpJson::operator[]( zp_uint index ) const
{
	switch( m_type )
	{
	case ZP_JSON_TYPE_ARRAY:
		return m_array->at( index );
	default:
		return null;
	}
}

zpJson& zpJson::operator[]( const zp_char* key )
{
	return this->operator[]( zpString( key ) );
}
const zpJson& zpJson::operator[]( const zp_char* key ) const
{
	return this->operator[]( zpString( key ) );
}

zpJson& zpJson::operator[]( const zpString& key )
{
	ZP_ASSERT( isObject(), "" );

	if( m_type == ZP_JSON_TYPE_NULL )
	{
		m_type = ZP_JSON_TYPE_OBJECT;
		m_object = new zpHashMap< zpString, zpJson >();
	}

	return (*m_object)[ key ];
}
const zpJson& zpJson::operator[]( const zpString& key ) const
{
	ZP_ASSERT( isObject(), "" );

	switch( m_type )
	{
	case ZP_JSON_TYPE_OBJECT:
		return m_object->get( key );
	default:
		return null;
	}
}

void zpJson::memberNames( zpArrayList< zpString >& names ) const
{
	ZP_ASSERT( isObject(), "" );

	if( m_type == ZP_JSON_TYPE_OBJECT )
	{
		m_object->keys( names );
	}
}

#define JSON_IS_NULL( j, i )	( \
	(j)[ (i) + 1 ] == 'u' &&	\
	(j)[ (i) + 2 ] == 'l' &&	\
	(j)[ (i) + 3 ] == 'l' )
#define JSON_IS_TRUE( j, i )	( \
	(j)[ (i) + 1 ] == 'r' &&	\
	(j)[ (i) + 2 ] == 'u' &&	\
	(j)[ (i) + 3 ] == 'e' )
#define JSON_IS_FALSE( j, i )	( \
	(j)[ (i) + 1 ] == 'a' &&	\
	(j)[ (i) + 2 ] == 'l' &&	\
	(j)[ (i) + 3 ] == 's' &&	\
	(j)[ (i) + 4 ] == 'e' )

zpJsonParser::zpJsonParser() {}
zpJsonParser::~zpJsonParser() {}

zp_bool zpJsonParser::parseFile( const zpString& filename, zpJson& outJson )
{
	zp_bool success = false;
	zpFile jsonFile( filename );
	zpStringBuffer jsonBuffer;
	if( jsonFile.open( ZP_FILE_MODE_ASCII_READ ) )
	{
		jsonFile.readFile( jsonBuffer );
		jsonFile.close();

		if( !jsonBuffer.isEmpty() )
		{
			const zp_char* json = jsonBuffer.getChars();
			const zp_uint length = jsonBuffer.length();

			m_start = json;
			m_current = m_start;
			m_end = m_start + jsonBuffer.size();

			m_nodes.clear();
			m_nodes.pushBack( &outJson );
			
			success = readValue();
		}
	}
	
	if( !success )
	{
		outJson = zpJson::null;
	}

	return success;
}

zp_bool zpJsonParser::writeToFile( const zpJson& json, const zpString& filename )
{
	return false;
}
zp_bool zpJsonParser::writeToBuffer( const zpJson& json, const zpStringBuffer& buffer )
{
	return false;
}

zp_bool zpJsonParser::readToken( zpJsonToken& token )
{
	skipWhitespace();
	token.start = m_current;
	zp_bool ok = true;

	zp_char c = nextChar();
	switch( c )
	{
	case json_obj_open:
		token.type = zpJsonToken::ZP_JSON_TOKEN_OBJECT_OPEN;
		break;
	case json_obj_close:
		token.type = zpJsonToken::ZP_JSON_TOKEN_OBJECT_CLOSE;
		break;
	case json_arr_open:
		token.type = zpJsonToken::ZP_JSON_TOKEN_ARRAY_OPEN;
		break;
	case json_arr_close:
		token.type = zpJsonToken::ZP_JSON_TOKEN_ARRAY_CLOSE;
		break;
	case json_dquote:
		token.type = zpJsonToken::ZP_JSON_TOKEN_STRING;
		ok = readString();
		break;
	case json_fslash:
		token.type = zpJsonToken::ZP_JSON_TOKEN_COMMENT;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
		token.type = zpJsonToken::ZP_JSON_TOKEN_NUMBER;
		ok = readNumber();
		break;
	case json_true_open:
		token.type = zpJsonToken::ZP_JSON_TOKEN_TRUE;
		ok = matches( "rue" );
		break;
	case json_false_open:
		token.type = zpJsonToken::ZP_JSON_TOKEN_FALSE;
		ok = matches( "alse" );
		break;
	case json_null_open:
		token.type = zpJsonToken::ZP_JSON_TOKEN_NULL;
		ok = matches( "ull" );
		break;
	case json_member_sep:
		token.type = zpJsonToken::ZP_JSON_TOKEN_MEMBER_SEP;
		break;
	case json_arr_sep:
		token.type = zpJsonToken::ZP_JSON_TOKEN_ARRAY_SEP;
		break;
	case '\0':
		token.type = zpJsonToken::ZP_JSON_TOKEN_EOF;
		break;
	default:
		break;
	}

	if( !ok )
	{
		token.type = zpJsonToken::ZP_JSON_TOKEN_ERROR;
	}
	token.end = m_current;

	return true;
}
zp_bool zpJsonParser::readValue()
{
	zpJsonToken token;
	readToken( token );

	zp_bool ok = false;
	switch( token.type )
	{
	case zpJsonToken::ZP_JSON_TOKEN_OBJECT_OPEN:
		ok = readObject();
		break;
	case zpJsonToken::ZP_JSON_TOKEN_ARRAY_OPEN:
		ok = readArray();
		break;
	case zpJsonToken::ZP_JSON_TOKEN_STRING:
		{
			zpString str;
			tokenToString( token, str );
			currentValue() = zpJson( str );
		}
		break;
	case zpJsonToken::ZP_JSON_TOKEN_NUMBER:
		tokenToNumber( token );
		break;
	case zpJsonToken::ZP_JSON_TOKEN_TRUE:
		currentValue() = zpJson( true );
		break;
	case zpJsonToken::ZP_JSON_TOKEN_FALSE:
		currentValue() = zpJson( false );
		break;
	case zpJsonToken::ZP_JSON_TOKEN_NULL:
		currentValue() = zpJson();
		break;
	}

	return true;
}
zp_bool zpJsonParser::readObject()
{
	currentValue() = zpJson();

	zpJsonToken token;
	zpString name;

	while( readToken( token ) )
	{
		if( token.type == zpJsonToken::ZP_JSON_TOKEN_OBJECT_CLOSE && name.isEmpty() )
		{
			return true;
		}

		if( token.type != zpJsonToken::ZP_JSON_TOKEN_STRING )
		{
			break;
		}

		name = "";
		tokenToString( token, name );

		zpJsonToken colon;
		if( !readToken( colon ) || colon.type != zpJsonToken::ZP_JSON_TOKEN_MEMBER_SEP )
		{
			// missing : in member name
			return false;
		}

		zpJson& value = currentValue()[ name ];

		m_nodes.pushBack( &value );
		zp_bool ok = readValue();
		m_nodes.popBack();

		if( !ok )
		{
			// error from value
			return false;
		}

		zpJsonToken comma;
		if( !readToken( comma ) || (
			comma.type != zpJsonToken::ZP_JSON_TOKEN_OBJECT_CLOSE &&
			comma.type != zpJsonToken::ZP_JSON_TOKEN_ARRAY_SEP
			) )
		{
			// missing , or }
			return false;
		}

		if( comma.type == zpJsonToken::ZP_JSON_TOKEN_OBJECT_CLOSE )
		{
			return true;
		}
	}

	// failed to parse object
	return false;
}
zp_bool zpJsonParser::readArray()
{
	currentValue() = zpJson();
	skipWhitespace();

	if( *m_current == json_arr_close )
	{
		zpJsonToken endArray;
		readToken( endArray );
		return true;
	}

	zp_uint index = 0;
	zp_bool isDone = false;
	while( !isDone )
	{
		zpJson& value = currentValue()[ index++ ];
		m_nodes.pushBack( &value );
		zp_bool ok = readValue();
		m_nodes.popBack();

		if( !ok )
		{
			ZP_ASSERT( false, "" );
			return false;
		}

		zpJsonToken token;
		ok = readToken( token );
		while( token.type == zpJsonToken::ZP_JSON_TOKEN_COMMENT && ok )
		{
			ok = readToken( token );
		}

		zp_bool isArrayEnd = token.type == zpJsonToken::ZP_JSON_TOKEN_ARRAY_SEP || token.type == zpJsonToken::ZP_JSON_TOKEN_ARRAY_CLOSE;
		if( !ok || !isArrayEnd )
		{
			ZP_ASSERT( false, "" );
			return false;
		}

		isDone = token.type == zpJsonToken::ZP_JSON_TOKEN_ARRAY_CLOSE;
	}
	return true;
}
zp_bool zpJsonParser::readString()
{
	zp_char c = '\0';
	while( m_current != m_end )
	{
		c = nextChar();
		if( c == json_bslash )
		{
			nextChar();
		}
		else if( c == json_dquote )
		{
			break;
		}
	}

	return c == json_dquote;
}
zp_bool __jsonIn( zp_char value, zp_char a, zp_char b, zp_char c, zp_char d, zp_char e )
{
	return value == a || value == b || value == c || value == d || value == e;
}

zp_bool zpJsonParser::readNumber()
{
	for( ; m_current != m_end && ( zp_is_digit( *m_current ) || __jsonIn( *m_current, '.', 'e', 'E', '+', '-' ) ); ++m_current );
	return true;
}
zp_bool zpJsonParser::readBool()
{
	return true;
}

void zpJsonParser::skipWhitespace()
{
	for( ; m_current != m_end && zp_is_whitespace( *m_current ); ++m_current );
}
void zpJsonParser::skipComments()
{

}

zp_bool zpJsonParser::matches( const zp_char* pattern, zp_int length )
{
	if( m_end - m_current < length )
	{
		return false;
	}

	zp_int index = length;
	while( index-- )
	{
		if( m_current[ index ] != pattern[ index ] )
		{
			return false;
		}
	}

	m_current += length;
	return true;
}

void zpJsonParser::tokenToString( const zpJsonToken& token, zpString& str )
{
	const zp_char* s = token.start + 1;
	const zp_char* e = token.end - 1;
	str.reserve( e - s );

	while( s != e )
	{
		zp_char c = *s++;
		if( c == json_dquote )
		{
			break;
		}
		else if( c == json_bslash )
		{
			if( s == e )
			{
				ZP_ASSERT( false, "" );
				break;
			}
			else
			{
				zp_char esc = *s++;
				switch( esc )
				{
				case json_dquote: str.append( json_dquote ); break;
				case json_fslash: str.append( json_fslash ); break;
				case json_bslash: str.append( json_bslash ); break;
				case 'b': str.append( '\b' ); break;
				case 'f': str.append( '\f' ); break;
				case 'n': str.append( '\n' ); break;
				case 'r': str.append( '\r' ); break;
				case 't': str.append( '\t' ); break;
				case 'u':
					ZP_ASSERT( false, "todo implement utf8" );
					break;
				default:
					ZP_ASSERT( false, "bad escape sequence" );
					break;
				}
			}
		}
		else
		{
			str.append( c );
		}
	}
}
void zpJsonParser::tokenToNumber( const zpJsonToken& token )
{
	const zp_char* s = token.start;
	if( *s == json_dash )
	{
		++s;
	}

	zp_bool isDouble = false;
	for( const zp_char* i = s; i != token.end && !isDouble; ++i )
	{
		isDouble = isDouble || __jsonIn( *i, '.', 'e', 'E', '+', '-' );
	}

	if( isDouble )
	{
		tokenToDouble( token );
	}
	else
	{
		zp_int value = zp_atoi( token.start );
		currentValue() = zpJson( value );
	}
}
void zpJsonParser::tokenToDouble( const zpJsonToken& token )
{
	zp_float value = zp_atof( token.start );
	currentValue() = zpJson( value );
}

zpJson& zpJsonParser::currentValue()
{
	return *m_nodes.back();
}
zp_char zpJsonParser::nextChar()
{
	return m_current == m_end ? '\0' : *m_current++;
}


zpJsonWriter::zpJsonWriter()
{}
zpJsonWriter::~zpJsonWriter()
{}

const zp_char* zpJsonWriter::fastWrite( const zpJson& json )
{
	m_string.clear();
	writeJson( m_string, json, -1 );
	return m_string.getChars();
}
const zp_char* zpJsonWriter::styleWrite( const zpJson& json )
{
	m_string.clear();
	writeJson( m_string, json, 0 );
	return m_string.getChars();
}

void zpJsonWriter::writeJson( zpStringBuffer& buffer, const zpJson& json, zp_int indent )
{
	switch( json.type() )
	{
	case ZP_JSON_TYPE_NULL:
		buffer.append( "null" );
		break;
	case ZP_JSON_TYPE_BOOL:
		buffer.append( json.asBool() ? "true" : "false" );
		break;
	case ZP_JSON_TYPE_INT:
		buffer.append( json.asInt() );
		break;
	case ZP_JSON_TYPE_UINT:
		buffer.append( json.asUInt() );
		break;
	case ZP_JSON_TYPE_LONG:
		buffer.append( json.asLong() );
		break;
	case ZP_JSON_TYPE_ULONG:
		buffer.append( json.asULong() );
		break;
	case ZP_JSON_TYPE_FLOAT:
		buffer.append( json.asFloat() );
		break;
	case ZP_JSON_TYPE_DOUBLE:
		buffer.append( json.asDouble() );
		break;
	case ZP_JSON_TYPE_STRING:
		buffer.append( '"' );
		buffer.append( json.asCString() );
		buffer.append( '"' );
		break;
	case ZP_JSON_TYPE_ARRAY:
		{
			buffer.append( '[' );
			writeNewLine( buffer, indent );
			if( json.size() > 0 )
			{
				zp_int ind = indent > -1 ? indent + 1 : indent;
				writeIndent( buffer, ind );
				writeJson( buffer, json[ (zp_uint)0 ], ind );

				for( zp_uint i = 1; i < json.size(); ++i )
				{
					buffer.append( ',' );
					writeNewLine( buffer, ind );
					writeIndent( buffer, ind );
					writeJson( buffer, json[ i ], ind );
				}
			}
			writeNewLine( buffer, indent );
			writeIndent( buffer, indent );
			buffer.append( ']' );
		}
		break;
	case ZP_JSON_TYPE_OBJECT:
		{
			buffer.append( '{' );
			writeNewLine( buffer, indent );
			if( json.size() > 0 )
			{
				zp_int ind = indent > -1 ? indent + 1 : indent;

				zpArrayList< zpString > members;
				json.memberNames( members );

				writeIndent( buffer, ind );

				buffer.append( '"' );
				buffer.append( members[ 0 ] );
				buffer.append( '"' );
			
				writeObjectSeperator( buffer, ind );
				
				writeJson( buffer, json[ members[ 0 ] ], ind );

				for( zp_uint i = 1; i < members.size(); ++i )
				{
					buffer.append( ',' );
					writeNewLine( buffer, ind );
					writeIndent( buffer, ind );

					buffer.append( '"' );
					buffer.append( members[ i ] );
					buffer.append( '"' );

					writeObjectSeperator( buffer, ind );

					writeJson( buffer, json[ members[ i ] ], ind );
				}
			}
			writeNewLine( buffer, indent );
			writeIndent( buffer, indent );
			buffer.append( '}' );
		}
		break;
	}
}
void zpJsonWriter::writeIndent( zpStringBuffer& buffer, zp_int indent )
{
	for( zp_int i = 0; i < indent; ++i )
	{
		buffer.append( '\t' );
	}
}
void zpJsonWriter::writeNewLine( zpStringBuffer& buffer, zp_int indent )
{
	if( indent > -1 )
	{
		buffer.append( '\n' );
	}
}
void zpJsonWriter::writeObjectSeperator( zpStringBuffer& buffer, zp_int indent )
{
	if( indent > -1 )
	{
		buffer.append( " : " );
	}
	else
	{
		buffer.append( ':' );
	}
}
