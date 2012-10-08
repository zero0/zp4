#include "zpCore.h"

zpRect::zpRect() {}
zpRect::zpRect( const zpVector2f& position, const zpVector2f& size )
	: m_position( position )
	, m_size( size )
{}
zpRect::zpRect( const zpRect& rect )
	: m_position( rect.m_position )
	, m_size( rect.m_size )
{}
zpRect::zpRect( zpRect&& rect )
	: m_position( rect.m_position )
	, m_size( rect.m_size )
{}
zpRect::~zpRect() {}

void zpRect::operator=( const zpRect& rect ) {
	m_position = rect.m_position;
	m_size = rect.m_size;
}
void zpRect::operator=( zpRect&& rect ) {
	m_position = (zpVector2f&&)rect.m_position;
	m_size = (zpVector2f&&)rect.m_size;
}

const zpVector2f& zpRect::getPosition() const {
	return m_position;
}
const zpVector2f& zpRect::getSize() const {
	return m_size;
}

void zpRect::setPosition( const zpVector2f& position ) {
	m_position = position;
}
void zpRect::setSize( const zpVector2f& size ) {
	m_size = size;
}
