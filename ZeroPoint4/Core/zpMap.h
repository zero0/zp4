#pragma once
#ifndef ZP_MAP_H
#define ZP_MAP_H

template<typename Key, typename Value>
class zpMap {
public:
	typedef zpPair<Key, Value> zpMapEntity;

	zpMap();
	zpMap( zp_uint size );
	zpMap( const zpMap& map );
	zpMap( zpMap&& map );
	~zpMap();

	void operator=( const zpMap& map );
	void operator=( zpMap&& map );

	Value& operator[]( const Key& key );
	Value& operator[]( Key&& key );

	zp_uint size() const;
	zp_uint capacity() const;
	zp_bool isEmpty() const;
	zp_bool isFull() const;

	const Value& get( const Key& key ) const;

	void put( const Key& key, const Value& value );
	void put( Key&& key, const Value& value );
	void put( Key&& key, Value&& value );

	void putAll( const zpMap& map );
	
	zp_bool containsKey( const Key& key ) const;
	zp_bool containsValue( const Value& value ) const;

	zp_bool remove( const Key& key, Value* outValue = ZP_NULL );
	zp_bool erase( const Key& key );

	void clear();

	template<typename Func>
	void foreach( Func func ) const;

private:
	zpArray<zpMapEntity> m_map;
};

#include "zpMap.inl"

#endif