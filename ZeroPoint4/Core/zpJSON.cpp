#include "zpCore.h"

const zpJson zpJson::null;
zpJson::zpJson()
	: m_type( ZP_JSON_TYPE_NULL )
{}
zpJson::zpJson( zp_bool value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_bool( value )
{}
zpJson::zpJson( zp_int value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_int( value )
{}
zpJson::zpJson( zp_uint value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_uint( value )
{}
zpJson::zpJson( zp_long value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_long( value )
{}
zpJson::zpJson( zp_ulong value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_ulong( value )
{}
zpJson::zpJson( zp_float value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_float( value )
{}
zpJson::zpJson( zp_double value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_double( value )
{}
zpJson::zpJson( const zp_char* value )
	: m_type( ZP_JSON_TYPE_BOOL )
	, m_string( new zpString( value ) )
{}
zpJson::zpJson( const zpString& value )
	: m_type( ZP_JSON_TYPE_BOOL )
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
		return !m_array->isEmpty();
	case ZP_JSON_TYPE_OBJECT:
		return !m_object->isEmpty();
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
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return (zp_int)m_data;
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
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return (zp_uint)m_data;
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
	case ZP_JSON_TYPE_UINT:
	case ZP_JSON_TYPE_LONG:
	case ZP_JSON_TYPE_ULONG:
		return (zp_long)m_data;
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
