
template<typename T>
zpIntrusiveList<T>::zpIntrusiveList() {
    m_root.m_next = &m_root;
    m_root.m_prev = &m_root;
}
template<typename T>
zpIntrusiveList<T>::~zpIntrusiveList() {}

template<typename T>
void zpIntrusiveList<T>::pushBack( zpIntrusiveListNode<T>* node ) {
    if( !node || node->m_next != node ) return;

    node->m_next = &m_root;
    node->m_prev = m_root.m_prev;
    node->m_next->m_prev = node;
    node->m_prev->m_next = node;
}
template<typename T>
void zpIntrusiveList<T>::pushFront( zpIntrusiveListNode<T>* node ) {
    if( !node || node->m_next != node ) return;

    node->m_next = m_root.m_next;
    node->m_prev = &m_root;
    node->m_next->m_prev = node;
    node->m_prev->m_next = node;
}
template<typename T>
void zpIntrusiveList<T>::remove( zpIntrusiveListNode<T>* node ) {
    node->m_prev->m_next = node->m_next;
    node->m_next->m_prev = node->m_prev;

    node->m_next = node->m_prev = node;
}

template<typename T>
zp_bool zpIntrusiveList<T>::isAttached() const {
    return m_root.m_next != &m_root;
}

template<typename T>
const zpIntrusiveListNode<T>* zpIntrusiveList<T>::front() const {
    return m_root.m_next;
}
template<typename T>
const zpIntrusiveListNode<T>* zpIntrusiveList<T>::back() const {
    return m_root.m_prev;
}

template<typename T> template<typename Func>
void zpIntrusiveList<T>::foreach( Func func ) {
    const zpIntrusiveListNode<T>* ring = m_root.m_next;

    while( ring != &m_root ) {
        func( (T*)ring );
        ring = ring->m_next;
    }
}
template<typename T> template<typename Func>
void zpIntrusiveList<T>::foreach( Func func ) const {
    const zpIntrusiveListNode<T>* ring = m_root.m_next;

    while( ring != &m_root ) {
        func( (T*)ring );
        ring = ring->m_next;
    }
}

template<typename T> template<typename Func>
T* zpIntrusiveList<T>::findFirstIf( Func func ) const {
    const zpIntrusiveListNode<T>* ring = m_root.m_next;

    while( ring != &m_root ) {
        if( func( (const T*)ring ) ) {
            return (T*)ring;
        };
        ring = ring->m_next;
    }

    return ZP_NULL;
}
template<typename T> template<typename Func>
T* zpIntrusiveList<T>::findFirstIf( Func func ) {
    const zpIntrusiveListNode<T>* ring = m_root.m_next;

    while( ring != &m_root ) {
        T* val = (T*)ring;
        if( func( val ) ) {
            return val;
        };
        ring = ring->m_next;
    }

    return ZP_NULL;
}

template<typename T> template<typename Func>
const T* zpIntrusiveList<T>::findLastIf( Func func ) const {
    const zpIntrusiveListNode<T>* ring = m_root.m_prev;

    while( ring != &m_root ) {
        const T* val = (const T*)ring;
        if( func( val ) ) {
            return val;
        };
        ring = ring->m_prev;
    }

    return ZP_NULL;
}
template<typename T> template<typename Func>
T* zpIntrusiveList<T>::findLastIf( Func func ) {
    const zpIntrusiveListNode<T>* ring = m_root.m_prev;

    while( ring != &m_root ) {
        T* val = (T*)ring;
        if( func( val ) ) {
            return val;
        };
        ring = ring->m_prev;
    }

    return ZP_NULL;
}