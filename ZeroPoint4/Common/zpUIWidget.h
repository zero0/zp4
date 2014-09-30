#pragma once
#ifndef ZP_UI_WIDGET_H
#define ZP_UI_WIDGET_H

enum zpUIWidgetPivot
{
	ZP_UI_WIDGET_PIVOT_TOP_LEFT,
	ZP_UI_WIDGET_PIVOT_TOP,
	ZP_UI_WIDGET_PIVOT_TOP_RIGHT,
	ZP_UI_WIDGET_PIVOT_TOP_LEFT,
	ZP_UI_WIDGET_PIVOT_LEFT,
	ZP_UI_WIDGET_PIVOT_CENTER,
	ZP_UI_WIDGET_PIVOT_RIGHT,
	ZP_UI_WIDGET_PIVOT_BOTTOM_LEFT,
	ZP_UI_WIDGET_PIVOT_BOTTOM,
	ZP_UI_WIDGET_PIVOT_BOTTOM_RIGHT,
};

ZP_ABSTRACT_CLASS zpUIWidget
{
public:
	virtual ~zpUIWidget() {}

	void start();
	void enabled();
	void update();
	void disabled();
	void destroyed();

protected:
	virtual void onStart() = 0;
	virtual void onEnabled() = 0;
	virtual void onUpdate() = 0;
	virtual void onDisabled() = 0;
	virtual void onDestroyed() = 0;

private:
	zpRectf m_rect;
	zpVector2f m_localScale;
	zp_float m_localRotation;

	zpVector4f m_padding;
	zpVector4f m_boarder;

	zpColor4f m_color;
	zpUIWidgetPivot m_pivot;
	zp_int m_depth;

	zp_bool m_isChanged;
};

#endif
