
template<typename T>
zpLinkedList<T>::zpLinkedList() : m_size( 0 ) {
	m_root.m_next = &m_root;
	m_root.m_prev = &m_root;
}
template<typename T>
zpLinkedList<T>::zpLinkedList( const zpLinkedList& list ) : m_size( 0 ) {
	m_root.m_next = &m_root;
	m_root.m_prev = &m_root;
	pushBackAll( list );
}
template<typename T>
zpLinkedList<T>::zpLinkedList( zpLinkedList&& list ) : m_size( list.m_size ), m_root( list.m_root ) {
	list.m_root.m_next = &list.m_root;
	list.m_root.m_prev = &list.m_root;
}
template<typename T>
zpLinkedList<T>::~zpLinkedList() {
	clear();
}

template<typename T>
void zpLinkedList<T>::pushBack( const T& val ) {
	addNode( val, &m_root, m_root.m_prev );
}
template<typename T>
void zpLinkedList<T>::pushBack( T&& val ) {
	addNode( (T&&)val, &m_root, m_root.m_prev );
}

template<typename T>
void zpLinkedList<T>::pushFront( const T& val ) {
	addNode( val, m_root.m_next, &m_root );
}
template<typename T>
void zpLinkedList<T>::pushFront( T&& val ) {
	addNode( (T&&)val, m_root.m_next, &m_root );
}

template<typename T>
void zpLinkedList<T>::popBack( zp_uint count ) {
	//if( isEmpty() ) return;
	while( !isEmpty() && count --> 0 ) {
		removeNode( m_root.m_prev );
	}
}
template<typename T>
void zpLinkedList<T>::popFront( zp_uint count ) {
	//if( isEmpty() ) return;
	while( !isEmpty() && count --> 0 ) {
		removeNode( m_root.m_next );
	}
}

template<typename T>
void zpLinkedList<T>::pushBackAll( const zpLinkedList<T>& list ) {
	list.foreach( []( const T& val ) {
		pushBack( val );
	});
}
template<typename T>
void zpLinkedList<T>::pushFrontAll( const zpLinkedList<T>& list ) {
	list.foreach( []( const T& val ) {
		pushFront( val );
	});
}

template<typename T>
zp_bool zpLinkedList<T>::contains( const T& val ) const {
	const zpLinkedListNode<T>* forward = m_root.m_next;
	const zpLinkedListNode<T>* backward = m_root.m_prev;

	while( forward != backward ) {
		if( val == forward->m_value ) return true;
		if( val == backward->m_value ) return true;

		forward = forward->m_next;
		backward = backward->m_prev;
	}
	return false;
}

template<typename T>
zp_bool zpLinkedList<T>::removeFirst( const T& val ) {
	zpLinkedListNode<T>* ring = m_root.m_next;
	const zpLinkedListNode<T>* head = &m_root;

	while( ring != head ) {
		if( val == ring->m_value ) {
			removeNode( ring );
			return true;
		}
		ring = ring->m_next;
	}
	return false;
}
template<typename T>
zp_bool zpLinkedList<T>::removeLast( const T& val ) {
	zpLinkedListNode<T>* ring = m_root.m_prev;
	const zpLinkedListNode<T>* head = &m_root;

	while( ring != head ) {
		if( val == ring->m_value ) {
			removeNode( ring );
			return true;
		}
		ring = ring->m_prev;
	}
	return false;
}
template<typename T>
zp_uint zpLinkedList<T>::removeAll( const T& val ) {
	zpLinkedListNode<T>* ring = m_root.m_next;
	const zpLinkedListNode<T>* head = &m_root;
	zp_uint numRemoved = 0;
	
	while( ring != head ) {
		if( val == ring->m_value ) {
			removeNode( ring );
			++numRemoved;
		}
		ring = ring->m_next;
	}
	return numRemoved;
}

template<typename T>
void zpLinkedList<T>::clear() {
	zpLinkedListNode<T>* ring = m_root.m_next;
	const zpLinkedListNode<T>* head = &m_root;

	while( ring != head ) {
		removeNode( ring );
		ring = ring->m_next;
	}
}

template<typename T>
const T& zpLinkedList<T>::front() const {
	return m_root.m_next.m_value;
}
template<typename T>
const T& zpLinkedList<T>::back() const {
	return m_root.m_prev.m_value;
}

template<typename T>
T& zpLinkedList<T>::front() {
	return m_root.m_next.m_value;
}
template<typename T>
T& zpLinkedList<T>::back() {
	return m_root.m_prev.m_value;
}

template<typename T>
zp_bool zpLinkedList<T>::isEmpty() const {
	return m_size == 0;
}
template<typename T>
zp_uint zpLinkedList<T>::size() const {
	return m_size;
}

template<typename T> template<typename Func>
void zpLinkedList<T>::foreach( Func func ) const {
	const zpLinkedListNode<T>* ring = m_root.m_next;
	const zpLinkedListNode<T>* head = &m_root;

	while( ring != head ) {
		func( ring->m_value );
		ring = ring->m_next;
	}
}
template<typename T> template<typename Func>
void zpLinkedList<T>::foreachIndexed( Func func ) const {
	const zpLinkedListNode<T>* ring = m_root.m_next;
	const zpLinkedListNode<T>* head = &m_root;
	zp_uint index = 0;
	
	while( ring != head ) {
		func( index, ring->m_value );
		++index;
		ring = ring->m_next;
	}
}

template<typename T> template<typename Func>
zp_uint zpLinkedList<T>::removeIf( Func func ) {
	zpLinkedListNode<T>* ring = m_root.m_next;
	const zpLinkedListNode<T>* head = &m_root;
	zp_uint numRemoved = 0;
	
	while( ring != head ) {
		if( func( ring->m_value ) ) {
			removeNode( ring );
			++numRemoved;
		}
		ring = ring->m_next;
	}
	return numRemoved;
}

template<typename T>
void zpLinkedList<T>::addNode( const T& val, zpLinkedListNode<T>* next, zpLinkedListNode<T>* prev ) {
	zpLinkedListNode<T>* node = new zpLinkedListNode<T>( val, next, prev );
	
	node->m_next->m_prev = node;
	node->m_prev->m_next = node;
	++m_size;
}
template<typename T>
void zpLinkedList<T>::addNode( T&& val, zpLinkedListNode<T>* next, zpLinkedListNode<T>* prev ) {
	zpLinkedListNode<T>* node = new zpLinkedListNode<T>( (T&&)val, next, prev );
	
	node->m_next->m_prev = node;
	node->m_prev->m_next = node;
	++m_size;
}
template<typename T>
void zpLinkedList<T>::removeNode( zpLinkedListNode<T>* node, zp_bool andDeallocNode ) {
	node->m_prev->m_next = node->m_next;
	node->m_next->m_prev = node->m_prev;
	--m_size;

	if( andDeallocNode ) {
		delete node;
	}
}