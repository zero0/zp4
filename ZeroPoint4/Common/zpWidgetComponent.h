#pragma once
#ifndef ZP_WIDGET_COMPONENT_H
#define ZP_WIDGET_COMPONENT_H

enum zpWidgetType
{
    ZP_WIDGET_CONTAINER,
    ZP_WIDGET_SPRITE,
    ZP_WIDGET_LABEL,
};

class zpWidgetComponent : public zpComponent
{
public:
    zpWidgetComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpWidgetComponent();

    void fill( zpRenderingContext* context );

    void setColor( const zpColor4f& color );
    void setColor( const zpColor4f& topColor, const zpColor4f& bottomColor );
    void setColor( const zpColor4f& bottomLeft, const zpColor4f& topLeft, const zpColor4f& topRight, const zpColor4f& bottomRight );

    zp_float getAlpha() const;
    void setAlpha( zp_float a );

    const zpString& getText() const;
    void setText( const zpString& text );
    void setText( const zp_char* text );

    zp_int getDepth() const;
    void setDepth( zp_int depth );

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    void addToCanvas();
    void removeFromCanvas();

    void fillSprite( zpRenderingContext* context );
    void fillLabel( zpRenderingContext* context );

    zp_bool m_isAdded;
    zpCanvasComponent* m_canvas;

    zpMaterialResourceInstance m_material;

    zp_int m_depth;

    zpWidgetType m_type;

    zpVector4f m_border;

    zpColor4f m_colorBottomLeft;
    zpColor4f m_colorTopLeft;
    zpColor4f m_colorTopRight;
    zpColor4f m_colorBottomRight;

    zp_float m_textHeight;
    zpFontAlignment m_textAlignment;
    zpString m_text;
};

class zpWidgetComponentPool : public zpContentPool< zpWidgetComponent, 8 >
{
public:
    zpWidgetComponentPool();
    virtual ~zpWidgetComponentPool();

    void render( zpRenderingContext* context );

    void update( zp_float deltaTime, zp_float realTime );
};

#endif
