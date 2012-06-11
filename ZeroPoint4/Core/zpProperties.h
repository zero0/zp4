#pragma once
#ifndef ZP_PROPERTIES_H
#define ZP_PROPERTIES_H

class zpProperties : public zpSerializable {
public:
	zpProperties();
	zpProperties( const zpString& file );
	zpProperties( const zpProperties& properties );
	zpProperties( zpProperties&& properties );
	~zpProperties();

	void operator=( const zpProperties& properties );
	void operator=( zpProperties&& properties );

	zpString& operator[]( const zpString& key );
	zpString& operator[]( zpString&& key );

	zp_int getInt( const zpString& key ) const;
	zp_float getFloat( const zpString& key ) const;
	const zpString& getString( const zpString& key ) const;

	void setInt( const zpString& key, zp_int value );
	void setFloat( const zpString& key, zp_float value );
	void setString( const zpString& key, const zpString& value );

	zp_bool hasProperty( const zpString& key ) const;

	void load( const zpString& file );
	void load( const zpProperties& properties );

	void save( const zpString& file ) const;

	void clear();
	zp_uint size() const;
	zp_bool isEmpty() const;

	zpProperties getSubProperties( const zpString& subPropertyName ) const;

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

private:
	zpHashMap<zpString, zpString> m_properties;
};

#endif