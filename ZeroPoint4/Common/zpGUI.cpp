#include "zpCommon.h"

void zpGUI::create()
{
	m_application->getRenderPipeline()->getMaterialContentManager()->getResource( "materials/gui.materialb", m_guiMaterial );

	m_mainColor.set( 1, 1, 1, 1 );
	m_backgroundColor.set( 0.8f, 0.8f, 0.8f, 1 );
}
void zpGUI::destroy()
{
	m_guiMaterial.release();
}

void zpGUI::beginWindow( const zp_char* title, const zpRectf& rect, zpRectf& outPos )
{
	zpGUIWidget* window = &m_allWidgets.pushBackEmpty();
	window->type = ZP_GUI_WIDGET_CONTAINER;
	window->localRect = rect;
	window->parent = ZP_NULL;

	if( !m_widgetStack.isEmpty() )
	{
		window->parent = m_widgetStack.back();
	}

	m_widgetStack.pushBack( window );

	m_renderingContext->addQuad(
		zpVector4f( rect.getLeft(),  rect.getBottom(), 0, 1 ),
		zpVector4f( rect.getLeft(),  rect.getTop(),    0, 1 ),
		zpVector4f( rect.getRight(), rect.getTop(), 0, 1 ),
		zpVector4f( rect.getRight(), rect.getBottom(),    0, 1 ),
		zpColor4f( 1, 1, 1, 1 )
		);

	outPos = window->localRect;
}
void zpGUI::endWindow()
{
	m_widgetStack.popBack();
}

void zpGUI::label( zp_float size, const zp_char* text )
{
	zpGUIWidget* label = addWidget( size );

	zpRectf worldRect;
	getWorldRect( label, worldRect );
}

zp_bool zpGUI::button( zp_float size, const zp_char* text )
{
	zpGUIWidget* button = addWidget( size );
	button->type = ZP_GUI_WIDGET_BUTTON;

	zpRectf worldRect;
	zp_bool isDown;
	zp_bool over = isMouseOverWidget( button, worldRect, isDown );

	m_renderingContext->addQuad(
		worldRect.getTopLeft().asVector4(),
		worldRect.getTopRight().asVector4(),
		worldRect.getBottomRight().asVector4(),
		worldRect.getBottomLeft().asVector4(),
		over ? m_backgroundColor * 0.8f : m_backgroundColor
		);

	return over && isDown;
}

zp_bool zpGUI::text( zp_float size, const zp_char* text, zp_char* outText )
{
	zpGUIWidget* label = addWidget( size );
	label->type = ZP_GUI_WIDGET_LABEL;

	zpRectf worldRect;
	getWorldRect( label, worldRect );

	return false;
}

void zpGUI::startGUI()
{
	m_allWidgets.reset();

	m_renderingContext = m_application->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();
	m_renderingContext->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI_DEBUG, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
}
void zpGUI::endGUI()
{
	ZP_ASSERT_WARN( m_widgetStack.isEmpty(), "Widget still on stack" );
	m_renderingContext->endDrawImmediate();
}

zpGUI::zpGUIWidget* zpGUI::addWidget( zp_float height )
{
	zpGUIWidget* widget = &m_allWidgets.pushBackEmpty();
	zpGUIWidget* window = m_widgetStack.back();

	zpVector2f margin( 2, 2 );

	zpVector2f pos = margin;
	zpGUIWidget** b = window->children.begin();
	zpGUIWidget** e = window->children.end();
	for( ; b != e; ++b )
	{
		pos.setY( pos.getY() + (*b)->localRect.getSize().getY() + margin.getY() );
	}

	zpVector2f size( window->localRect.getSize().getX() - ( margin.getX() * 2.f ), height );

	window->children.pushBack( widget );
	widget->localRect = zpRectf( pos, size );
	widget->parent = window;

	return widget;
}
zpGUI::zpGUIWidget* zpGUI::addChildWidget( zp_float height, zpGUIWidget* parent )
{
	zpGUIWidget* widget = &m_allWidgets.pushBackEmpty();

	zpVector2f margin( 2, 2 );

	zpVector2f pos = margin;
	zpGUIWidget** b = parent->children.begin();
	zpGUIWidget** e = parent->children.end();
	for( ; b != e; ++b )
	{
		pos.setY( pos.getY() + (*b)->localRect.getSize().getY() + margin.getY() );
	}

	zpVector2f size( parent->localRect.getSize().getX() - ( margin.getX() * 2.f ), height );

	parent->children.pushBack( widget );
	widget->localRect = zpRectf( pos, size );
	widget->parent = parent;

	return widget;
}

void zpGUI::getWorldRect( zpGUIWidget* widget, zpRectf& outWorldRect ) const
{
	outWorldRect = widget->localRect;

	while( ( widget = widget->parent ) != ZP_NULL )
	{
		outWorldRect.setPosition( outWorldRect.getPosition() + widget->localRect.getPosition() );
	}
}
zp_bool zpGUI::isMouseOverWidget( zpGUIWidget* widget, zpRectf& outWorldRect, zp_bool& isDown ) const
{
	getWorldRect( widget, outWorldRect );

	const zpMouse* mouse = m_application->getInputManager()->getMouse();
	zpVector2f pos = mouse->getLocation();
	isDown = mouse->isButtonPressed( ZP_MOUSE_BUTTON_LEFT );

	return outWorldRect.contains( pos );
}
