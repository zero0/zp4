
template<typename T>
zpRect<T>::zpRect()
{}

template<typename T>
zpRect<T>::zpRect( const zpVector2<T>& position, const zpVector2<T>& size )
	: m_position( position )
	, m_size( size )
{}

template<typename T>
zpRect<T>::zpRect( const zpRect& rect )
	: m_position( rect.m_position )
	, m_size( rect.m_size )
{}

template<typename T>
zpRect<T>::zpRect( zpRect&& rect )
	: m_position( zp_move( rect.m_position ) )
	, m_size( zp_move( rect.m_size ) )
{}

template<typename T>
zpRect<T>::~zpRect()
{}

template<typename T>
void zpRect<T>::operator=( const zpRect& rect )
{
	m_position = rect.m_position;
	m_size = rect.m_size;
}

template<typename T>
void zpRect<T>::operator=( zpRect&& rect )
{
	m_position = zp_move( rect.m_position );
	m_size = zp_move( rect.m_size );
}

template<typename T>
const zpVector2<T>& zpRect<T>::getPosition() const
{
	return m_position;
}

template<typename T>
const zpVector2<T>& zpRect<T>::getSize() const
{
	return m_size;
}

template<typename T>
void zpRect<T>::setPosition( const zpVector2<T>& position )
{
	m_position = position;
}

template<typename T>
void zpRect<T>::setSize( const zpVector2<T>& size )
{
	m_size = size;
}