#pragma once
#ifndef ZP_UI_CANVAS_COMPONENT_H
#define ZP_UI_CANVAS_COMPONENT_H

enum zpUIWidgetEvent
{
    ZP_UI_WIDGET_EVENT_ON_ALIGNMENT,

    ZP_UI_WIDGET_EVENT_ON_HOVER_ENTER,
    ZP_UI_WIDGET_EVENT_ON_HOVER_EXIT,

    ZP_UI_WIDGET_EVENT_ON_PRESS,
    ZP_UI_WIDGET_EVENT_ON_RELEASE,
    ZP_UI_WIDGET_EVENT_ON_CLICK,

    ZP_UI_WIDGET_EVENT_ON_SHOWN,
    ZP_UI_WIDGET_EVENT_ON_HIDDEN,

    ZP_UI_WIDGET_EVENT_ON_ENABLED,
    ZP_UI_WIDGET_EVENT_ON_DISABLED,

    ZP_UI_WIDGET_EVENT_ON_DESTROYED,

    zpUIWidgetEvent_Count
};

enum zpUIWidgetAnchorSide
{
    ZP_UI_WIDGET_ANCHOR_SIDE_LEFT,
    ZP_UI_WIDGET_ANCHOR_SIDE_BOTTOM,
    ZP_UI_WIDGET_ANCHOR_SIDE_RIGHT,
    ZP_UI_WIDGET_ANCHOR_SIDE_TOP,

    zpUIWidgetAnchorSide_Count
};

enum zpUIWidgetLockAspectRatio
{
    ZP_UI_WIDGET_LOCK_ASPECT_RATIO_FREE,
    ZP_UI_WIDGET_LOCK_ASPECT_RATIO_BASED_ON_WIDTH,
    ZP_UI_WIDGET_LOCK_ASPECT_RATIO_BASED_ON_HEIGHT,
};

enum zpUIWidgetPivot
{
    ZP_UI_WIDGET_PIVOT_BOTTOM_LEFT,
    ZP_UI_WIDGET_PIVOT_BOTTOM,
    ZP_UI_WIDGET_PIVOT_BOTTOM_RIGHT,

    ZP_UI_WIDGET_PIVOT_LEFT,
    ZP_UI_WIDGET_PIVOT_CENTER,
    ZP_UI_WIDGET_PIVOT_RIGHT,

    ZP_UI_WIDGET_PIVOT_TOP_LEFT,
    ZP_UI_WIDGET_PIVOT_TOP,
    ZP_UI_WIDGET_PIVOT_TOP_RIGHT,
};

enum
{
    ZP_UI_WIDGET_UNLINKED = -2,
    ZP_UI_WIDGET_INVALID_ID = -1,
    ZP_UI_WIDGET_ROOT_ID = 0,
};

struct zpUIDrawCall
{
    zpMaterialResourceInstance material;
    zpArrayList< zpVector4f > pos;
    zpArrayList< zpColor4f > cols;
    zpArrayList< zpVector2f > uvs;
};

struct zpUIWidget;

typedef void(*zpUIWidgetEventListener)( zpUIWidget* widgent, zpUIWidgetEvent e, void* data );
typedef void(*zpUIWIdgetFillRender)( zpUIWidget* widget, zpUIDrawCall* drawCall );

struct zpUIWidgetAnchor
{
    zpUIWidgetAnchor();
    ~zpUIWidgetAnchor();

    zp_int widget;
    zp_float relative;
    zp_int absolute;
};

struct zpUIWidget
{
    zpUIWidget();
    ~zpUIWidget();

    zp_int id;
    zpString name;
    zpString tag;

    zp_bool isVisible;
    zp_bool isEnabled;
    zp_uint frameUpdated;
    zp_float aspectRatio;
    zpUIWidgetLockAspectRatio aspectRatioLock;
    zpUIWidgetPivot pivot;

    zpMatrix4f transform;
    zpMatrix4f worldTransform;

    zpRectf uv;
    zp_float border[4];
    zp_float drawRegion[4];

    zpVector2f offset;
    zp_int width;
    zp_int height;
    zp_int minWidth;
    zp_int minHeight;

    zp_size_t depth;
    zp_float alpha;

    zpColor4f colorBottomLeft;
    zpColor4f colorTopLeft;
    zpColor4f colorTopRight;
    zpColor4f colorBottomRight;

    zp_int parent;
    zpArrayList< zp_int > children;

    zpMaterialResourceInstance material;

    zpUIWIdgetFillRender fillRenderer;
    zpUIWidgetEventListener listeners[ zpUIWidgetEvent_Count ];
    zpUIWidgetAnchor anchors[ zpUIWidgetAnchorSide_Count ];
};

class zpUICanvasComponent : public zpComponent
{
public:
    zpUICanvasComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpUICanvasComponent();

    void render( zpRenderingContext* context );

    zp_int addWidget( zp_int parent );

    void removeWidget( zp_int w );
    void removeWidgetChildren( zp_int w );
    void removeAllWidgets();

    void addWidgetChild( zp_int parent, zp_int child );
    void removeWidgetChild( zp_int parent, zp_int child );

    const zpString& getWidgetName( zp_int w ) const;
    void setWidgetName( zp_int w, const zp_char* name );

    const zpVector4f& getWidgetLocalPosition( zp_int w ) const;
    void setWidgetLocalPosition( zp_int w, zpVector4fParamF position );

    zpVector2i getWidgetSize( zp_int w ) const;
    zp_int getWidgetWidth( zp_int w ) const;
    zp_int getWidgetHeight( zp_int w ) const;
    void setWidgetWidth( zp_int w, zp_int width );
    void setWidgetHeight( zp_int w, zp_int height );

    void setWidgetColor( zp_int w, const zpColor4f& color );

    zp_float getWidgetAlpha( zp_int w ) const;
    void setWidgetAlpha( zp_int w, zp_float a );

    void setWidgetMaterial( zp_int w, const zpMaterialResourceInstance& material );

    void setWidgetFill( zp_int w, zpUIWIdgetFillRender fill );
    void setWidgetListener( zp_int w, zpUIWidgetEvent e, zpUIWidgetEventListener listener );

    void sendWidgetEvent( zp_int w, zpUIWidgetEvent e, void* data );

    void setWidgetVisible( zp_uint w, zp_bool visible );
    zp_bool isWidgetVisible( zp_int w ) const;
    zp_bool isWidgetVisibleInHeirachy( zp_int w ) const;

    void setWidgetEnabled( zp_uint w, zp_bool enabled );
    zp_bool isWidgetEnabled( zp_int w ) const;

    void setWidgetAlignmentUniform( zp_int w, zp_int anchor, zp_int left, zp_int bottom, zp_int right, zp_int top );
    void setWidgetAlignmentLeft( zp_int w, zp_int anchor, zp_float relative, zp_int absolute );
    void setWidgetAlignmentRight( zp_int w, zp_int anchor, zp_float relative, zp_int absolute );
    void setWidgetAlignmentBottom( zp_int w, zp_int anchor, zp_float relative, zp_int absolute );
    void setWidgetAlignmentTop( zp_int w, zp_int anchor, zp_float relative, zp_int absolute );

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    zpUIWidget* getWidget( zp_int w );
    const zpUIWidget* getWidget( zp_int w ) const;

    void updateWidget( zp_int w, zp_uint frameCount );
    void updateWidgetAnchors( zpUIWidget* w );

    void sortWidget( zp_uint w );
    void clearDrawCalls();
    void fillWidget( zp_int w );

    void createUIFromData( const zpBison::Value& def );
    void createWidgetFromData( const zpBison::Value& def, zp_int parent );

    zp_uint m_layer;
    zp_int m_ids;

    zpVector2i m_screenSize;

    zpArrayList< zpUIWidget > m_widgets;
    zpArrayList< zpUIDrawCall > m_drawCalls;
};

class zpUICanvasComponentPool : public zpContentPool< zpUICanvasComponent, 8 >
{
public:
    zpUICanvasComponentPool();
    virtual ~zpUICanvasComponentPool();

    void render( zpRenderingContext* context );

    void update( zp_float deltaTime, zp_float realTime );
};

#endif
