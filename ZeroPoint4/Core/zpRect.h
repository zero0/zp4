#pragma once
#ifndef ZP_RECT_H
#define ZP_RECT_H

class zpRect {
public:
	zpRect();
	zpRect( const zpVector2f& position, const zpVector2f& size );
	zpRect( const zpRect& rect );
	zpRect( zpRect&& rect );
	~zpRect();

	void operator=( const zpRect& rect );
	void operator=( zpRect&& rect );

	const zpVector2f& getPosition() const;
	const zpVector2f& getSize() const;

	void setPosition( const zpVector2f& position );
	void setSize( const zpVector2f& size );

private:
	zpVector2f m_position;
	zpVector2f m_size;
};

#endif