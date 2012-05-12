#pragma once
#ifndef ZP_LINK_H
#define ZP_LINK_H

template<typename T>
struct __declspec(novtable) zpLinkNode {
	T* m_parent;
	zpLinkNode* m_next;
	zpLinkNode* m_prev;
};

template<typename T>
class zpLink {
public:
	zpLink( T* parent = ZP_NULL ) {
		m_root.m_next = &m_root;
		m_root.m_prev = &m_root;
		m_root.m_parent = parent;
	}
	~zpLink() {}

	void prepend( zpLinkNode<T>* node ) {
		if( !node || node->m_next == node ) return;

		node->m_next = &m_root;
		node->m_prev = m_root.m_prev;
		node->m_next->m_prev = node;
		node->m_prev->m_next = node;
	}
	void append( zpLinkNode<T>* node ) {
		if( !node || node->m_next == node ) return;

		node->m_next = m_root.m_next;
		node->m_prev = &m_root;
		node->m_next->m_prev = node;
		node->m_prev->m_next = node;
	}
	void detatch( zpLinkNode<T>* node ) {
		node->m_prev->m_next = node->m_next;
		node->m_next->m_prev = node->m_prev;

		node->m_next = node->m_prev = node;
	}

	zp_bool isAttached() const {
		return m_root.m_next != &m_root;
	}

	void setParent( T* parent ) {
		m_root.m_parent = parent;
	}

	zpLinkNode<T>* getNext() const { return m_root.m_next; }
	zpLinkNode<T>* getPrev() const { return m_root.m_prev; }
	T* getParent() const { return m_root.m_parent; }
	
	template<typename Func>
	Func foreach( Func func ) const {
		const zpLinkNode<T>* ring = m_root.m_next;

		while( ring != &m_root ) {
			func( (T*)ring );
			ring = ring->m_next;
		}

		return func;
	}
	
private:
	zpLinkNode<T> m_root;
};
#endif