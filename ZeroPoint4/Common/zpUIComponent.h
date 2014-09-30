#pragma once
#ifndef ZP_UI_COMPONENT_H
#define ZP_UI_COMPONENT_H

enum zpUIComponentType
{
	ZP_UI_COMPONENT_TYPE_CONTAINER,
	ZP_UI_COMPONENT_TYPE_SPRITE,
	ZP_UI_COMPONENT_TYPE_LABEL,

	zpUIComponent_Count,
};

enum zpUIPivot
{
	ZP_UI_PIVOT_TOP_LEFT,
	ZP_UI_PIVOT_TOP,
	ZP_UI_PIVOT_TOP_RIGHT,
	ZP_UI_PIVOT_LEFT,
	ZP_UI_PIVOT_CENTER,
	ZP_UI_PIVOT_RIGHT,
	ZP_UI_PIVOT_BOTTOM_LEFT,
	ZP_UI_PIVOT_BOTTOM,
	ZP_UI_PIVOT_BOTTOM_RIGHT,
};

class zpUIComponent;

struct zpUIAnchor
{
	zpObject* target;
	zp_float relative;
	zp_int absolute;
};

class zpUIComponent : public zpComponent
{
public:
	zpUIComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpUIComponent();

	void render( zpRenderingContext* context );

	void setColor( const zpColor4f& color );
	void setColor( const zpColor4f& colorTop, const zpColor4f& colorBottom );
	void setColor( const zpColor4f& colorTopLeft, const zpColor4f& colorTopRight, const zpColor4f& colorBottomLeft, const zpColor4f& colorBottomRight );

	const zpVector4f& getPivotOffset() const;

	const zpMaterialResourceInstance& getMaterial() const;

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

	void onAnchor();

	void getLocalCorners( zpVector4f& topLeft, zpVector4f& topRight, zpVector4f& bottomLeft, zpVector4f& bottomRight ) const;
	void getWorldCorners( zpVector4f& topLeft, zpVector4f& topRight, zpVector4f& bottomLeft, zpVector4f& bottomRight ) const;

	void getDrawingDimensions( zpVector4f& topLeft, zpVector4f& topRight, zpVector4f& bottomLeft, zpVector4f& bottomRight ) const;

private:
	zpUIComponentType m_type;
	zpUIPivot m_pivot;

	zp_int m_width;
	zp_int m_height;
	zpVector2f m_localScale;
	zp_float m_localRotation;

	zpVector4f m_padding;
	zpVector4f m_boarder;
	zpVector4f m_drawRegion;

	zpColor4f m_color0;
	zpColor4f m_color1;
	zpColor4f m_color2;
	zpColor4f m_color3;
	zp_float m_alpha;

	zpString m_text;
	zp_float m_textHeight;

	zp_float m_filledValue;
	zp_int m_step;

	zp_int m_depth;

	zpUIAnchor m_leftAnchor;
	zpUIAnchor m_rightAnchor;
	zpUIAnchor m_topAnchor;
	zpUIAnchor m_bottomAnchor;

	zp_bool m_isChanged;
	zp_bool m_isVisible;

	zpMaterialResourceInstance m_material;
};

class zpUIComponentPool : public zpContentPool< zpUIComponent, 64 >
{
public:
	zpUIComponentPool();
	virtual ~zpUIComponentPool();

	void render( zpRenderingContext* context );

	void update();
	void simulate();
};

#endif
