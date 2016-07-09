#include "zpCommon.h"

zpCanvasComponent::zpCanvasComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_layer( 0 )
    , m_backgroundColor( 0, 0, 0, 0 )
    , m_queue( ZP_RENDERING_QUEUE_UI )
    , m_canvasType( ZP_CANVAS_RENDER_TYPE_2D_UI )
{
    const zpBison::Value& layer = def[ "Layer" ];
    const zpBison::Value& backgroundColor = def[ "BackgroundColor" ];
    const zpBison::Value& material = def[ "Material" ];

    if( layer.isInt() )
    {
        m_layer = layer.asInt();
    }

    if( !backgroundColor.isEmpty() )
    {
        m_backgroundColor = zpColor4f( backgroundColor[ 0 ].asFloat(), backgroundColor[ 1 ].asFloat(), backgroundColor[ 2 ].asFloat(), backgroundColor[ 3 ].asFloat() );
    }

    if( material.isString() )
    {
        zp_bool ok = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( material.asCString(), m_material );
        ZP_ASSERT( ok, "Failed to get material %s", material.asCString() );
    }
}
zpCanvasComponent::~zpCanvasComponent()
{

}

void zpCanvasComponent::render( zpRenderingContext* context )
{
    zpVector4f corners[4];
    zpRectf uvs( 0, 0, 1, 1 );

    zpRectTransformComponent* rect = getParentObject()->getComponents()->getRectTransformComponent();

    rect->getWorldCorners( corners );

    context->beginDrawImmediate( m_layer, m_queue, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, &m_material );
    context->addQuad(
        corners[ 0 ], uvs.getBottomLeft(), m_backgroundColor,
        corners[ 1 ], uvs.getTopLeft(), m_backgroundColor,
        corners[ 2 ], uvs.getTopRight(), m_backgroundColor,
        corners[ 3 ], uvs.getBottomRight(), m_backgroundColor
        );
    context->endDrawImmediate();
}

void zpCanvasComponent::addWidget( zpWidgetComponent* w )
{
    m_widgets.pushBack( w );
}
void zpCanvasComponent::removeWidget( zpWidgetComponent* w )
{
    m_widgets.eraseAll( w );
}
void zpCanvasComponent::removeAllWidgets()
{
    m_widgets.clear();
}

void zpCanvasComponent::onCreate()
{
    
}
void zpCanvasComponent::onInitialize()
{

}
void zpCanvasComponent::onDestroy()
{

}

void zpCanvasComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{

}
void zpCanvasComponent::onSimulate()
{

}

void zpCanvasComponent::onEnabled()
{

}
void zpCanvasComponent::onDisabled()
{

}



zpCanvasComponentPool::zpCanvasComponentPool()
{

}
zpCanvasComponentPool::~zpCanvasComponentPool()
{

}

void zpCanvasComponentPool::render( zpRenderingContext* context )
{
    m_used.foreach( [ context ]( zpCanvasComponent* o ) {
        o->render( context );
    } );
}

void zpCanvasComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpCanvasComponent* o ) {
        o->update( deltaTime, realTime );
    } );
}

#if 0
zpUIComponent::zpUIComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_type( ZP_UI_COMPONENT_TYPE_CONTAINER )
    , m_pivot( ZP_UI_PIVOT_TOP_LEFT )
    , m_width( 0 )
    , m_height( 0 )
    , m_localScale( 1, 1 )
    , m_localRotation( 0.f )
    , m_padding( 0, 0, 0, 0 )
    , m_boarder( 0, 0, 0, 0 )
    , m_drawRegion( 0, 0, 1, 1 )
    , m_color0( 1, 1, 1, 1 )
    , m_color1( 1, 1, 1, 1 )
    , m_color2( 1, 1, 1, 1 )
    , m_color3( 1, 1, 1, 1 )
    , m_alpha( 1 )
    , m_text()
    , m_textHeight( 0 )
    , m_filledValue( 0 )
    , m_step( 0 )
    , m_depth( 0 )
    , m_isChanged( true )
{
    m_width = def[ "Width" ].asInt();
    m_height = def[ "Height" ].asInt();

    const zpBison::Value& padding = def[ "Padding" ];
    if( !padding.isNull() )
    {
        m_padding = zpVector4f( padding[ 0 ].asFloat(), padding[ 1 ].asFloat(), padding[ 2 ].asFloat(), padding[ 3 ].asFloat() );
    }

    const zpBison::Value& boarder = def[ "Boarder" ];
    if( !boarder.isNull() )
    {
        m_boarder = zpVector4f( boarder[ 0 ].asFloat(), boarder[ 1 ].asFloat(), boarder[ 2 ].asFloat(), boarder[ 3 ].asFloat() );
    }

    const zpBison::Value& drawRegion = def[ "DrawRegion" ];
    if( !drawRegion.isNull() )
    {
        m_drawRegion = zpVector4f( drawRegion[ 0 ].asFloat(), drawRegion[ 1 ].asFloat(), drawRegion[ 2 ].asFloat(), drawRegion[ 3 ].asFloat() );
    }

    const zpBison::Value& color = def[ "Color" ];
    if( !color.isNull() )
    {
        zpColor4f c( color[ 0 ].asFloat(), color[ 1 ].asFloat(), color[ 2 ].asFloat(), color[ 3 ].asFloat() );
        m_color0 = c;
        m_color1 = c;
        m_color2 = c;
        m_color3 = c;
    }

    const zpBison::Value& text = def[ "Text" ];
    if( !text.isNull() )
    {
        m_text = text.asCString();
    }

    const zp_char* material = def[ "Material" ].asCString();
    getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( material, m_material );
}
zpUIComponent::~zpUIComponent()
{

}

void zpUIComponent::render( zpRenderingContext* context )
{
    switch( m_type )
    {
    case ZP_UI_COMPONENT_TYPE_SPRITE:
        {
            zpVector4f tl, tr, bl, br;
            getDrawingDimensions( tl, tr, bl, br );
            zpRectf uvs( 0, 0, 1, 1 );

            context->addQuad(
                tl, uvs.getTopLeft(), m_color0,
                tr, uvs.getTopRight(), m_color1,
                br, uvs.getBottomRight(), m_color3,
                bl, uvs.getBottomLeft(), m_color2
                );
        }
        break;
    }
}

void zpUIComponent::setColor( const zpColor4f& color )
{
    m_color0 = color;
    m_color1 = color;
    m_color2 = color;
    m_color3 = color;
}
void zpUIComponent::setColor( const zpColor4f& colorTop, const zpColor4f& colorBottom )
{
    m_color0 = colorTop;
    m_color1 = colorTop;
    m_color2 = colorBottom;
    m_color3 = colorBottom;
}
void zpUIComponent::setColor( const zpColor4f& colorTopLeft, const zpColor4f& colorTopRight, const zpColor4f& colorBottomLeft, const zpColor4f& colorBottomRight )
{
    m_color0 = colorTopLeft;
    m_color1 = colorTopRight;
    m_color2 = colorBottomLeft;
    m_color3 = colorBottomRight;
}

const zpVector4f& zpUIComponent::getPivotOffset() const
{
    static const zpVector4f offsets[] = {
        zpVector4f( 0.0f, 1.0f, 0, 0 ),
        zpVector4f( 0.5f, 1.0f, 0, 0 ),
        zpVector4f( 1.0f, 1.0f, 0, 0 ),

        zpVector4f( 0.0f, 0.5f, 0, 0 ),
        zpVector4f( 0.5f, 0.5f, 0, 0 ),
        zpVector4f( 1.0f, 0.5f, 0, 0 ),

        zpVector4f( 0.0f, 0.0f, 0, 0 ),
        zpVector4f( 0.5f, 0.0f, 0, 0 ),
        zpVector4f( 1.0f, 0.0f, 0, 0 ),
    };

    const zpVector4f& offset = offsets[ m_pivot ];
    return offset;
}

const zpMaterialResourceInstance& zpUIComponent::getMaterial() const
{
    return m_material;
}

void zpUIComponent::onCreate()
{

}
void zpUIComponent::onInitialize()
{

}
void zpUIComponent::onDestroy()
{

}

void zpUIComponent::onUpdate()
{

}
void zpUIComponent::onSimulate()
{

}

void zpUIComponent::onEnabled()
{
    m_isVisible = true;
}
void zpUIComponent::onDisabled()
{
    m_isVisible = false;
}

void zpUIComponent::onAnchor()
{

}

void zpUIComponent::getLocalCorners( zpVector4f& topLeft, zpVector4f& topRight, zpVector4f& bottomLeft, zpVector4f& bottomRight ) const
{
    const zpVector4f& offset = getPivotOffset();
    zpScalar x0, y0, x1, y1, w( (zp_float)m_width ), h( (zp_float)m_height ), nx, ny, ox( offset.getX() ), oy( offset.getY() );

    zpMath::Neg( nx, ox );
    zpMath::Neg( nx, oy );

    zpMath::Mul( x0, w, nx );
    zpMath::Mul( y0, h, ny );
    zpMath::Add( x1, x0, w );
    zpMath::Add( y1, y0, h );

    zpTransformComponent* attachment = getParentObject()->getComponents()->getTransformComponent();
    const zpMatrix4f& transform = attachment->getWorldTransform();
    zpMath::Mul( topLeft, zpVector4f( x0, y1 ), transform );
    zpMath::Mul( topRight, zpVector4f( x1, y1 ), transform );
    zpMath::Mul( bottomRight, zpVector4f( x1, y0 ), transform );
    zpMath::Mul( bottomLeft, zpVector4f( x0, y0 ), transform );
}
void zpUIComponent::getWorldCorners( zpVector4f& topLeft, zpVector4f& topRight, zpVector4f& bottomLeft, zpVector4f& bottomRight ) const
{
    const zpVector4f& offset = getPivotOffset();
    zpScalar x0, y0, x1, y1, w( (zp_float)m_width ), h((zp_float) m_height ), nx, ny, ox( offset.getX() ), oy( offset.getY() );

    zpMath::Neg( nx, ox );
    zpMath::Neg( nx, oy );

    zpMath::Mul( x0, w, nx );
    zpMath::Mul( y0, h, ny );
    zpMath::Add( x1, x0, w );
    zpMath::Add( y1, y0, h );

    zpTransformComponent* attachment = getParentObject()->getComponents()->getTransformComponent();
    const zpMatrix4f& transform = attachment->getWorldTransform();
    zpMath::Mul( topLeft, zpVector4f( x0, y1 ), transform );
    zpMath::Mul( topRight, zpVector4f( x1, y1 ), transform );
    zpMath::Mul( bottomRight, zpVector4f( x1, y0 ), transform );
    zpMath::Mul( bottomLeft, zpVector4f( x0, y0 ), transform );
}
void zpUIComponent::getDrawingDimensions( zpVector4f& topLeft, zpVector4f& topRight, zpVector4f& bottomLeft, zpVector4f& bottomRight ) const
{
    const zpVector4f& offset = getPivotOffset();
    zpScalar x0, y0, x1, y1, w( (zp_float)m_width ), h( (zp_float)m_height ), nx, ny, ox( offset.getX() ), oy( offset.getY() );

    zpMath::Neg( nx, ox );
    zpMath::Neg( nx, oy );

    zpMath::Mul( x0, w, nx );
    zpMath::Mul( y0, h, ny );
    zpMath::Add( x1, x0, w );
    zpMath::Add( y1, y0, h );

    zpScalar rx0, rx1, ry0, ry1;
    zpMath::Lerp( rx0, x0, x1, m_drawRegion.getX() );
    zpMath::Lerp( ry0, y0, y1, m_drawRegion.getY() );
    zpMath::Lerp( rx1, x0, x1, m_drawRegion.getZ() );
    zpMath::Lerp( ry1, y0, y1, m_drawRegion.getW() );

    topLeft = zpVector4f( rx0, ry1 );
    topRight = zpVector4f( rx1, ry1 );
    bottomRight = zpVector4f( rx1, ry0 );
    bottomLeft = zpVector4f( rx0, ry0 );
}

zpUIComponentPool::zpUIComponentPool()
{
}
zpUIComponentPool::~zpUIComponentPool()
{
}

void zpUIComponentPool::render( zpRenderingContext* context )
{
    const zpMaterialResource* material = ZP_NULL;
    zpUIComponent** b = m_used.begin();
    zpUIComponent** e = m_used.begin();

    for( ; b != e; ++b )
    {
        zpUIComponent* comp = *b;
        const zpMaterialResource* mat = comp->getMaterial().getResource();

        if( material != mat )
        {
            if( material != ZP_NULL )
            {
                context->endDrawImmediate();
            }

            material = mat;
            context->beginDrawImmediate( 0, ZP_RENDERING_QUEUE_UI, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, material );
        }

        comp->render( context );
    }

    if( material != ZP_NULL )
    {
        context->endDrawImmediate();
    }
}

void zpUIComponentPool::update()
{
    m_used.foreach( []( zpUIComponent* comp ) {
        comp->update();
    } );
}
void zpUIComponentPool::simulate()
{
    m_used.foreach( []( zpUIComponent* comp ) {
        comp->simulate();
    } );
}

#endif
