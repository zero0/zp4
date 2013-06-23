#include "zpCore.h"
#include <stdio.h>

zpProperties::zpProperties()
{}
zpProperties::zpProperties( const zpString& file )
{
	load( file );
}
zpProperties::zpProperties( const zpProperties& properties )
	: m_properties( properties.m_properties )
{}
zpProperties::zpProperties( zpProperties&& properties )
	: m_properties( zp_move( properties.m_properties ) )
{}
zpProperties::~zpProperties()
{
	m_properties.clear();
}

void zpProperties::operator=( const zpProperties& properties )
{
	m_properties.clear();
	load( properties );
}
void zpProperties::operator=( zpProperties&& properties )
{
	m_properties.clear();
	m_properties = zp_move( properties.m_properties );
}

zpString& zpProperties::operator[]( const zp_char* key )
{
	return m_properties[ zpString( key ) ];
}
zpString& zpProperties::operator[]( const zpString& key )
{
	return m_properties[ key ];
}
zpString& zpProperties::operator[]( zpString&& key )
{
	return m_properties[ zp_move( key ) ];
}

zp_int zpProperties::getInt( const zpString& key ) const
{
	return zp_atoi( m_properties.get( key ).str() );
}
zp_float zpProperties::getFloat( const zpString& key ) const
{
	return zp_atof( m_properties.get( key ).str() );
}
const zpString& zpProperties::getString( const zpString& key ) const
{
	return m_properties.get( key );
}

zp_int zpProperties::getInt( const zp_char* key ) const
{
	return zp_atoi( m_properties.get( zpString( key ) ).str() );
}
zp_float zpProperties::getFloat( const zp_char* key ) const
{
	return zp_atof( m_properties.get( zpString( key ) ).str() );
}
const zpString& zpProperties::getString( const zp_char* key ) const
{
	return m_properties.get( zpString( key ) );
}

void zpProperties::setInt( const zpString& key, zp_int value )
{
	zp_char buff[16];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%d", value );
	m_properties[ key ] = buff;
}
void zpProperties::setFloat( const zpString& key, zp_float value )
{
	zp_char buff[16];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%f", value );
	m_properties[ key ] = buff;
}
void zpProperties::setString( const zpString& key, const zpString& value )
{
	m_properties[ key ] = value;
}

void zpProperties::setInt( const zp_char* key, zp_int value )
{
	zp_char buff[16];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%d", value );
	m_properties[ zpString( key ) ] = buff;
}
void zpProperties::setFloat( const zp_char* key, zp_float value )
{
	zp_char buff[16];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%f", value );
	m_properties[ zpString( key ) ] = buff;
}
void zpProperties::setString( const zp_char* key, const zpString& value )
{
	m_properties[ zpString( key ) ] = value;
}

zp_bool zpProperties::hasProperty( const zp_char* key ) const
{
	return m_properties.containsKey( zpString( key ) );
}
zp_bool zpProperties::hasProperty( const zpString& key ) const
{
	return m_properties.containsKey( key );
}
zp_bool zpProperties::find( const zpString& key, const zpString** outValue ) const
{
	return m_properties.find( key, outValue );
}

void zpProperties::load( const zpString& file )
{
	zpFile f( file );
	if( f.open( ZP_FILE_MODE_ASCII_READ ) )
	{
		zp_uint pos;

		zpStringBuffer buff;
		zpString str, line, key, value;

		while( f.readLine( buff ) > 0 )
		{
			if( buff.isEmpty() ) continue;

			str = buff.str();
			str.trim( line );

			if( line.isEmpty() || line.charAt( 0 ) == '#' || ( pos = line.indexOf( '=' ) ) == zpString::npos ) continue;

			line.substring( key, 0, pos );
			line.substring( value, pos + 1 );
			m_properties[ key ] = value;

			buff.clear();
		}
		f.close();
	}
}
void zpProperties::load( const zpProperties& properties )
{
	m_properties.putAll( properties.m_properties );
}

void zpProperties::save( const zpString& file ) const
{
	if( m_properties.isEmpty() ) return;
	
	zpFile f( file );
	if( f.open( ZP_FILE_MODE_ASCII_TRUNCATE_WRITE ) )
	{
		
		zpStringBuffer buffer;
		m_properties.foreach( [ &buffer, &f ]( const zpString& key, const zpString& value )
		{
			buffer << key << " = " << value << '\n';

			f.writeFormat( "%s", buffer.str() );
			f.flush();

			buffer.clear();
		} );
		
		f.close();
	}
}

void zpProperties::clear()
{
	m_properties.clear();
}
zp_uint zpProperties::size() const
{
	return m_properties.size();
}
zp_bool zpProperties::isEmpty() const
{
	return m_properties.isEmpty();
}

zpProperties zpProperties::getSubProperties( const zp_char* subPropertyName ) const
{
	return getSubProperties( zpString( subPropertyName ) );
}
zpProperties zpProperties::getSubProperties( const zpString& subPropertyName ) const
{
	zpProperties sub;
	zpString prop;

	m_properties.foreach( [ &sub, &subPropertyName, &prop ]( const zpString& key, const zpString& value )
	{
		zp_uint pos = key.indexOf( subPropertyName );
		if( pos != zpString::npos )
		{
			key.substring( prop, pos + subPropertyName.length() );
			if( !prop.isEmpty() )
			{
				sub[ prop ] = value;
			}
		}
	} );

	return sub;
}

void zpProperties::serialize( zpSerializedOutput* out ) {}
void zpProperties::deserialize( zpSerializedInput* in ) {}