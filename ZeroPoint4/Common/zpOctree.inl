
template<typename Func>
void zpOctreeNode::query( const zpBoundingAABB& bounds, Func queryFunc ) const
{
	if( ZP_IS_COLLISION( bounds, m_bounds ) )
	{
		queryFunc( this, m_children );

		if( !m_children.isEmpty() )
		{
			zpOctreeNode** b = m_children.begin();
			zpOctreeNode** e = m_children.end();
			for( ; b != e; ++b )
			{
				b->query( bounds, queryFunc );
			}
		}
	}
}

template<typename Func>
void zpOctreeNode::foreach( Func foreachFunc ) const
{
	foreachFunc( this, m_children );

	if( !m_children.isEmpty() )
	{
		zpOctreeNode** b = m_children.begin();
		zpOctreeNode** e = m_children.end();
		for( ; b != e; ++b )
		{
			b->query( bounds, queryFunc );
		}
	}
}


template<typename Func>
void zpOctree::query( const zpBoundingAABB& bounds, Func queryFunc ) const
{
	m_root.query( bounds, queryFunc );
}

template<typename Func>
void zpOctree::foreach( Func foreachFunc ) const
{
	m_root.foreach( foreachFunc );
}
