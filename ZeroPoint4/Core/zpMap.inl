
template<typename Key, typename Value>
zpMap<Key, Value>::zpMap() : m_map( 8 ) {}
template<typename Key, typename Value>
zpMap<Key, Value>::zpMap( zp_uint size ) : m_map( size ) {}
template<typename Key, typename Value>
zpMap<Key, Value>::zpMap( const zpMap& map ) : m_map( map.size() ) {
    putAll( map );
}
template<typename Key, typename Value>
zpMap<Key, Value>::zpMap( zpMap&& map ) : m_map( (zpMapEntity&&)map.m_map ) {}
template<typename Key, typename Value>
zpMap<Key, Value>::~zpMap() {}

template<typename Key, typename Value>
void zpMap<Key, Value>::operator=( const zpMap& map ) {
    clear();
    putAll( map );
}
template<typename Key, typename Value>
void zpMap<Key, Value>::operator=( zpMap&& map ) {
    m_map = (zpMapEntity&&)map.m_map;
}

template<typename Key, typename Value>
Value& zpMap<Key, Value>::operator[]( const Key& key ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) return entity.second();
    }

    m_map.pushBack( zpMapEntity( key, Value() ) );
    return m_map.back().second();
}
template<typename Key, typename Value>
Value& zpMap<Key, Value>::operator[]( Key&& key ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) return entity.second();
    }

    m_map.pushBack( zpMapEntity( (Key&&)key, Value() ) );
    return m_map.back().second();
}

template<typename Key, typename Value>
zp_uint zpMap<Key, Value>::size() const {
    return m_map.size();
}
template<typename Key, typename Value>
zp_uint zpMap<Key, Value>::capacity() const {
    return m_map.capacity();
}
template<typename Key, typename Value>
zp_bool zpMap<Key, Value>::isEmpty() const {
    return m_map.isEmpty();
}
template<typename Key, typename Value>
zp_bool zpMap<Key, Value>::isFull() const {
    return m_map.isFull();
}

template<typename Key, typename Value>
const Value& zpMap<Key, Value>::get( const Key& key ) const {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        const zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) return entity.second();
    }
    throw;
}

template<typename Key, typename Value>
void zpMap<Key, Value>::put( const Key& key, const Value& value ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) {
            entity.second() = value;
            return;
        }
    }
    m_map.pushBack( zpMapEntity( key, value ) );
}
template<typename Key, typename Value>
void zpMap<Key, Value>::put( Key&& key, const Value& value ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) {
            entity.second() = value;
            return;
        }
    }
    m_map.pushBack( zpMapEntity( (Key&&)key, value ) );
}
template<typename Key, typename Value>
void zpMap<Key, Value>::put( Key&& key, Value&& value ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) {
            entity.second() = (Value&&)value;
            return;
        }
    }
    m_map.pushBack( zpMapEntity( (Key&&)key, (Value&&)value ) );
}

template<typename Key, typename Value>
void zpMap<Key, Value>::putAll( const zpMap& map ) {
    m_map.ensureCapacity( m_map.capacity() + map.size() );

    map.foreach( []( const zpMapEntity& pair ) {
        put( pair.first(), pair.second() );
    });
}

template<typename Key, typename Value>
zp_bool zpMap<Key, Value>::containsKey( const Key& key ) const {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        if( m_map[ i ].first() == key ) return true;
    }
    return false;
}
template<typename Key, typename Value>
zp_bool zpMap<Key, Value>::containsValue( const Value& value ) const {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        if( m_map[ i ].second() == value ) return true;
    }
    return false;
}

template<typename Key, typename Value>
zp_bool zpMap<Key, Value>::remove( const Key& key, Value* outValue = ZP_NULL ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) {
            if( outValue ) *outValue = entity.second();
            m_map.erase( i );
            return true;
        }
    }
    return false;
}
template<typename Key, typename Value>
zp_bool zpMap<Key, Value>::erase( const Key& key ) {
    for( zp_uint i = 0; i < m_map.size(); ++i ) {
        zpMapEntity& entity = m_map[ i ];
        if( entity.first() == key ) {
            m_map.erase( i );
            return true;
        }
    }
    return false;
}

template<typename Key, typename Value>
void zpMap<Key, Value>::clear() {
    m_map.clear();
}

template<typename Key, typename Value> template<typename Func>
void zpMap<Key, Value>::foreach( Func func ) const {
    m_map.foreach( [ &func ]( const zpMapEntity& pair ) {
        func( pair.first(), pair.second() );
    });
}