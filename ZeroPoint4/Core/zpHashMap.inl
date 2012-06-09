
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap() : 
	m_map( 16 ),
	m_size( 0 ),
	m_loadFactor( ZP_HASH_MAP_DEFAULT_LOAD_FACTOR )
{}
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap( zp_uint size, zp_float loadFactor ) :
	m_map( zp_near_pow2( size ) ),
	m_size( 0 ),
	m_loadFactor( loadFactor )
{}
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap( const zpHashMap& map ) :
	m_map( map.m_map ),
	m_size( map.m_size ),
	m_loadFactor( map.m_loadFactor )
{}
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap( zpHashMap&& map ) :
	m_map( (zpMapEntity<Key, Value>&&)map.m_map ),
	m_size( map.m_size ),
	m_loadFactor( map.m_loadFactor )
{}
template<typename Key, typename Value>
zpHashMap<Key, Value>::~zpHashMap() {
	clear();
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::operator=( const zpHashMap& map ) {
	m_map.clear();
	m_map = map.m_map;
	m_size = map.m_size;
	m_loadFactor = map.m_loadFactor;
}
template<typename Key, typename Value>
void zpHashMap<Key, Value>::operator=( zpHashMap&& map ) {
	m_map.clear();
	m_map = (zpMapEntity<Key, Value>&&)map.m_map;
	m_size = map.m_size;
	m_loadFactor = map.m_loadFactor;
}

template<typename Key, typename Value>
Value& zpHashMap<Key, Value>::operator[]( const Key& key ) {
	zp_hash hash = (zp_hash)key;
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			return e->value;
		}
	}
	zpMapEntity* entity;
	addMapEntity( key, Value(), hash, index, &entity );
	return entity->value;
}
template<typename Key, typename Value>
Value& zpHashMap<Key, Value>::operator[]( Key&& key ) {
	zp_hash hash = (zp_hash)key;
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			return e->value;
		}
	}
	zpMapEntity* entity;
	addMapEntity( (Key&&)key, Value(), hash, index, &entity );
	return entity->value;
}

template<typename Key, typename Value>
zp_uint zpHashMap<Key, Value>::size() const {
	return m_size;
}
template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::isEmpty() const {
	return m_size == 0;
}

template<typename Key, typename Value>
const Value& zpHashMap<Key, Value>::get( const Key& key ) const {
	zp_hash hash = hash( (zp_hash)key );
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			return e->value;
		}
	}
	throw;
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::put( const Key& key, const Value& value ) {
	zp_hash hash = hash( (zp_hash)key );
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			(&e->value)->~Value();
			e->value = value;
			return;
		}
	}
	addMapEntity( key, value, hash, index, ZP_NULL );
}
template<typename Key, typename Value>
void zpHashMap<Key, Value>::put( Key&& key, const Value& value ) {
	zp_hash hash = hash( (zp_hash)key );
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			(&e->value)->~Value();
			e->value = value;
			return;
		}
	}
	addMapEntity( (Key&&)key, value, hash, index, ZP_NULL );
}
template<typename Key, typename Value>
void zpHashMap<Key, Value>::put( Key&& key, Value&& value ) {
	zp_hash hash = hash( (zp_hash)key );
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			(&e->value)->~Value();
			e->value = (Value&&)value;
			return;
		}
	}
	addMapEntity( (Key&&)key, (Value&&)value, hash, index, ZP_NULL );
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::putAll( const zpHashMap& map ) {
	map.m_map.foreach( [ this ]( zpMapEntity* entity ) {
		put( entity->key, entity->value );
	} );
}

template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::containsKey( const Key& key ) const {
	zp_hash hash = (zp_hash)key;
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next ) {
		if( e->hash == hash && e->key == key ) {
			return true;
		}
	}
	return false;
}
template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::containsValue( const Value& value ) const {
	for( zp_uint i = 0; i < m_map.size(); ++i ) {
		for( zpMapEntity* e = m_map[ i ]; e != ZP_NULL; e = e->next ) {
			if( e->value == value ) {
				return true;
			}
		}
	}
	return false;
}

template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::remove( const Key& key, Value* outValue = ZP_NULL ) {
	zp_hash hash = (zp_hash)key;
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ], *prev = e, *next; e != ZP_NULL; prev = e, e = next ) {
		next = e->next;
		if( e->hash == hash && e->key == key ) {
			--m_size;
			if( prev == e ) {
				m_map[ index ] = next;
			} else {
				prev->next = next;
			}
			if( outValue ) *outValue = *e;
			ZP_SAFE_DELETE( e );
			return true;
		}
	}
	return false;
}
template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::erase( const Key& key ) {
	zp_hash hash = (zp_hash)key;
	zp_uint index = hash & m_map.size() - 1;
	for( zpMapEntity* e = m_map[ index ], *prev = e, *next; e != ZP_NULL; prev = e, e = next ) {
		next = e->next;
		if( e->hash == hash && e->key == key ) {
			--m_size;
			if( prev == e ) {
				m_map[ index ] = next;
			} else {
				prev->next = next;
			}
			ZP_SAFE_DELETE( e );
			return true;
		}
	}
	return false;
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::clear() {
	for( zp_uint i = 0; i < m_map.size(); ++i ) {
		for( zpMapEntity* e = m_map[ i ], *next = e; next != ZP_NULL;  ) {
			next = e->next;
			ZP_SAFE_DELETE( e );
			e = next;
		}
	}
	m_map.clear();
	m_size = 0;
}

template<typename Key, typename Value> template<typename Func>
void zpHashMap<Key, Value>::foreach( Func func ) const {
	for( zp_uint i = 0; i < m_map.size(); ++i ) {
		for( zpMapEntity* e = m_map[ i ]; e != ZP_NULL; e = e->next ) {
			func( e->key, e->value );
		}
	}
}

template<typename Key, typename Value> 
zp_hash zpHashMap<Key, Value>::hash( zp_hash hash ) const {
	hash ^= ( hash >>> 20 ) ^ ( hash >>> 12 );
	return hash ^ ( hash >>> 7 ) ^ ( hash >>> 4 );
}

template<typename Key, typename Value> 
void zpHashMap<Key, Value>::resize( zp_uint newSize ) {
	zpArray<zpMapEntity*> newMap( newSize );
	zpMapEntity* e;
	zpMapEntity* next;
	zp_uint index;

	for( zp_uint i = 0; i < m_map.size(); ++i ) {
		e = m_map[ i ];
		if( e ) {
			m_map[ i ] = ZP_NULL;
			do {
				next = e->next;
				index = e->hash & ( newSize - 1 );
				e->next = newMap[ index ];
				newMap[ index ] = e;
				e = next;
			} while( e );
		}
	}

	m_map = (zpArray<zpMapEntity*>&&)newMap;
}

template<typename Key, typename Value> 
void zpHashMap<Key, Value>::addMapEntity( const Key& key, const Value& value, zp_hash hash, zp_uint index, zpMapEntity** entity ) {
	zpMapEntity* e = m_map[ index ];
	m_map[ index ] = new zpMapEntity( key, value, hash, e );
	if( *entity ) *entity = m_map[ index ];
	++m_size;
	if( (zp_float)m_size >= ( (zp_float)m_map.size() * m_loadFactor ) ) {
		resize( m_map.size() * 2 );
	}
}
template<typename Key, typename Value> 
void zpHashMap<Key, Value>::addMapEntity( Key&& key, const Value& value, zp_hash hash, zp_uint index, zpMapEntity** entity ) {
	zpMapEntity* e = m_map[ index ];
	m_map[ index ] = new zpMapEntity( (Key&&)key, value, hash, e );
	if( *entity ) *entity = m_map[ index ];
	++m_size;
	if( (zp_float)m_size >= ( (zp_float)m_map.size() * m_loadFactor ) ) {
		resize( m_map.size() * 2 );
	}
}
template<typename Key, typename Value> 
void zpHashMap<Key, Value>::addMapEntity( Key&& key, Value&& value, zp_hash hash, zp_uint index, zpMapEntity** entity ) {
	zpMapEntity* e = m_map[ index ];
	m_map[ index ] = new zpMapEntity( (Key&&)key, (Value&&)value, hash, e );
	if( *entity ) *entity = m_map[ index ];
	++m_size;
	if( (zp_float)m_size >= ( (zp_float)m_map.size() * m_loadFactor ) ) {
		resize( m_map.size() * 2 );
	}
}