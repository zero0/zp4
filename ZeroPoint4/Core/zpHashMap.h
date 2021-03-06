#pragma once
#ifndef ZP_HASH_MAP_H
#define ZP_HASH_MAP_H

#define ZP_HASH_MAP_DEFAULT_LOAD_FACTOR        0.85f
#define ZP_HASH_MAP_DEFAULT_CAPACITY        16

template<typename Key, typename Value>
class zpHashMap {
public:
    zpHashMap();
    explicit zpHashMap( zp_uint size, zp_float loadFactor = ZP_HASH_MAP_DEFAULT_LOAD_FACTOR );
    zpHashMap( const zpHashMap& map );
    zpHashMap( zpHashMap&& map );
    ~zpHashMap();

    void operator=( const zpHashMap& map );
    void operator=( zpHashMap&& map );

    Value& operator[]( const Key& key );
    Value& operator[]( Key&& key );

    zp_uint size() const;
    zp_bool isEmpty() const;

    const Value& get( const Key& key ) const;

    void put( const Key& key, const Value& value );
    void put( Key&& key, const Value& value );
    void put( Key&& key, Value&& value );

    void putAll( const zpHashMap& map );
    
    zp_bool containsKey( const Key& key ) const;
    zp_bool containsValue( const Value& value ) const;

    zp_bool erase( const Key& key );

    zp_bool find( const Key& key, const Value** outValue ) const;
    zp_bool find( const Key& key, Value** outValue );

    void keys( zpArrayList< Key >& keys ) const;
    void values( zpArrayList< Value >& values ) const;

    void reserve( zp_uint size );

    void clear();

    template<typename Func>
    void foreach( Func func ) const;

private:
    zp_hash generateHash( zp_hash hash ) const;
    void resize( zp_uint newSize );

    struct zpMapEntity
    {
        zpMapEntity()
            : key()
            , value()
            , hash( 0 )
            , next( ZP_NULL )
        {}
        zpMapEntity( const Key& key, const Value& value, zp_hash hash, zpMapEntity* next )
            : key( key )
            , value( value )
            , hash( hash )
            , next( next )
        {};
        zpMapEntity( Key&& key, const Value& value, zp_hash hash, zpMapEntity* next )
            : key( zp_move( key ) )
            , value( value )
            , hash( hash )
            , next( next )
        {};
        zpMapEntity( Key&& key, Value&& value, zp_hash hash, zpMapEntity* next )
            : key( zp_move( key ) )
            , value( zp_move( value ) )
            , hash( hash )
            , next( next )
        {};

        Key key;
        Value value;
        zp_hash hash;
        zpMapEntity* next;
    };

    void addMapEntity( const Key& key, const Value& value, zp_hash hash, zp_uint index, zpMapEntity** entity );
    void addMapEntity( Key&& key, const Value& value, zp_hash hash, zp_uint index, zpMapEntity** entity );
    void addMapEntity( Key&& key, Value&& value, zp_hash hash, zp_uint index, zpMapEntity** entity );
    
    zpMapEntity** m_map;
    zp_uint m_capacity;
    zp_uint m_size;
    zp_float m_loadFactor;
};

#include "zpHashMap.inl"

#endif