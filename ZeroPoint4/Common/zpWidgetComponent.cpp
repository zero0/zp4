#include "zpCommon.h"

zpWidgetComponent::zpWidgetComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_isAdded( false )
	, m_canvas( ZP_NULL )
	, m_textHeight( 12.f )
	, m_fontAlignment( ZP_FONT_ALIGNMENT_LEFT )
	, m_depth( 0 )
	, m_type( ZP_WIDGET_CONTAINER )
	, m_text()
	, m_border( zpMath::Vector4( 0, 0, 0, 0 ) )
	, m_colorBottomLeft( 1, 1, 1, 1 )
	, m_colorTopLeft( 1, 1, 1, 1 )
	, m_colorTopRight( 1, 1, 1, 1 )
	, m_colorBottomRight( 1, 1, 1, 1 )
{

}
zpWidgetComponent::~zpWidgetComponent()
{

}

void zpWidgetComponent::fill( zpRenderingContext* context )
{
	switch( m_type )
	{
	case ZP_WIDGET_CONTAINER:
		break;

	case ZP_WIDGET_SPRITE:
		fillSprite( context );
		break;

	case ZP_WIDGET_LABEL:
		fillLabel( context );
		break;

	}
}

void zpWidgetComponent::setColor( const zpColor4f& color )
{
	m_colorBottomLeft = color;
	m_colorTopLeft = color;
	m_colorTopRight = color;
	m_colorBottomRight = color;
}
void zpWidgetComponent::setColor( const zpColor4f& topColor, const zpColor4f& bottomColor )
{
	m_colorBottomLeft = bottomColor;
	m_colorTopLeft = topColor;
	m_colorTopRight = topColor;
	m_colorBottomRight = bottomColor;
}
void zpWidgetComponent::setColor( const zpColor4f& bottomLeft, const zpColor4f& topLeft, const zpColor4f& topRight, const zpColor4f& bottomRight )
{
	m_colorBottomLeft = bottomLeft;
	m_colorTopLeft = topLeft;
	m_colorTopRight = topRight;
	m_colorBottomRight = bottomRight;
}

const zpString& zpWidgetComponent::getText() const
{
	return m_text;
}
void zpWidgetComponent::setText( const zpString& text )
{
	m_text = text;
}
void zpWidgetComponent::setText( const zp_char* text )
{
	m_text = text;
}

zp_int zpWidgetComponent::getDepth() const
{
	return m_depth;
}
void zpWidgetComponent::setDepth( zp_int depth )
{
	if( m_depth != depth )
	{
		m_depth = depth;
	}
}

void zpWidgetComponent::onCreate()
{

}
void zpWidgetComponent::onInitialize()
{
	zpTransformComponent* parent = getParentObject()->getComponents()->getTransformComponent();
	while( parent != ZP_NULL )
	{
		m_canvas = parent->getParentObject()->getComponents()->getCanvasComponent();
		if( m_canvas != ZP_NULL ) break;

		parent = parent->getParent();
	}

	addToCanvas();
}
void zpWidgetComponent::onDestroy()
{
	removeFromCanvas();
}

void zpWidgetComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{

}
void zpWidgetComponent::onSimulate()
{

}

void zpWidgetComponent::onEnabled()
{
	addToCanvas();
}
void zpWidgetComponent::onDisabled()
{
	removeFromCanvas();
}

void zpWidgetComponent::addToCanvas()
{
	if( !m_isAdded )
	{
		m_isAdded = true;
		m_canvas->addWidget( this );
	}
}
void zpWidgetComponent::removeFromCanvas()
{
	if( m_isAdded )
	{
		m_isAdded = false;
		m_canvas->removeWidget( this );
	}
}

void zpWidgetComponent::fillSprite( zpRenderingContext* context )
{
	zpRectTransformComponent* rect = getParentObject()->getComponents()->getRectTransformComponent();

	zpVector4f corners[4];
	rect->getWorldCorners( corners );

	zpRectf uv( 0, 0, 1, 1 );

	context->addQuad(
		corners[ 0 ], uv.getBottomLeft(), m_colorBottomLeft,
		corners[ 1 ], uv.getTopLeft(), m_colorTopLeft,
		corners[ 2 ], uv.getTopRight(), m_colorTopRight,
		corners[ 3 ], uv.getBottomRight(), m_colorBottomRight
		);
}
void zpWidgetComponent::fillLabel( zpRenderingContext* context )
{
	context->addText( m_text, m_textHeight, zpVector2f( 0, 0 ), m_fontAlignment, m_colorTopLeft, m_colorBottomLeft );
}



zpWidgetComponentPool::zpWidgetComponentPool()
{

}
zpWidgetComponentPool::~zpWidgetComponentPool()
{

}

void zpWidgetComponentPool::render( zpRenderingContext* context )
{

}

void zpWidgetComponentPool::update( zp_float deltaTime, zp_float realTime )
{

}
