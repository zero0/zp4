#pragma once
#ifndef ZP_INTRUSIVE_LIST_H
#define ZP_INTRUSIVE_LIST_H

template<typename T>
class zpIntrusiveList;

template<typename T>
struct ZP_NO_VTABLE zpIntrusiveListNode {
protected:
	zpIntrusiveListNode* m_next;
	zpIntrusiveListNode* m_prev;

	friend class zpIntrusiveList<T>;
};

template<typename T>
class zpIntrusiveList {
public:
	zpIntrusiveList();
	~zpIntrusiveList();

	void pushBack( zpIntrusiveListNode<T>* node );
	void pushFront( zpIntrusiveListNode<T>* node );
	void remove( zpIntrusiveListNode<T>* node );

	zp_bool isAttached() const;
	
	const zpIntrusiveListNode<T>* front() const;
	const zpIntrusiveListNode<T>* back() const;
	
	template<typename Func>
	void foreach( Func func );
	template<typename Func>
	void foreach( Func func ) const;

	template<typename Func>
	const T* findFirstIf( Func func ) const;
	template<typename Func>
	T* findFirstIf( Func func );

	template<typename Func>
	const T* findLastIf( Func func ) const;
	template<typename Func>
	T* findLastIf( Func func );
	
private:
	zpIntrusiveListNode<T> m_root;
};

#include "zpIntrusiveList.inl"

#endif