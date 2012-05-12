#pragma once
#ifndef ZP_INTRUSIVE_LIST_H
#define ZP_INTRUSIVE_LIST_H

template<typename T>
struct __declspec(novtable) zpIntrusiveListNode {
	T* m_parent;
	zpIntrusiveListNode* m_next;
	zpIntrusiveListNode* m_prev;
};

template<typename T>
class zpIntrusiveList {
public:
	zpIntrusiveList( T* parent = ZP_NULL );
	~zpIntrusiveList();

	void prepend( zpIntrusiveListNode<T>* node );
	void append( zpIntrusiveListNode<T>* node );
	void detatch( zpIntrusiveListNode<T>* node );

	zp_bool isAttached() const;

	void setParent( T* parent );

	zpIntrusiveListNode<T>* getNext() const;
	zpIntrusiveListNode<T>* getPrev() const;
	T* getParent() const;
	
	template<typename Func>
	void foreach( Func func ) const;
	
private:
	zpIntrusiveListNode<T> m_root;
};

#include "zpIntrusiveList.inl"

#endif