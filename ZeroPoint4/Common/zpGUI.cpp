#include "zpCommon.h"

void zpGUI::create()
{

}
void zpGUI::setup()
{
    m_isDrawingWidgets = false;
    m_currentlySelected = ZP_NULL;

    m_application->getRenderPipeline()->getMaterialContentManager()->getResource( "materials/gui.materialb", m_guiMaterial );
    m_application->getRenderPipeline()->getFontContentManager()->getResource( "fonts/arial32.fontb", m_guiFont );

    m_mainColor.set( 0.1f, 0.1f, 0.1f, 1 );
    m_backgroundColor.set( 0.8f, 0.8f, 0.8f, 1 );
    m_margin.set( 2, 2 );

    m_allWidgets.resize( ZP_GUI_NUM_WIDGETS );

    zpGUIWidget* b = m_allWidgets.begin();
    zpGUIWidget* e = m_allWidgets.end();
    for( ; b != e; ++b )
    {
        m_freeWidgets.pushBack( b );
    }
}
void zpGUI::teardown()
{
    ZP_ASSERT( m_usedWidgets.isEmpty(), "" );

    m_guiMaterial.release();
    m_guiFont.release();

    m_usedWidgets.clear();
    m_freeWidgets.clear();
    m_allWidgets.clear();
    m_widgetStack.clear();
}
void zpGUI::destroy()
{
    
}

void zpGUI::beginWindow( const zp_char* title, const zpRectf& rect, zpRectf& outPos )
{
    zpGUIWidget* window = m_freeWidgets.back();
    m_freeWidgets.popBack();
    m_usedWidgets.pushBack( window );

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

    // draw widgets
    zpRenderingContext* context = m_application->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();
    drawWidgets( context );

    // push used widgets back to the free list
    zpGUIWidget** b = m_usedWidgets.begin();
    zpGUIWidget** e = m_usedWidgets.end();
    for( ; b != e; ++b )
    {
        zpGUIWidget* w = *b;
        w->text.clear();
        w->children.clear();
        m_freeWidgets.pushBack( w );
    }
    m_usedWidgets.reset();
    m_widgetStack.reset();
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
    zpGUIWidget* widget = m_freeWidgets.back();
    m_freeWidgets.popBack();
    m_usedWidgets.pushBack( widget );

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

    zpVector2f pos = outWorldRect.getPosition();

    zpGUIWidget* parent = widget->parent;
    while( parent != ZP_NULL )
    {
        pos += parent->localRect.getPosition();
        parent = parent->parent;
    }

    outWorldRect.setPosition( pos );
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

void zpGUI::drawWidgets( zpRenderingContext* context )
{
    // begin immediate mode
    context->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
    m_isDrawingWidgets = true;

    // draw widgets
    zpGUIWidget** b = m_usedWidgets.begin();
    zpGUIWidget** e = m_usedWidgets.end();
    for( ; b != e; ++b )
    {
        zpGUIWidget* w = *b;
        if( w->parent == ZP_NULL )
        {
            drawWidget( context, w );
        }
    }

    // end immediate mode
    if( m_isDrawingWidgets )
    {
        context->endDrawImmediate();
    }
    else
    {
        context->endDrawFont();
    }

}
void zpGUI::drawWidget( zpRenderingContext* context, zpGUIWidget* widget )
{
    zpRectf worldRect;
    getWorldRect( widget, worldRect );

    switch( widget->type )
    {
    case ZP_GUI_WIDGET_CONTAINER:
        {
            if( !m_isDrawingWidgets )
            {
                context->endDrawFont();
                context->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
                m_isDrawingWidgets = true;
            }

            context->addQuad(
                worldRect.getTopLeft().asVector4(),
                worldRect.getTopRight().asVector4(),
                worldRect.getBottomRight().asVector4(),
                worldRect.getBottomLeft().asVector4(),
                widget->color );

            zpGUIWidget** b = widget->children.begin();
            zpGUIWidget** e = widget->children.end();
            for( ; b != e; ++b )
            {
                drawWidget( context, *b );
            }
        }
        break;
    case ZP_GUI_WIDGET_BUTTON:
        {
            if( !m_isDrawingWidgets )
            {
                context->endDrawFont();
                context->beginDrawImmediate( 1 << 4, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR, &m_guiMaterial );
                m_isDrawingWidgets = true;
            }

            context->addQuad(
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
                context->endDrawImmediate();
                context->beginDrawFont( 1 << 4, ZP_RENDERING_QUEUE_UI, &m_guiFont );
                m_isDrawingWidgets = false;
            }

            context->addText( widget->text.str(), worldRect.getSize().getY(), worldRect.getPosition(), ZP_FONT_ALIGNMENT_LEFT, widget->color );
        }
        break;
    }
}
