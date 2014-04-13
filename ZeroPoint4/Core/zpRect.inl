
template<typename T>
zpRect<T>::zpRect()
{}

template<typename T>
zpRect<T>::zpRect( const T& x, const T& y, const T& w, const T& h )
	: m_position( x, y )
	, m_size( w, h )
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

template<typename T>
T zpRect<T>::getLeft() const
{
	return m_position.getX();
}
template<typename T>
T zpRect<T>::getRight() const
{
	return m_position.getX() + m_size.getX();
}
template<typename T>
T zpRect<T>::getTop() const
{
	return m_position.getY();
}
template<typename T>
T zpRect<T>::getBottom() const
{
	return m_position.getY() + m_size.getY();
}

template<typename T>
zp_bool zpRect<T>::contains( const zpVector2<T>& pos ) const
{
	zp_bool inside = !(
		pos.getX() < getLeft() ||
		pos.getX() > getRight() ||
		pos.getY() < getTop() ||
		pos.getY() > getBottom()
		);
	return inside;
}

template<typename T>
zpVector2<T> zpRect<T>::getTopLeft() const
{
	return zpVector2<T>( getLeft(), getTop() );
}
template<typename T>
zpVector2<T> zpRect<T>::getTopRight() const
{
	return zpVector2<T>( getRight(), getTop() );
}
template<typename T>
zpVector2<T> zpRect<T>::getBottomLeft() const
{
	return zpVector2<T>( getLeft(), getBottom() );
}
template<typename T>
zpVector2<T> zpRect<T>::getBottomRight() const
{
	return zpVector2<T>( getRight(), getBottom() );
}
