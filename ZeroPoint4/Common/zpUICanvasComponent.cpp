#include "zpCommon.h"

static const zpVector2f s_rectPivots[] =
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

static void GetWidgetDrawingDimensions( zpUIWidget* widget, zp_float* dim )
{
	const zpVector2f& offset = s_rectPivots[ widget->pivot ];

	zp_float x0 = -offset.getX() * widget->width;
	zp_float y0 = -offset.getY() * widget->height;
	zp_float x1 = x0 + widget->width;
	zp_float y1 = y0 + widget->height;

	ZP_ALIGN16 zp_float b[4];
	zpMath::Vector4Store4( widget->border, b );

	ZP_ALIGN16 zp_float dr[4];
	zpMath::Vector4Store4( widget->drawRegion, dr );

	zp_float fw = b[0] + b[2];
	zp_float fh = b[1] + b[3];

	dim[0] = zp_lerp( x0, x1 - fw, dr[0] );
	dim[1] = zp_lerp( y0, y1 - fh, dr[1] );
	dim[2] = zp_lerp( x0 + fw, x1, dr[2] );
	dim[3] = zp_lerp( y0 + fh, y1, dr[3] );
}

static void BasicSpriteFill( zpUIWidget* widget, zpUIDrawCall* drawCall )
{
	const zpVector2f& offset = s_rectPivots[ widget->pivot ];

	zp_float v[4];
	GetWidgetDrawingDimensions( widget, v );

	zpVector4f c0 = zpMath::Vector4( v[0], v[1], 0, 0 );
	zpVector4f c1 = zpMath::Vector4( v[0], v[3], 0, 0 );
	zpVector4f c2 = zpMath::Vector4( v[2], v[3], 0, 0 );
	zpVector4f c3 = zpMath::Vector4( v[2], v[1], 0, 0 );
	
	zpVector4f worldPosition = widget->worldTransform.m_m4;
	c0 = zpMath::Vector4Add( worldPosition, c0 );
	c1 = zpMath::Vector4Add( worldPosition, c1 );
	c2 = zpMath::Vector4Add( worldPosition, c2 );
	c3 = zpMath::Vector4Add( worldPosition, c3 );

	drawCall->pos.pushBack( c0 );
	drawCall->pos.pushBack( c1 );
	drawCall->pos.pushBack( c2 );
	drawCall->pos.pushBack( c3 );

	zpRectf uv( 0, 0, 1, 1 );
	drawCall->uvs.pushBack( uv.getTopLeft() );
	drawCall->uvs.pushBack( uv.getTopRight() );
	drawCall->uvs.pushBack( uv.getBottomRight() );
	drawCall->uvs.pushBack( uv.getBottomLeft() );
	
	drawCall->cols.pushBack( widget->colorTopLeft );
	drawCall->cols.pushBack( widget->colorTopRight );
	drawCall->cols.pushBack( widget->colorBottomRight );
	drawCall->cols.pushBack( widget->colorBottomLeft );
}

zpUIWidget::zpUIWidget()
	: id( ZP_UI_WIDGET_INVALID_ID )
	, name()
	, tag()
	, isVisible( true )
	, isEnabled( true )
	, frameUpdated( 0 )
	, aspectRatio( 1.f )
	, aspectRatioLock( ZP_UI_WIDGET_LOCK_ASPECT_RATIO_FREE )
	, pivot( ZP_UI_WIDGET_PIVOT_TOP_LEFT )
	, transform( zpMath::MatrixIdentity() )
	, worldTransform( zpMath::MatrixIdentity() )
	, border( zpMath::Vector4( 0, 0, 0, 0 ) )
	, drawRegion( zpMath::Vector4( 0, 0, 1, 1 ) )
	, offset( 0, 0 )
	, width( 100 )
	, height( 100 )
	, minWidth( 2 )
	, minHeight( 2 )
	, depth( 0 )
	, alpha( 1.f )
	, colorBottomLeft( 1, 1, 1, 1 )
	, colorTopLeft( 1, 1, 1, 1 )
	, colorTopRight( 1, 1, 1, 1 )
	, colorBottomRight( 1, 1, 1, 1 )
	, parent( ZP_UI_WIDGET_INVALID_ID )
	, fillRenderer( ZP_NULL )
{
	zp_zero_memory_array( listeners );
}
zpUIWidget::~zpUIWidget()
{
	zp_zero_memory_array( listeners );
}

zpUIWidgetAnchor::zpUIWidgetAnchor()
	: widget( ZP_UI_WIDGET_INVALID_ID )
	, relative( 0 )
	, absolute( 0 )
{
}
zpUIWidgetAnchor::~zpUIWidgetAnchor()
{
}

zpUICanvasComponent::zpUICanvasComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_layer( 0 )
	, m_ids( ZP_UI_WIDGET_ROOT_ID )
{
	const zpVector2i& screenSize = getApplication()->getWindow()->getScreenSize();

	// create root object
	zpUIWidget* widget = &m_widgets.pushBackEmpty();
	widget->id = m_ids++;
	widget->parent = -1;
	widget->width = screenSize.getX();
	widget->height = screenSize.getY();
	widget->aspectRatioLock = ZP_UI_WIDGET_LOCK_ASPECT_RATIO_BASED_ON_HEIGHT;
	widget->aspectRatio = (zp_float)widget->width / (zp_float)widget->height;
	widget->pivot = ZP_UI_WIDGET_PIVOT_CENTER;

	// create rest of the objects
	createUIFromData( def[ "UI" ] );
}
zpUICanvasComponent::~zpUICanvasComponent()
{

}

void zpUICanvasComponent::render( zpRenderingContext* context )
{
	const zpUIDrawCall* b = m_drawCalls.begin();
	const zpUIDrawCall* e = m_drawCalls.end();
	for( ; b != e; ++b )
	{
		context->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, &b->material );

		for( zp_uint i = 0, imax = b->pos.size(); i < imax; i += 4 )
		{
			context->addQuad(
				b->pos[ i + 0 ], b->uvs[ i + 0 ], b->cols[ i + 0 ],
				b->pos[ i + 1 ], b->uvs[ i + 1 ], b->cols[ i + 1 ],
				b->pos[ i + 2 ], b->uvs[ i + 2 ], b->cols[ i + 2 ],
				b->pos[ i + 3 ], b->uvs[ i + 3 ], b->cols[ i + 3 ]
				);
		}

		context->endDrawImmediate();
	}
}

zp_int zpUICanvasComponent::addWidget( zp_int parent )
{
	zpUIWidget* widget = &m_widgets.pushBackEmpty();
	widget->id = m_ids++;

	addWidgetChild( parent, widget->id );

	return widget->id;
}

void zpUICanvasComponent::removeWidget( zp_int w )
{
	zpUIWidget* widget = getWidget( w );

	widget->parent = ZP_UI_WIDGET_INVALID_ID;
	m_widgets.eraseIf( [ &w ]( zpUIWidget& widget ) { return widget.id == w; } );

	widget->children.foreach( [ this ]( zp_int child ) {
		removeWidget( child );
	} );
}
void zpUICanvasComponent::removeAllWidgets()
{
	removeWidget( 0 );
}

void zpUICanvasComponent::addWidgetChild( zp_int parent, zp_int child )
{
	zpUIWidget* parentWidget = getWidget( parent );
	zpUIWidget* childWidget = getWidget( child );

	if( parentWidget->children.indexOf( child ) < 0 )
	{
		removeWidgetChild( childWidget->parent, child );

		childWidget->parent = parent;
		childWidget->depth = parentWidget->children.size();
		parentWidget->children.pushBack( child );
	}
}
void zpUICanvasComponent::removeWidgetChild( zp_int parent, zp_int child )
{
	zpUIWidget* parentWidget = getWidget( parent );
	zpUIWidget* childWidget = getWidget( child );

	zp_int index = parentWidget->children.indexOf( child );
	if( index >= 0 )
	{
		childWidget->parent = 0;
		parentWidget->children.erase( index );
	}
}

const zpString& zpUICanvasComponent::getWidgetName( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	return widget->name;
}
void zpUICanvasComponent::setWidgetName( zp_int w, const zp_char* name )
{
	zpUIWidget* widget = getWidget( w );
	widget->name = name;
}

const zpVector4f& zpUICanvasComponent::getWidgetLocalPosition( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	return widget->transform.m_m4;
}
void zpUICanvasComponent::setWidgetLocalPosition( zp_int w, zpVector4fParamF position )
{
	zpUIWidget* widget = getWidget( w );
	widget->transform.m_m4 = position;
}

zpVector2i zpUICanvasComponent::getWidgetSize( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );

	return zpVector2i( widget->width, widget->height );
}
zp_int zpUICanvasComponent::getWidgetWidth( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	return widget->width;
}
zp_int zpUICanvasComponent::getWidgetHeight( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	return widget->height;
}
void zpUICanvasComponent::setWidgetWidth( zp_int w, zp_int width )
{
	zpUIWidget* widget = getWidget( w );

	width = ZP_MAX( widget->minWidth, width );

	if( widget->width != width )
	{
		widget->width = width;

		if( widget->aspectRatioLock == ZP_UI_WIDGET_LOCK_ASPECT_RATIO_BASED_ON_WIDTH )
		{
			widget->height = zp_floor_to_int( (zp_float)widget->width / widget->aspectRatio );
		}

		widget->aspectRatio = (zp_float)widget->width / (zp_float)widget->height;
	}
}
void zpUICanvasComponent::setWidgetHeight( zp_int w, zp_int height )
{
	zpUIWidget* widget = getWidget( w );

	height = ZP_MAX( widget->minHeight, height );

	if( widget->height != height )
	{
		widget->height = height;

		if( widget->aspectRatioLock == ZP_UI_WIDGET_LOCK_ASPECT_RATIO_BASED_ON_HEIGHT )
		{
			widget->width = zp_floor_to_int( (zp_float)widget->height / widget->aspectRatio );
		}

		widget->aspectRatio = (zp_float)widget->width / (zp_float)widget->height;
	}
}

void zpUICanvasComponent::setWidgetColor( zp_int w, const zpColor4f& color )
{
	zpUIWidget* widget = getWidget( w );
	widget->colorBottomLeft = color;
	widget->colorTopLeft = color;
	widget->colorTopRight = color;
	widget->colorBottomRight = color;
}

zp_float zpUICanvasComponent::getWidgetAlpha( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	zp_float a = widget->alpha;

	while( widget->parent != ZP_UI_WIDGET_INVALID_ID )
	{
		widget = getWidget( widget->parent );
		a *= widget->alpha;
	}

	return a;
}
void zpUICanvasComponent::setWidgetAlpha( zp_int w, zp_float a )
{
	zpUIWidget* widget = getWidget( w );
	widget->alpha = a;
}

void zpUICanvasComponent::setWidgetMaterial( zp_int w, const zpMaterialResourceInstance& material )
{
	zpUIWidget* widget = getWidget( w );
	widget->material = material;
}

void zpUICanvasComponent::setWidgetFill( zp_int w, zpUIWIdgetFillRender fill )
{
	zpUIWidget* widget = getWidget( w );
	widget->fillRenderer = fill;
}
void zpUICanvasComponent::setWidgetListener( zp_int w, zpUIWidgetEvent e, zpUIWidgetEventListener listener )
{
	zpUIWidget* widget = getWidget( w );
	widget->listeners[ e ] = listener;
}

void zpUICanvasComponent::sendWidgetEvent( zp_int w, zpUIWidgetEvent e, void* data )
{
	zpUIWidget* widget = getWidget( w );
	zpUIWidgetEventListener l = widget->listeners[ e ];
	if( l != ZP_NULL ) l( widget, e, data );

	widget->children.foreach( [ this, w, e, data ]( zp_int child ) { sendWidgetEvent( child, e, data ); } );
}

void zpUICanvasComponent::setWidgetVisible( zp_uint w, zp_bool visible )
{
	zpUIWidget* widget = getWidget( w );
	if( widget->isVisible != visible )
	{
		zp_bool wasVisible = widget->isVisible;
		widget->isVisible = visible;
	}
}
zp_bool zpUICanvasComponent::isWidgetVisible( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	return widget->isVisible;
}
zp_bool zpUICanvasComponent::isWidgetVisibleInHeirachy( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	zp_bool visible = widget->isVisible;

	while( visible && widget->parent != ZP_UI_WIDGET_INVALID_ID )
	{
		widget = getWidget( widget->parent );
		visible = widget->isVisible;
	}

	return visible;
}

void zpUICanvasComponent::setWidgetEnabled( zp_uint w, zp_bool enabled )
{
	zpUIWidget* widget = getWidget( w );
	widget->isEnabled = enabled;
}
zp_bool zpUICanvasComponent::isWidgetEnabled( zp_int w ) const
{
	const zpUIWidget* widget = getWidget( w );
	return widget->isEnabled;
}

void zpUICanvasComponent::setWidgetAlignmentUniform( zp_int w, zp_int anchor, zp_int left, zp_int bottom, zp_int right, zp_int top )
{
	zpUIWidget* widget = getWidget( w );
	zpUIWidgetAnchor* l = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_LEFT;
	zpUIWidgetAnchor* b = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_BOTTOM;
	zpUIWidgetAnchor* r = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_RIGHT;
	zpUIWidgetAnchor* t = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_TOP;

	l->widget = w;
	r->widget = w;
	t->widget = w;
	b->widget = w;

	l->absolute = left;
	r->absolute = right;
	t->absolute = top;
	b->absolute = bottom;
	
	l->relative = 0.f;
	r->relative = 1.f;
	t->relative = 0.f;
	b->relative = 1.f;
}
void zpUICanvasComponent::setWidgetAlignmentLeft( zp_int w, zp_int anchor, zp_float relative, zp_int absolute )
{
	zpUIWidget* widget = getWidget( w );
	zpUIWidgetAnchor* a = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_LEFT;

	a->widget = anchor;
	a->relative = relative;
	a->absolute = absolute;
}
void zpUICanvasComponent::setWidgetAlignmentRight( zp_int w, zp_int anchor, zp_float relative, zp_int absolute )
{
	zpUIWidget* widget = getWidget( w );
	zpUIWidgetAnchor* a = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_RIGHT;

	a->widget = anchor;
	a->relative = relative;
	a->absolute = absolute;
}
void zpUICanvasComponent::setWidgetAlignmentBottom( zp_int w, zp_int anchor, zp_float relative, zp_int absolute )
{
	zpUIWidget* widget = getWidget( w );
	zpUIWidgetAnchor* a = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_BOTTOM;

	a->widget = anchor;
	a->relative = relative;
	a->absolute = absolute;
}
void zpUICanvasComponent::setWidgetAlignmentTop( zp_int w, zp_int anchor, zp_float relative, zp_int absolute )
{
	zpUIWidget* widget = getWidget( w );
	zpUIWidgetAnchor* a = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_TOP;

	a->widget = anchor;
	a->relative = relative;
	a->absolute = absolute;
}


void zpUICanvasComponent::onCreate()
{
}
void zpUICanvasComponent::onInitialize()
{
}
void zpUICanvasComponent::onDestroy()
{
	removeAllWidgets();
}

void zpUICanvasComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	zp_uint frameCount = getApplication()->getFrameCount();
	updateWidget( ZP_UI_WIDGET_ROOT_ID, frameCount, deltaTime, realTime );

	sortWidget( ZP_UI_WIDGET_ROOT_ID );

	clearDrawCalls();

	fillWidget( ZP_UI_WIDGET_ROOT_ID );
}
void zpUICanvasComponent::onSimulate()
{

}

void zpUICanvasComponent::onEnabled()
{
	setWidgetVisible( ZP_UI_WIDGET_ROOT_ID, true );
}
void zpUICanvasComponent::onDisabled()
{
	setWidgetVisible( ZP_UI_WIDGET_ROOT_ID, false );
}

zpUIWidget* zpUICanvasComponent::getWidget( zp_int w )
{
	zpUIWidget* widgetPtr = ZP_NULL;
	m_widgets.findIf( [ &w ]( zpUIWidget& widget ) { return widget.id == w; }, &widgetPtr );
	return widgetPtr;
}
const zpUIWidget* zpUICanvasComponent::getWidget( zp_int w ) const
{
	const zpUIWidget* widgetPtr = ZP_NULL;
	m_widgets.findIf( [ &w ]( zpUIWidget& widget ) { return widget.id == w; }, &widgetPtr );
	return widgetPtr;
}

void zpUICanvasComponent::updateWidget( zp_int w, zp_uint frameCount, zp_float deltaTime, zp_float realTime )
{
	zpUIWidget* widget = getWidget( w );
	if( widget->frameUpdated != frameCount )
	{
		widget->frameUpdated = frameCount;

		if( widget->parent != ZP_UI_WIDGET_INVALID_ID )
		{
			zpUIWidget* parent = getWidget( widget->parent );
			widget->worldTransform = zpMath::MatrixMul( parent->worldTransform, widget->transform );
		}
		
		zp_bool isAnchored = false;
		zp_int anchor;

		anchor = widget->anchors[0].widget;
		if( anchor != ZP_UI_WIDGET_INVALID_ID )
		{
			isAnchored = true;
			updateWidget( anchor, frameCount, deltaTime, realTime );
		}

		anchor = widget->anchors[1].widget;
		if( anchor != ZP_UI_WIDGET_INVALID_ID )
		{
			isAnchored = true;
			updateWidget( anchor, frameCount, deltaTime, realTime );
		}

		anchor = widget->anchors[2].widget;
		if( anchor != ZP_UI_WIDGET_INVALID_ID )
		{
			isAnchored = true;
			updateWidget( anchor, frameCount, deltaTime, realTime );
		}

		anchor = widget->anchors[3].widget;
		if( anchor != ZP_UI_WIDGET_INVALID_ID )
		{
			isAnchored = true;
			updateWidget( anchor, frameCount, deltaTime, realTime );
		}

		if( isAnchored )
		{
			updateWidgetAnchors( widget );
		}
	}

	widget->children.foreach( [ this, frameCount, deltaTime, realTime ]( zp_int child ) { updateWidget( child, frameCount, deltaTime, realTime ); } );
}
void zpUICanvasComponent::updateWidgetAnchors( zpUIWidget* widget )
{
	zp_float lt, bt, rt, tt;
	zpUIWidgetAnchor* l = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_LEFT;
	zpUIWidgetAnchor* b = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_BOTTOM;
	zpUIWidgetAnchor* r = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_RIGHT;
	zpUIWidgetAnchor* t = widget->anchors + ZP_UI_WIDGET_ANCHOR_SIDE_TOP;
	zpVector4f sides[4];
	
	ZP_ALIGN16 zp_float pos[4];
	zpMath::Vector4Store4( widget->transform.m_m4, pos );

	const zpVector2f& pvt = s_rectPivots[ widget->pivot ];

	if( l->widget == b->widget && l->widget == r->widget && l->widget == t->widget )
	{
		if( l->widget == ZP_UI_WIDGET_INVALID_ID )
		{
			lt = pos[0] + l->absolute;
			bt = pos[1] + b->absolute;
			rt = pos[0] + r->absolute;
			tt = pos[1] + t->absolute;
		}
		else
		{
			zpUIWidget* relateTo = getWidget( l->widget );
			getWidgetSides( widget, relateTo, sides );

			zp_float s0x = zpMath::AsFloat( zpMath::Vector4GetX( sides[0] ) );
			zp_float s1y = zpMath::AsFloat( zpMath::Vector4GetY( sides[1] ) );
			zp_float s2x = zpMath::AsFloat( zpMath::Vector4GetX( sides[2] ) );
			zp_float s3y = zpMath::AsFloat( zpMath::Vector4GetY( sides[3] ) );

			lt = zp_lerp< zp_float >( s0x, s2x, l->relative ) + l->absolute;
			rt = zp_lerp< zp_float >( s0x, s2x, r->relative ) + r->absolute;
			bt = zp_lerp< zp_float >( s3y, s1y, b->relative ) + b->absolute;
			tt = zp_lerp< zp_float >( s3y, s1y, t->relative ) + t->absolute;
		}
	}
	else
	{
		// left
		if( l->widget != ZP_UI_WIDGET_INVALID_ID )
		{
			zpUIWidget* relateTo = getWidget( l->widget );
			getWidgetSides( widget, relateTo, sides );

			zp_float s0x = zpMath::AsFloat( zpMath::Vector4GetX( sides[0] ) );
			zp_float s2x = zpMath::AsFloat( zpMath::Vector4GetX( sides[2] ) );

			lt = zp_lerp< zp_float >( s0x, s2x, l->relative ) + l->absolute;
		}
		else
		{
			lt = pos[0] - pvt.getX() * (zp_float)widget->width;
		}

		// right
		if( r->widget != ZP_UI_WIDGET_INVALID_ID )
		{
			zpUIWidget* relateTo = getWidget( r->widget );
			getWidgetSides( widget, relateTo, sides );

			zp_float s0x = zpMath::AsFloat( zpMath::Vector4GetX( sides[0] ) );
			zp_float s2x = zpMath::AsFloat( zpMath::Vector4GetX( sides[2] ) );

			rt = zp_lerp< zp_float >( s0x, s2x, r->relative ) + r->absolute;
		}
		else
		{
			rt = pos[0] - pvt.getX() * (zp_float)widget->width + (zp_float)widget->width;
		}

		// bottom
		if( b->widget != ZP_UI_WIDGET_INVALID_ID )
		{
			zpUIWidget* relateTo = getWidget( r->widget );
			getWidgetSides( widget, relateTo, sides );

			zp_float s1y = zpMath::AsFloat( zpMath::Vector4GetY( sides[1] ) );
			zp_float s3y = zpMath::AsFloat( zpMath::Vector4GetY( sides[3] ) );

			bt = zp_lerp< zp_float >( s3y, s1y, b->relative ) + b->absolute;
		}
		else
		{
			bt = pos[1] - pvt.getY() * (zp_float)widget->height;
		}

		// top
		if( t->widget != ZP_UI_WIDGET_INVALID_ID )
		{
			zpUIWidget* relateTo = getWidget( r->widget );
			getWidgetSides( widget, relateTo, sides );

			zp_float s1y = zpMath::AsFloat( zpMath::Vector4GetY( sides[1] ) );
			zp_float s3y = zpMath::AsFloat( zpMath::Vector4GetY( sides[3] ) );

			tt = zp_lerp< zp_float >( s3y, s1y, t->relative ) + t->absolute;
		}
		else
		{
			tt = pos[1] - pvt.getY() * (zp_float)widget->height + (zp_float)widget->height;
		}
	}

	zp_float nx = zp_lerp( lt, rt, pvt.getX() );
	zp_float ny = zp_lerp( bt, tt, pvt.getY() );

	zpVector4f newPos = zpMath::Vector4( nx, ny, pos[2], 1.f );
	zp_int w = zp_floor_to_int( rt - lt + 0.5f );
	zp_int h = zp_floor_to_int( tt - bt + 0.5f );

	if( widget->aspectRatioLock != ZP_UI_WIDGET_LOCK_ASPECT_RATIO_FREE && widget->aspectRatio != 0.f )
	{
		if( widget->aspectRatioLock == ZP_UI_WIDGET_LOCK_ASPECT_RATIO_BASED_ON_HEIGHT )
		{
			w = zp_floor_to_int( (zp_float)h * widget->aspectRatio );
		}
		else
		{
			h = zp_floor_to_int( (zp_float)w / widget->aspectRatio );
		}
	}

	w = ZP_MAX( w, widget->minWidth );
	h = ZP_MAX( h, widget->minHeight );

	widget->transform.m_m4 = newPos;
	widget->width = w;
	widget->height = h;
}

void zpUICanvasComponent::sortWidget( zp_uint w )
{
	zpUIWidget* widget = getWidget( w );

	widget->children.sort( [ this ]( zp_int a, zp_int b ) -> zp_bool {
		zp_int da = getWidget( a )->depth;
		zp_int db = getWidget( b )->depth;
		return da < db;
	} );

	widget->children.foreach( [ this ]( zp_int child ) { sortWidget( child ); } );
}
void zpUICanvasComponent::clearDrawCalls()
{
	zpUIDrawCall* b = m_drawCalls.begin();
	zpUIDrawCall* e = m_drawCalls.end();
	for( ; b != e; ++b )
	{
		b->pos.reset();
		b->uvs.reset();
		b->cols.reset();
	}
}
void zpUICanvasComponent::fillWidget( zp_int w )
{
	zpUIWidget* widget = getWidget( w );
	if( widget->fillRenderer != ZP_NULL && widget->isVisible )
	{
		zpUIDrawCall* drawCall;
		zp_bool found = m_drawCalls.findIf( [ widget ]( zpUIDrawCall& dc ) { return widget->material.getResource() == dc.material.getResource(); }, &drawCall );
		if( !found )
		{
			drawCall = &m_drawCalls.pushBackEmpty();
			drawCall->material = widget->material;
		}

		widget->fillRenderer( widget, drawCall );
	}

	widget->children.foreach( [ this ]( zp_int child ) { fillWidget( child ); } );
}

void zpUICanvasComponent::createUIFromData( const zpBison::Value& def )
{
	if( !def.isEmpty() )
	{
		def.foreachArray( [ this ]( const zpBison::Value& d ) {
			createWidgetFromData( d, ZP_UI_WIDGET_ROOT_ID );
		} );
	}
}
void zpUICanvasComponent::createWidgetFromData( const zpBison::Value& def, zp_int parent )
{
	if( !def.isEmpty() )
	{
		zpUIWidget* widget = &m_widgets.pushBackEmpty();
		widget->id = m_ids++;

		zpUIWidget* parentWidget = getWidget( parent );
		widget->parent = parent;
		widget->depth = parentWidget->children.size();
		parentWidget->children.pushBack( widget->id );

		widget->name = def[ "Name" ].asCString();
		//widget->tag = def[ "Tag" ].asCString();

		zpVector4f p = zpMath::Vector4( 0, 0, 0, 1 );
		zpQuaternion4f r = zpMath::Quaternion( 0, 0, 0, 0 );
		zpVector4f s = zpMath::Vector4( 1, 1, 1, 0 );

		const zpBison::Value& pos = def[ "Position" ];
		if( !pos.isEmpty() )
		{
			p = zpMath::Vector4( pos[ 0 ].asFloat(), pos[ 1 ].asFloat(), 0.f, 1.f );
		}

		widget->transform = zpMath::TRS( p, r, s );
		widget->worldTransform = widget->transform;

		const zpBison::Value& border = def[ "Border" ];
		if( !border.isEmpty() )
		{
			widget->border = zpMath::Vector4( border[ 0 ].asFloat(), border[ 1 ].asFloat(), border[ 2 ].asFloat(), border[ 3 ].asFloat() );
		}
		
		const zpBison::Value& drawRegion = def[ "DrawRegion" ];
		if( !drawRegion.isEmpty() )
		{
			widget->drawRegion = zpMath::Vector4( drawRegion[ 0 ].asFloat(), drawRegion[ 1 ].asFloat(), drawRegion[ 2 ].asFloat(), drawRegion[ 3 ].asFloat() );

		}
		const zpBison::Value& offset = def[ "Offset" ];
		if( !offset.isEmpty() )
		{
			widget->offset = zpVector2f( offset[ 0 ].asFloat(), offset[ 1 ].asFloat() );
		}

		widget->width = def[ "Width" ].asInt();
		widget->height = def[ "Height" ].asInt();

		widget->alpha = def[ "Alpha" ].asFloat();

		const zp_char* materialName = def[ "Material" ].asCString();
		getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialName, widget->material );

		widget->fillRenderer = &BasicSpriteFill;

		const zpBison::Value& color = def[ "Color" ];
		if( !color.isEmpty() )
		{
			zpColor4f c( color[ 0 ].asFloat(), color[ 1 ].asFloat(), color[ 2 ].asFloat(), color[ 3 ].asFloat() );

			widget->colorBottomLeft = c;
			widget->colorTopLeft = c;
			widget->colorTopRight = c;
			widget->colorBottomRight = c;
		}

		const zpBison::Value& children = def[ "Children" ];

	}
}

void zpUICanvasComponent::getWidgetSides( zpUIWidget* widget, zpUIWidget* relativeTo, zpVector4f* sides )
{
	const zpVector2f& offset = s_rectPivots[ widget->pivot ];

	zp_float x0 = -offset.getX() * widget->width;
	zp_float y0 = -offset.getY() * widget->height;
	zp_float x1 = x0 + widget->width;
	zp_float y1 = y0 + widget->height;
	zp_float cx = ( x0 + x1 ) * 0.5f;
	zp_float cy = ( y0 + y1 ) * 0.5f;

	zpVector4f a = zpMath::Vector4( x0, cy, 0.f, 1.f );
	zpVector4f b = zpMath::Vector4( cx, y1, 0.f, 1.f );
	zpVector4f c = zpMath::Vector4( x1, cy, 0.f, 1.f );
	zpVector4f d = zpMath::Vector4( cx, y0, 0.f, 1.f );

	a = zpMath::MatrixTransform( widget->worldTransform, a );
	b = zpMath::MatrixTransform( widget->worldTransform, b );
	c = zpMath::MatrixTransform( widget->worldTransform, c );
	d = zpMath::MatrixTransform( widget->worldTransform, d );

	if( relativeTo != ZP_NULL )
	{
		zpMatrix4f inv = zpMath::Invert( relativeTo->worldTransform );
		a = zpMath::MatrixTransform( inv, a );
		b = zpMath::MatrixTransform( inv, b );
		c = zpMath::MatrixTransform( inv, c );
		d = zpMath::MatrixTransform( inv, d );
	}

	sides[0] = a;
	sides[1] = b;
	sides[2] = c;
	sides[3] = d;
}








zpUICanvasComponentPool::zpUICanvasComponentPool()
{

}
zpUICanvasComponentPool::~zpUICanvasComponentPool()
{

}

void zpUICanvasComponentPool::render( zpRenderingContext* context )
{
	zpUICanvasComponent** b = m_used.begin();
	zpUICanvasComponent** e = m_used.end();
	for( ; b != e; ++b )
	{
		(*b)->render( context );
	}
}

void zpUICanvasComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	zpUICanvasComponent** b = m_used.begin();
	zpUICanvasComponent** e = m_used.end();
	for( ; b != e; ++b )
	{
		(*b)->update( deltaTime, realTime );
	}
}
