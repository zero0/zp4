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
	window->localRect = rect;
	window->parent = ZP_NULL;

	if( !m_widgetStack.isEmpty() )
	{
		window->parent = m_widgetStack.back();
	}

	m_widgetStack.pushBack( window );

	outPos = window->localRect;
}
void zpGUI::endWindow()
{
	m_widgetStack.popBack();
}

void zpGUI::label( const zp_char* text )
{
	zpGUIWidget* label = addWidget();
}

zp_bool zpGUI::button( const zp_char* text )
{
	zpGUIWidget* button = addWidget();

	zpRectf worldRect;
	zp_bool isDown;
	zp_bool over = isMouseOverWidget( button, worldRect, isDown );

	m_renderingContext->addQuad(
		zpVector4f( worldRect.getLeft(),  worldRect.getBottom(), 0, 1 ),
		zpVector4f( worldRect.getLeft(),  worldRect.getTop(),    0, 1 ),
		zpVector4f( worldRect.getRight(), worldRect.getTop(), 0, 1 ),
		zpVector4f( worldRect.getRight(), worldRect.getBottom(),    0, 1 ),
		over ? m_backgroundColor * 0.8f : m_backgroundColor
		);

	return over && isDown;
}

zp_bool zpGUI::text( const zp_char* text, zp_char* outText )
{
	zpGUIWidget* label = addWidget();

	return false;
}

void zpGUI::startGUI()
{
	m_allWidgets.reset();

	m_renderingContext = m_application->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();
	m_renderingContext->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
}
void zpGUI::endGUI()
{
	ZP_ASSERT_WARN( m_widgetStack.isEmpty(), "Widget still on stack" );
	m_renderingContext->endDrawImmediate();
}

zpGUI::zpGUIWidget* zpGUI::addWidget()
{
	zpGUIWidget* widget = &m_allWidgets.pushBackEmpty();
	zpGUIWidget* window = m_widgetStack.back();

	zp_float height = 12.f;
	zp_float space = 1.f;
	zpVector2f margin( 2, 2 );

	zpVector2f pos = margin + zpVector2f( 0, height );
	pos.setY( pos.getY() * window->children.size() );

	zpVector2f size( window->localRect.getSize().getX() - margin.getX(), height );

	window->children.pushBack( widget );
	widget->localRect = zpRectf( pos, size );
	widget->parent = window;

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