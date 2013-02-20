
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap()
	: m_map( new zpMapEntity*[ ZP_HASH_MAP_DEFAULT_CAPACITY ] )
	, m_capacity( ZP_HASH_MAP_DEFAULT_CAPACITY )
	, m_size( 0 )
	, m_loadFactor( ZP_HASH_MAP_DEFAULT_LOAD_FACTOR )
{
	zp_zero_memory( m_map );
}
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap( zp_uint size, zp_float loadFactor )
	: m_map( new zpMapEntity*[ zp_near_pow2( size ) ] )
	, m_capacity( zp_near_pow2( size ) )
	, m_size( 0 )
	, m_loadFactor( loadFactor )
{
	zp_zero_memory( m_map );
}
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap( const zpHashMap& map )
	: m_map( new zpMapEntity*[ map.m_capacity ] )
	, m_capacity( map.m_capacity )
	, m_size( 0 )
	, m_loadFactor( map.m_loadFactor )
{
	zp_zero_memory( m_map );
	putAll( map );
}
template<typename Key, typename Value>
zpHashMap<Key, Value>::zpHashMap( zpHashMap&& map )
	: m_map( map.m_map )
	, m_capacity( map.m_capacity )
	, m_size( map.m_size )
	, m_loadFactor( map.m_loadFactor )
{
	map.m_map = ZP_NULL;
	map.m_size = 0;
}
template<typename Key, typename Value>
zpHashMap<Key, Value>::~zpHashMap()
{
	clear();
	ZP_SAFE_DELETE_ARRAY( m_map );
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::operator=( const zpHashMap& map )
{
	clear();
	m_loadFactor = map.m_loadFactor;
	putAll( map );
}
template<typename Key, typename Value>
void zpHashMap<Key, Value>::operator=( zpHashMap&& map )
{
	clear();
	m_map = map.m_map;
	m_capacity = map.m_capacity;
	m_size = map.m_size;
	m_loadFactor = map.m_loadFactor;

	map.m_map = ZP_NULL;
	map.m_size = 0;
}

template<typename Key, typename Value>
Value& zpHashMap<Key, Value>::operator[]( const Key& key )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			return e->value;
		}
	}

	zpMapEntity* entity;
	addMapEntity( key, Value(), h, index, &entity );
	return entity->value;
}
template<typename Key, typename Value>
Value& zpHashMap<Key, Value>::operator[]( Key&& key )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			return e->value;
		}
	}

	zpMapEntity* entity;
	addMapEntity( zp_move( key ), Value(), h, index, &entity );
	return entity->value;
}

template<typename Key, typename Value>
zp_uint zpHashMap<Key, Value>::size() const
{
	return m_size;
}
template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::isEmpty() const
{
	return m_size == 0;
}

template<typename Key, typename Value>
const Value& zpHashMap<Key, Value>::get( const Key& key ) const
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	Value* v = ZP_NULL;
	
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			v = &e->value;
			break;
		}
	}
	ZP_ASSERT( v != ZP_NULL, "zpHashMap: Unable to find key" );

	return *v;
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::put( const Key& key, const Value& value )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			(&e->value)->~Value();
			e->value = value;
			return;
		}
	}

	zpMapEntity* entity = ZP_NULL;
	addMapEntity( key, value, h, index, &entity );
}
template<typename Key, typename Value>
void zpHashMap<Key, Value>::put( Key&& key, const Value& value )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			(&e->value)->~Value();
			e->value = value;
			return;
		}
	}

	zpMapEntity* entity = ZP_NULL;
	addMapEntity( (Key&&)key, value, h, index, &entity );
}
template<typename Key, typename Value>
void zpHashMap<Key, Value>::put( Key&& key, Value&& value )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			(&e->value)->~Value();
			e->value = zp_move( value );
			return;
		}
	}

	zpMapEntity* entity = ZP_NULL;
	addMapEntity( (Key&&)key, (Value&&)value, h, index, &entity );
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::putAll( const zpHashMap& map )
{
	for( zp_uint i = 0; i < m_capacity; ++i )
	{
		for( zpMapEntity* e = m_map[ i ]; e != ZP_NULL; e = e->next )
		{
			put( e->key, e->value );
		}
	}
}

template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::containsKey( const Key& key ) const
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ]; e != ZP_NULL; e = e->next )
	{
		if( e->hash == h && e->key == key )
		{
			return true;
		}
	}
	return false;
}
template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::containsValue( const Value& value ) const
{
	for( zp_uint i = 0; i < m_capacity; ++i )
	{
		for( zpMapEntity* e = m_map[ i ]; e != ZP_NULL; e = e->next )
		{
			if( e->value == value )
			{
				return true;
			}
		}
	}
	return false;
}

template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::erase( const Key& key )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ], *prev = e, *next; e != ZP_NULL; prev = e, e = next )
	{
		next = e->next;
		if( e->hash == h && e->key == key )
		{
			--m_size;
			if( prev == e )
			{
				m_map[ index ] = next;
			}
			else
			{
				prev->next = next;
			}

			ZP_SAFE_DELETE( e );
			return true;
		}
	}
	return false;
}

template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::find( const Key& key, const Value** outValue ) const
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ], *prev = e, *next; e != ZP_NULL; prev = e, e = next )
	{
		next = e->next;
		if( e->hash == h && e->key == key )
		{
			if( outValue ) *outValue = &e->value;
			return true;
		}
	}
	return false;
}
template<typename Key, typename Value>
zp_bool zpHashMap<Key, Value>::find( const Key& key, Value** outValue )
{
	zp_hash h = generateHash( (zp_hash)key );
	zp_uint index = h & m_capacity - 1;
	for( zpMapEntity* e = m_map[ index ], *prev = e, *next; e != ZP_NULL; prev = e, e = next )
	{
		next = e->next;
		if( e->hash == h && e->key == key )
		{
			if( outValue ) *outValue = &e->value;
			return true;
		}
	}
	return false;
}

template<typename Key, typename Value>
void zpHashMap<Key, Value>::clear()
{
	if( m_map && m_size > 0 )
	{
		for( zp_uint i = 0; i < m_capacity; ++i )
		{
			for( zpMapEntity* e = m_map[ i ], *next = e; next != ZP_NULL;  )
			{
				next = e->next;
				ZP_SAFE_DELETE( e );
				e = next;
			}
			m_map[ i ] = ZP_NULL;
		}
		m_size = 0;
	}
}

template<typename Key, typename Value> template<typename Func>
void zpHashMap<Key, Value>::foreach( Func func ) const
{
	for( zp_uint i = 0; i < m_capacity; ++i )
	{
		for( zpMapEntity* e = m_map[ i ]; e != ZP_NULL; e = e->next )
		{
			func( e->key, e->value );
		}
	}
}

template<typename Key, typename Value> 
zp_hash zpHashMap<Key, Value>::generateHash( zp_hash h ) const
{
	h ^= ( h >> 20 ) ^ ( h >> 12 );
	return h ^ ( h >> 7 ) ^ ( h >> 4 );
}

template<typename Key, typename Value> 
void zpHashMap<Key, Value>::resize( zp_uint newSize )
{
	zpMapEntity** newMap = new zpMapEntity*[ newSize ];
	zp_zero_memory( newMap );

	zpMapEntity* e;
	zpMapEntity* next;
	zp_uint index;

	for( zp_uint i = 0; i < m_capacity; ++i )
	{
		e = m_map[ i ];
		if( e )
		{
			do
			{
				next = e->next;
				index = e->hash & ( newSize - 1 );
				e->next = newMap[ index ];
				newMap[ index ] = e;
				e = next;
			} while( e );
		}
	}

	ZP_SAFE_DELETE_ARRAY( m_map );
	m_map = newMap;
	m_capacity = newSize;
}

template<typename Key, typename Value> 
void zpHashMap<Key, Value>::addMapEntity( const Key& key, const Value& value, zp_hash h, zp_uint index, zpMapEntity** entity )
{
	zpMapEntity* e = m_map[ index ];
	e = new zpMapEntity( key, value, h, e );

	if( *entity ) *entity = e;

	++m_size;
	if( (zp_float)m_size >= ( (zp_float)m_capacity * m_loadFactor ) )
	{
		resize( m_capacity * 2 );
	}
}
template<typename Key, typename Value> 
void zpHashMap<Key, Value>::addMapEntity( Key&& key, const Value& value, zp_hash h, zp_uint index, zpMapEntity** entity )
{
	zpMapEntity* e = m_map[ index ];
	e = new zpMapEntity( zp_move( key ), value, h, e );

	if( *entity ) *entity = e;

	++m_size;
	if( (zp_float)m_size >= ( (zp_float)m_capacity * m_loadFactor ) )
	{
		resize( m_capacity * 2 );
	}
}
template<typename Key, typename Value> 
void zpHashMap<Key, Value>::addMapEntity( Key&& key, Value&& value, zp_hash h, zp_uint index, zpMapEntity** entity )
{
	zpMapEntity* e = m_map[ index ];
	e = new zpMapEntity( zp_move( key ), zp_move( value ), h, e );
	
	if( *entity ) *entity = e;
	
	++m_size;
	if( (zp_float)m_size >= ( (zp_float)m_capacity * m_loadFactor ) )
	{
		resize( m_capacity * 2 );
	}
}
