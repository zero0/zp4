#pragma once
#ifndef ZP_RECT_H
#define ZP_RECT_H

template<typename T>
class zpRect {
public:
	zpRect();
	zpRect( const T& x, const T& y, const T& w, const T& h );
	zpRect( const zpVector2<T>& position, const zpVector2<T>& size );
	zpRect( const zpRect& rect );
	zpRect( zpRect&& rect );
	~zpRect();

	void operator=( const zpRect& rect );
	void operator=( zpRect&& rect );

	const zpVector2<T>& getPosition() const;
	const zpVector2<T>& getSize() const;

	void setPosition( const zpVector2<T>& position );
	void setSize( const zpVector2<T>& size );

	T getLeft() const;
	T getRight() const;
	T getTop() const;
	T getBottom() const;

	zp_bool contains( const zpVector2<T>& pos ) const;
	zp_bool contains( const zpRect<T>& rect ) const;

	zpVector2<T> getTopLeft() const;
	zpVector2<T> getTopRight() const;
	zpVector2<T> getBottomLeft() const;
	zpVector2<T> getBottomRight() const;

private:
	zpVector2<T> m_position;
	zpVector2<T> m_size;
};

#include "zpRect.inl"

typedef zpRect<zp_int> zpRecti;
typedef zpRect<zp_float> zpRectf;

#endif