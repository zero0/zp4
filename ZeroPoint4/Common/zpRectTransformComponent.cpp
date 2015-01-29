#include "zpCommon.h"

const zpVector2f zpRectTransformComponent::s_rectPivots[] =
{
	zpVector2f( 0.0f, 1.0f ),
	zpVector2f( 0.5f, 1.0f ),
	zpVector2f( 1.0f, 1.0f ),

	zpVector2f( 0.0f, 0.5f ),
	zpVector2f( 0.5f, 0.5f ),
	zpVector2f( 1.0f, 0.5f ),

	zpVector2f( 0.0f, 0.0f ),
	zpVector2f( 0.5f, 0.0f ),
	zpVector2f( 1.0f, 0.0f ),
};

zpRectTransformComponent::zpRectTransformComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_frameUpdate( -1 )
	, m_pivot( ZP_RECT_PIVOT_TOP_LEFT )
	, m_keepAspect( ZP_RECT_LOCK_ASPECT_RATIO_FREE )
	, m_offset( 0, 0 )
	, m_width( 100 )
	, m_height( 100 )
	, m_minWidth( 2 )
	, m_minHeight( 2 )
	, m_leftAnchor( 0 )
	, m_bottomAnchor( 0 )
	, m_rightAnchor( 1 )
	, m_topAnchor( 1 )
{

}
zpRectTransformComponent::~zpRectTransformComponent()
{

}

void zpRectTransformComponent::setPivot( zpRectPivot pivot )
{
	if( m_pivot != pivot )
	{
		m_pivot = pivot;
	}
}
zpRectPivot zpRectTransformComponent::getPivot() const
{
	return m_pivot;
}

void zpRectTransformComponent::setOffset( const zpVector2f& offset )
{
	m_offset = offset;
}
const zpVector2f& zpRectTransformComponent::getOffset() const
{
	return m_offset;
}

zp_int zpRectTransformComponent::getWidth() const
{
	return m_width;
}
void zpRectTransformComponent::setWidth( zp_int width )
{
	width = ZP_MAX( width, m_minWidth );

	if( m_width != width )
	{
		m_width = width;

		if( m_keepAspect == ZP_RECT_LOCK_ASPECT_RATIO_BASED_ON_WIDTH )
		{
			m_height = zp_floor_to_int( m_width / m_aspectRatio );
		}

		m_aspectRatio = (zp_float)m_width / (zp_float)m_height;
	}
}

zp_int zpRectTransformComponent::getHeight() const
{
	return m_height;
}
void zpRectTransformComponent::setHeight( zp_int height )
{
	height = ZP_MAX( height, m_minHeight );

	if( m_height != height )
	{
		m_height = height;

		if( m_keepAspect == ZP_RECT_LOCK_ASPECT_RATIO_BASED_ON_HEIGHT )
		{
			m_width = zp_floor_to_int( m_height * m_aspectRatio );
		}

		m_aspectRatio = (zp_float)m_width / (zp_float)m_height;
	}
}

zp_bool zpRectTransformComponent::isAnchored() const
{
	return m_leftAnchor.rect != ZP_NULL || m_bottomAnchor.rect != ZP_NULL || m_rightAnchor.rect != ZP_NULL || m_topAnchor.rect != ZP_NULL;
}
void zpRectTransformComponent::updateAnchors()
{
	if( isAnchored() ) onUpdateAnchors();
}


void zpRectTransformComponent::setAnchor( zpRectTransformComponent* rect )
{
	m_leftAnchor.rect = rect;
	m_bottomAnchor.rect = rect;
	m_rightAnchor.rect = rect;
	m_topAnchor.rect = rect;
}
void zpRectTransformComponent::setAnchor( zpRectTransformComponent* rect, zp_int left, zp_int bottom, zp_int right, zp_int top )
{
	m_leftAnchor.rect = rect;
	m_bottomAnchor.rect = rect;
	m_rightAnchor.rect = rect;
	m_topAnchor.rect = rect;

	m_leftAnchor.absolute = left;
	m_bottomAnchor.absolute = bottom;
	m_rightAnchor.absolute = right;
	m_topAnchor.absolute = top;

	m_leftAnchor.relative = 0;
	m_bottomAnchor.relative = 0;
	m_rightAnchor.relative = 1;
	m_topAnchor.relative = 1;
}

zp_float zpRectTransformComponent::getAspectRatio() const
{
	return m_aspectRatio;
}

void zpRectTransformComponent::getLocalCorners( zpVector4f* corners ) const
{
	const zpVector2f& offset = s_rectPivots[ m_pivot ];

	zp_float x0 = -offset.getX() * m_width;
	zp_float y0 = -offset.getY() * m_height;
	zp_float x1 = x0 + m_width;
	zp_float y1 = y0 + m_height;

	corners[ 0 ] = zpVector4f( x0, y0, 0, 1 );
	corners[ 1 ] = zpVector4f( x0, y1, 0, 1 );
	corners[ 2 ] = zpVector4f( x1, y1, 0, 1 );
	corners[ 3 ] = zpVector4f( x1, y0, 0, 1 );
}
void zpRectTransformComponent::getWorldCorners( zpVector4f* corners ) const
{
	getLocalCorners( corners );

	zpTransformComponent* trans = getParentObject()->getComponents()->getTransformComponent();

	const zpMatrix4f& mat = trans->getWorldTransform();
	zpMath::Mul( corners[ 0 ], corners[ 0 ], mat );
	zpMath::Mul( corners[ 1 ], corners[ 1 ], mat );
	zpMath::Mul( corners[ 2 ], corners[ 2 ], mat );
	zpMath::Mul( corners[ 3 ], corners[ 3 ], mat );
}

zpVector4f zpRectTransformComponent::getRegion() const
{
	const zpVector2f& offset = s_rectPivots[ m_pivot ];

	zp_float x0 = -offset.getX() * m_width;
	zp_float y0 = -offset.getY() * m_height;
	zp_float x1 = x0 + m_width;
	zp_float y1 = y0 + m_height;

	return zpVector4f( x0, y0, x1, y1 );
}

void zpRectTransformComponent::onCreate()
{

}
void zpRectTransformComponent::onInitialize()
{

}
void zpRectTransformComponent::onDestroy()
{

}

void zpRectTransformComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	zp_uint frame = getApplication()->getFrameCount();
	if( frame != m_frameUpdate )
	{
		m_frameUpdate = frame;

		zp_bool anchored = false;

		if( m_leftAnchor.rect != ZP_NULL )
		{
			anchored = true;
			m_leftAnchor.rect->update( deltaTime, realTime );
		}

		if( m_bottomAnchor.rect != ZP_NULL )
		{
			anchored = true;
			m_bottomAnchor.rect->update( deltaTime, realTime );
		}

		if( m_rightAnchor.rect != ZP_NULL )
		{
			anchored = true;
			m_rightAnchor.rect->update( deltaTime, realTime );
		}

		if( m_topAnchor.rect != ZP_NULL )
		{
			anchored = true;
			m_topAnchor.rect->update( deltaTime, realTime );
		}

		if( anchored )
		{
			onUpdateAnchors();
		}
	}
}
void zpRectTransformComponent::onSimulate()
{

}

void zpRectTransformComponent::onEnabled()
{

}
void zpRectTransformComponent::onDisabled()
{

}

void zpRectTransformComponent::onUpdateAnchors()
{
	zpTransformComponent* trans = getParentObject()->getComponents()->getTransformComponent();
	zpTransformComponent* parent = trans->getParent();

	zp_float lt, bt, rt, tt;
	zpVector4f pos = trans->getLocalPosition();
	zpVector4f sides[4];

	const zpVector2f& pvt = s_rectPivots[ m_pivot ];

	if( m_leftAnchor.rect == m_rightAnchor.rect &&
		m_leftAnchor.rect == m_topAnchor.rect &&
		m_leftAnchor.rect == m_bottomAnchor.rect )
	{
		if( m_leftAnchor.rect == ZP_NULL )
		{
			lt = pos.getX().getFloat() + m_leftAnchor.absolute;
			bt = pos.getY().getFloat() + m_bottomAnchor.absolute;
			rt = pos.getX().getFloat() + m_rightAnchor.absolute;
			tt = pos.getY().getFloat() + m_topAnchor.absolute;
		}
		else
		{
			getSides( parent, sides );

			zp_lerp< zp_float >( lt, sides[0].getX().getFloat(), sides[2].getX().getFloat(), m_leftAnchor.relative   );
			zp_lerp< zp_float >( rt, sides[0].getX().getFloat(), sides[2].getX().getFloat(), m_rightAnchor.relative  );
			zp_lerp< zp_float >( bt, sides[3].getY().getFloat(), sides[1].getY().getFloat(), m_bottomAnchor.relative );
			zp_lerp< zp_float >( tt, sides[3].getY().getFloat(), sides[1].getY().getFloat(), m_topAnchor.relative    );

			lt += m_leftAnchor.absolute;
			rt += m_rightAnchor.absolute;
			bt += m_bottomAnchor.absolute;
			tt += m_topAnchor.absolute;
		}
	}
	else
	{

	}

	zp_float nx, ny;
	zp_lerp( nx, lt, rt, pvt.getX() );
	zp_lerp( ny, bt, tt, pvt.getY() );

	zpVector4f newPos = zpVector4f( nx, ny, pos.getZ().getFloat(), 1 );
	zp_int w = zp_floor_to_int( rt - lt + 0.5f );
	zp_int h = zp_floor_to_int( tt - bt + 0.5f );

	w = ZP_MAX( w, m_minWidth );
	h = ZP_MAX( h, m_minHeight );

	zpScalar d;
	zpVector4f diff;
	zpMath::Sub( diff, pos, newPos );
	zpMath::LengthSquared3( d, diff );
	if( zpMath::Cmp( d, zpScalar( 0.001f ) ) > 0 )
	{
		trans->setLocalPosition( newPos );
	}

	if( w != m_width || h != m_height )
	{
		m_width = w;
		m_height = h;
	}
}

void zpRectTransformComponent::getSides( zpTransformComponent* relativeTo, zpVector4f* sides ) const
{
	zpTransformComponent* trans = getParentObject()->getComponents()->getTransformComponent();

	const zpVector2f& offset = s_rectPivots[ m_pivot ];

	zp_float x0 = -offset.getX() * m_width;
	zp_float y0 = -offset.getY() * m_height;
	zp_float x1 = x0 + m_width;
	zp_float y1 = y0 + m_height;
	zp_float cx = ( x0 + x1 ) * 0.5f;
	zp_float cy = ( y0 + y1 ) * 0.5f;

	const zpMatrix4f& mat = trans->getWorldTransform();
	zpMath::Mul( sides[ 0 ], zpVector4f( x0, cy, 0, 1 ), mat );
	zpMath::Mul( sides[ 1 ], zpVector4f( cx, y1, 0, 1 ), mat );
	zpMath::Mul( sides[ 2 ], zpVector4f( x1, cy, 0, 1 ), mat );
	zpMath::Mul( sides[ 3 ], zpVector4f( cx, y0, 0, 1 ), mat );

	//if( relativeTo != ZP_NULL )
	//{
	//	for (int i = 0; i < 4; ++i)
	//		mCorners[i] = relativeTo.InverseTransformPoint(mCorners[i]);
	//}
}



zpRectTransformComponentPool::zpRectTransformComponentPool()
{}
zpRectTransformComponentPool::~zpRectTransformComponentPool()
{}

void zpRectTransformComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpRectTransformComponent* o ) {
		o->update( deltaTime, realTime );
	} );
}
