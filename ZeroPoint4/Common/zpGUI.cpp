#include "zpCommon.h"

void zpGUI::create()
{
	m_renderingContext = m_application->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();

	m_isDrawingWidgets = false;
	m_currentlySelected = ZP_NULL;
}
void zpGUI::setup()
{
	m_application->getRenderPipeline()->getMaterialContentManager()->getResource( "materials/gui.materialb", m_guiMaterial );
	m_application->getRenderPipeline()->getFontContentManager()->getResource( "fonts/arial32.fontb", m_guiFont );

	m_mainColor.set( 0.1f, 0.1f, 0.1f, 1 );
	m_backgroundColor.set( 0.8f, 0.8f, 0.8f, 1 );
	m_margin.set( 2, 2 );
}
void zpGUI::teardown()
{
	m_guiMaterial.release();
	m_guiFont.release();
}
void zpGUI::destroy()
{
	
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

	zpGUIWidget* label = addWidget( 1.f, 16 );
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
	m_margin.set( 2, 2 );
}

void zpGUI::box( zp_float widthPrecent, zp_float size, const zpColor4f& color )
{
	zpGUIWidget* box = addWidget( widthPrecent, size );
	box->type = ZP_GUI_WIDGET_CONTAINER;
	box->color = color;
}

void zpGUI::label( zp_float size, const zp_char* text, const zpColor4f& color )
{
	zpGUIWidget* label = addWidget( 1.f, size );
	label->type = ZP_GUI_WIDGET_LABEL;
	label->text = text;
	label->color = color;
}

zp_bool zpGUI::button( zp_float size, const zp_char* text )
{
	zpGUIWidget* button = addWidget( 1.f, size + 4.0f );
	button->type = ZP_GUI_WIDGET_CONTAINER;

	zpRectf worldRect;
	zpVector2i mousePos, mouseDelta;
	zp_bool isDown;
	zp_bool isPressed;
	zp_bool over = isMouseOverWidget( button, worldRect, mousePos, mouseDelta, isDown, isPressed );

	button->color = over ? isDown ? m_backgroundColor * 0.4f : m_backgroundColor * 0.6f : m_backgroundColor * 0.8f;

	zpGUIWidget* labal = addChildWidget( 1.f, size, button );
	labal->type = ZP_GUI_WIDGET_LABEL;
	labal->text = text;
	labal->color = m_mainColor;

	return over && isPressed;
}

zp_bool zpGUI::text( zp_float size, const zp_char* text, const zpColor4f& color, zp_char* outText )
{
	zpGUIWidget* label = addWidget( 1.f, size );
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
	m_margin.set( 2, 2 );
}
void zpGUI::endGUI()
{
	ZP_ASSERT_WARN( m_widgetStack.isEmpty(), "Widget still on stack" );

	m_renderingContext->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI_DEBUG, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
	m_isDrawingWidgets = true;

	drawWidgets();

	if( m_isDrawingWidgets )
	{
		m_renderingContext->endDrawImmediate();
	}
	else
	{
		m_renderingContext->endDrawFont();
	}

	m_allWidgets.reset();
}

void zpGUI::setMargin( zp_float w, zp_float h )
{
	m_margin.set( w, h );
}

zpGUI::zpGUIWidget* zpGUI::addWidget( zp_float widthPercent, zp_float height )
{
	zpGUIWidget* window = ZP_NULL;

	if( !m_widgetStack.isEmpty() )
	{
		window = m_widgetStack.back();

	}
	return addChildWidget( widthPercent, height, window );
}
zpGUI::zpGUIWidget* zpGUI::addChildWidget( zp_float widthPercent, zp_float height, zpGUIWidget* parent )
{
	zpGUIWidget* widget = &m_allWidgets.pushBackEmpty();

	zpVector2f pos = m_margin;
	zpVector2f size( m_margin.getX(), height );

	widget->parent = parent;
	if( parent != ZP_NULL )
	{
		zpGUIWidget** b = parent->children.begin();
		zpGUIWidget** e = parent->children.end();
		for( ; b != e; ++b )
		{
			pos.setY( pos.getY() + (*b)->localRect.getSize().getY() + m_margin.getY() );
		}

		size.setX( widthPercent * ( parent->localRect.getSize().getX() - ( m_margin.getX() * 2.f ) ) );

		parent->children.pushBack( widget );
	}
	widget->localRect = zpRectf( pos, size );

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
