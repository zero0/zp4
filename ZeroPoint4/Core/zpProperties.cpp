#include "zpCore.h"
#include <stdio.h>

zpProperties::zpProperties() {}
zpProperties::zpProperties( const zpString& file ) {
	load( file );
}
zpProperties::zpProperties( const zpProperties& properties ) : m_properties( properties.m_properties ) {}
zpProperties::zpProperties( zpProperties&& properties ) : m_properties( (zpHashMap<zpString, zpString>&&)properties.m_properties ) {}
zpProperties::~zpProperties() {
	m_properties.clear();
}

void zpProperties::operator=( const zpProperties& properties ) {
	m_properties.clear();
	load( properties );
}
void zpProperties::operator=( zpProperties&& properties ) {
	m_properties.clear();
	m_properties = (zpHashMap<zpString, zpString>&&)properties.m_properties;
}

zpString& zpProperties::operator[]( const zpString& key ) {
	return m_properties[ key ];
}
zpString& zpProperties::operator[]( zpString&& key ) {
	return m_properties[ (zpString&&)key ];
}

zp_int zpProperties::getInt( const zpString& key ) const {
	zp_int value;
	m_properties.get( key ).scan( "%d", &value );
	return value;
}
zp_float zpProperties::getFloat( const zpString& key ) const {
	zp_float value;
	m_properties.get( key ).scan( "%f", &value );
	return value;
}
const zpString& zpProperties::getString( const zpString& key ) const {
	return m_properties.get( key );
}

void zpProperties::setInt( const zpString& key, zp_int value ) {
	zp_char buff[16];
	sprintf_s( buff, "%d", value );
	m_properties[ key ] = buff;
}
void zpProperties::setFloat( const zpString& key, zp_float value ) {
	zp_char buff[16];
	sprintf_s( buff, "%f", value );
	m_properties[ key ] = buff;
}
void zpProperties::setString( const zpString& key, const zpString& value ) {
	m_properties[ key ] = value;
}

zp_bool zpProperties::hasProperty( const zpString& key ) const {
	return m_properties.containsKey( key );
}

void zpProperties::load( const zpString& file ) {
	FILE* f = ZP_NULL;
	if( fopen_s( &f, file.c_str(), "r" ) == 0 ) {
		zp_char buff[265];
		zpString str;
		zp_uint pos;
		while( fgets( buff, 256, f ) ) {
			zp_uint len = strlen( buff );
			if( len == 0 ) continue;
			buff[ len - 1 ] = '\0';

			str = buff;
			str.trim();

			if( str.isEmpty() || str.charAt( 0 ) == '#' || ( pos = str.indexOf( '=' ) ) == zpString::npos ) continue;

			m_properties[ str.substring( 0, pos ).trim() ] = str.substring( pos + 1 ).trim();
		}
		fclose( f );
	}
}
void zpProperties::load( const zpProperties& properties ) {
	m_properties.putAll( properties.m_properties );
}

void zpProperties::save( const zpString& file ) const {
	if( m_properties.isEmpty() ) return;
	
	FILE* f = ZP_NULL;
	if( fopen_s( &f, file.c_str(), "w" ) == 0 ) {
		char buff[ 64 ];
		setbuf( f, buff );

		zpStringBuffer buffer( 256 );
		m_properties.foreach( [ &buffer, f ]( const zpString& key, const zpString& value ) {
			buffer << key << " = " << value << '\n';
			
			fputs( buffer.getChars(), f );
			fflush( f );

			buffer.clear();
		} );
		
		fclose( f );
	}
}

void zpProperties::clear() {
	m_properties.clear();
}
zp_uint zpProperties::size() const {
	return m_properties.size();
}
zp_bool zpProperties::isEmpty() const {
	return m_properties.isEmpty();
}

zpProperties zpProperties::getSubProperties( const zpString& subPropertyName ) const {
	zpProperties sub;
	zpString prop;

	m_properties.foreach( [ &sub, &subPropertyName, &prop ]( const zpString& key, const zpString& value ) {
		zp_uint pos = key.indexOf( subPropertyName );
		if( pos != zpString::npos ) {
			prop = key.substring( pos + subPropertyName.length() );
			if( !prop.isEmpty() ) {
				sub[ prop ] = value;
			}
		}
	} );

	return sub;
}

void zpProperties::serialize( zpSerializedOutput* out ) {}
void zpProperties::deserialize( zpSerializedInput* in ) {}