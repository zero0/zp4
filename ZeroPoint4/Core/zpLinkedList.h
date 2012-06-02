#pragma once
#ifndef ZP_LINKED_LIST_H
#define ZP_LINKED_LIST_H

template<typename T>
struct zpLinkedListNode {
	zpLinkedListNode() : m_value(), m_next( ZP_NULL ), m_prev( ZP_NULL ) {};

	T m_value;
	zpLinkedListNode* m_next;
	zpLinkedListNode* m_prev;
};

template<typename T>
class zpLinkedList {
public:
	zpLinkedList();
	~zpLinkedList();

	void pushBack( const T& val );
	void pushFront( const T& val );
	
	void popBack( zp_uint count = 1 );
	void popFront( zp_uint count = 1 );

	void pushBackAll( const zpLinkedList<T>& list );
	void pushFrontAll( const zpLinkedList<T>& list );

	zp_bool contains( const T& val ) const;

	zp_bool removeFirst( const T& val );
	zp_bool removeLast( const T& val );
	zp_uint removeAll( const T& val );

	void clear();

	const T& front() const;
	const T& back() const;

	T& front();
	T& back();

	zp_bool isEmpty() const;
	zp_uint getSize() const;
	
	template<typename Func>
	void foreach( Func func ) const;
	template<typename Func>
	void foreachIndexed( Func func ) const;

	template<typename Func>
	zp_uint removeIf( Func func );

private:
	void addNode( const T& val, zpLinkedListNode<T>* next, zpLinkedListNode<T>* prev );
	void removeNode( zpLinkedListNode<T>* node, zp_bool andDeallocNode = true );

	zpLinkedListNode<T> m_root;
	zp_uint m_size;
};

#include "zpLinkedList.inl"

#endif