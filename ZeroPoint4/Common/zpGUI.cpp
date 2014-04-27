#include "zpCommon.h"

void zpGUI::create()
{
	m_application->getRenderPipeline()->getMaterialContentManager()->getResource( "materials/gui.materialb", m_guiMaterial );
	m_application->getRenderPipeline()->getFontContentManager()->getResource( "fonts/arial32.fontb", m_guiFont );

	m_renderingContext = m_application->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();

	m_mainColor.set( 0.1f, 0.1f, 0.1f, 1 );
	m_backgroundColor.set( 0.8f, 0.8f, 0.8f, 1 );

	m_isDrawingWidgets = false;
	m_currentlySelected = ZP_NULL;
}
void zpGUI::destroy()
{
	m_guiMaterial.release();
	m_guiFont.release();
}

void zpGUI::beginWindow( const zp_char* title, const zpRectf& rect, zpRectf& outPos )
{
	zpGUIWidget* window = &m_allWidgets.pushBackEmpty();
	window->type = ZP_GUI_WIDGET_CONTAINER;
	window->localRect = rect;
	window->parent = ZP_NULL;
	window->text = title;
	window->color = m_backgroundColor;

	if( !m_widgetStack.isEmpty() )
	{
		window->parent = m_widgetStack.back();
	}

	m_widgetStack.pushBack( window );

	zpGUIWidget* label = addWidget( 16 );
	label->type = ZP_GUI_WIDGET_LABEL;
	label->text = title;
	label->color = m_mainColor;

	zpRectf worldRect;
	zpVector2i mousePos, mouseDelta;
	zp_bool isDown;
	zp_bool isPressed;
	zp_bool over = isMouseOverWidget( label, worldRect, mousePos, mouseDelta, isDown, isPressed );

	if( over && isDown )
	{
		window->localRect.setPosition( window->localRect.getPosition() + mouseDelta );
	}

	outPos = window->localRect;
}
void zpGUI::endWindow()
{
	m_widgetStack.popBack();
}

void zpGUI::label( zp_float size, const zp_char* text, const zpColor4f& color )
{
	zpGUIWidget* label = addWidget( size );
	label-> type = ZP_GUI_WIDGET_LABEL;
	label->text = text;
	label->color = color;
}

zp_bool zpGUI::button( zp_float size, const zp_char* text )
{
	zpGUIWidget* button = addWidget( size + 4.0f );
	button->type = ZP_GUI_WIDGET_CONTAINER;

	zpRectf worldRect;
	zpVector2i mousePos, mouseDelta;
	zp_bool isDown;
	zp_bool isPressed;
	zp_bool over = isMouseOverWidget( button, worldRect, mousePos, mouseDelta, isDown, isPressed );

	button->color = over ? isDown ? m_backgroundColor * 0.4f : m_backgroundColor * 0.6f : m_backgroundColor * 0.8f;

	zpGUIWidget* labal = addChildWidget( size, button );
	labal->type = ZP_GUI_WIDGET_LABEL;
	labal->text = text;
	labal->color = m_mainColor;

	return over && isPressed;
}

zp_bool zpGUI::text( zp_float size, const zp_char* text, const zpColor4f& color, zp_char* outText )
{
	zpGUIWidget* label = addWidget( size );
	label->type = ZP_GUI_WIDGET_LABEL;
	label->text = text;

	zpRectf worldRect;
	zpVector2i mousePos, mouseDelta;
	zp_bool isDown;
	zp_bool isPressed;
	zp_bool over = isMouseOverWidget( label, worldRect, mousePos, mouseDelta, isDown, isPressed );

	label->color = m_selectedWidget != ZP_NULL ? color : color * 0.8f;

	return m_selectedWidget == label;
}

void zpGUI::startGUI()
{
	m_allWidgets.reset();
	m_renderingContext->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI_DEBUG, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
	m_isDrawingWidgets = true;
}
void zpGUI::endGUI()
{
	ZP_ASSERT_WARN( m_widgetStack.isEmpty(), "Widget still on stack" );
	drawWidgets();

	if( m_isDrawingWidgets )
	{
		m_renderingContext->endDrawImmediate();
	}
	else
	{
		m_renderingContext->endDrawFont();
	}
}

zpGUI::zpGUIWidget* zpGUI::addWidget( zp_float height )
{
	zpGUIWidget* window = m_widgetStack.back();
	return addChildWidget( height, window );
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

void zpGUI::getWorldRect( zpGUIWidget* widget, zpRectf& outWorldRect )
{
	outWorldRect = widget->localRect;

	while( ( widget = widget->parent ) != ZP_NULL )
	{
		outWorldRect.setPosition( outWorldRect.getPosition() + widget->localRect.getPosition() );
	}
}
zp_bool zpGUI::isMouseOverWidget( zpGUIWidget* widget, zpRectf& outWorldRect, zpVector2i& mousePos, zpVector2i& mouseDelta, zp_bool& isDown, zp_bool& isPressed  )
{
	getWorldRect( widget, outWorldRect );

	const zpMouse* mouse = m_application->getInputManager()->getMouse();
	mousePos = mouse->getLocation();
	mouseDelta = mouse->getDelta();
	isDown = mouse->isButtonDown( ZP_MOUSE_BUTTON_LEFT );
	isPressed = mouse->isButtonPressed( ZP_MOUSE_BUTTON_LEFT );

	if( isPressed )
	{
		if( m_selectedWidget != widget )
		{
			m_selectedWidget = widget;
		}
		else
		{
			m_selectedWidget = ZP_NULL;
		}
	}

	return outWorldRect.contains( mousePos );
}

void zpGUI::drawWidgets()
{
	zpGUIWidget* b = m_allWidgets.begin();
	zpGUIWidget* e = m_allWidgets.end();
	for( ; b != e; ++b )
	{
		if( b->parent == ZP_NULL ) drawWidget( b );
	}
}
void zpGUI::drawWidget( zpGUIWidget* widget )
{
	zpRectf worldRect;
	getWorldRect( widget, worldRect );

	switch( widget->type )
	{
	case ZP_GUI_WIDGET_CONTAINER:
		{
			if( !m_isDrawingWidgets )
			{
				m_renderingContext->endDrawFont();
				m_renderingContext->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI_DEBUG, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
				m_isDrawingWidgets = true;
			}

			m_renderingContext->addQuad(
				worldRect.getTopLeft().asVector4(),
				worldRect.getTopRight().asVector4(),
				worldRect.getBottomRight().asVector4(),
				worldRect.getBottomLeft().asVector4(),
				widget->color );

			zpGUIWidget** b = widget->children.begin();
			zpGUIWidget** e = widget->children.end();
			for( ; b != e; ++b )
			{
				drawWidget( *b );
			}
		}
		break;
	case ZP_GUI_WIDGET_BUTTON:
		{
			if( !m_isDrawingWidgets )
			{
				m_renderingContext->endDrawFont();
				m_renderingContext->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI_DEBUG, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
				m_isDrawingWidgets = true;
			}

			m_renderingContext->addQuad(
				worldRect.getTopLeft().asVector4(),
				worldRect.getTopRight().asVector4(),
				worldRect.getBottomRight().asVector4(),
				worldRect.getBottomLeft().asVector4(),
				widget->color );
		}
		break;
	case ZP_GUI_WIDGET_LABEL:
		{
			if( m_isDrawingWidgets )
			{
				m_renderingContext->endDrawImmediate();
				m_renderingContext->beginDrawFont( 1 << 4, ZP_RENDERING_QUEUE_UI_DEBUG, &m_guiFont );
				m_isDrawingWidgets = false;
			}

			m_renderingContext->addText( widget->text.str(), worldRect.getSize().getY(), worldRect.getPosition(), ZP_FONT_ALIGNMENT_LEFT, widget->color );
		}
		break;
	}
}
