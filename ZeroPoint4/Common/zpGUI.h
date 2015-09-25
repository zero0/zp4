#pragma once
#ifndef ZP_GUI_H
#define ZP_GUI_H

struct zpGUIStyle
{

};

enum zpGUIWidgetType
{
	ZP_GUI_WIDGET_CONTAINER = 1,
	ZP_GUI_WIDGET_BUTTON,
	ZP_GUI_WIDGET_LABEL,
};

enum
{
	ZP_GUI_NUM_WIDGETS = 256
};

class zpGUI
{
public:

	void create();
	void setup();
	void teardown();
	void destroy();

	void beginWindow( const zp_char* title, const zpRectf& rect, zpRectf& outPos );
	void endWindow();

	void box( zp_float widthPercent, zp_float size, const zpColor4f& color );

	void label( zp_float size, const zp_char* text, const zpColor4f& color );

	zp_bool button( zp_float size, const zp_char* text );

	zp_bool text( zp_float size, const zp_char* text, const zpColor4f& color, zp_char* outText );

	void setApplication( zpApplication* app ) { m_application = app; }
	zpApplication* getApplication() const { return m_application; }

	const zpColor4f& getMainColor() const { return m_mainColor; }
	void setMainColor( const zpColor4f& color ) { m_mainColor = color; }

	const zpColor4f& getBackgroundColor() const { return m_backgroundColor; }
	void setBackgroundColor( const zpColor4f& color ) { m_backgroundColor = color; }

	void startGUI();
	void endGUI();

	void setMargin( zp_float w, zp_float h );

private:
	struct zpGUIWidget
	{
		zpGUIWidgetType type;

		zpRectf localRect;
		zpColor4f color;
		zpString text;

		zpGUIWidget* parent;
		zpArrayList< zpGUIWidget* > children;
	};

	zpGUIWidget* addWidget( zp_float widthPercent, zp_float height );
	zpGUIWidget* addChildWidget( zp_float widthPercent, zp_float height, zpGUIWidget* parent );

	void drawWidgets( zpRenderingContext* context );
	void drawWidget( zpRenderingContext* context, zpGUIWidget* widget );

	void getWorldRect( zpGUIWidget* widget, zpRectf& outWorldRect );
	zp_bool isMouseOverWidget( zpGUIWidget* widget, zpRectf& outWorldRect, zpVector2i& mousePos, zpVector2i& mouseDelta, zp_bool& isDown, zp_bool& isPressed );

	zp_bool m_isDrawingWidgets;
	zpGUIWidget* m_currentlySelected;

	zpApplication* m_application;

	zpGUIWidget* m_selectedWidget;
	zpFixedArrayList< zpGUIWidget, ZP_GUI_NUM_WIDGETS > m_allWidgets;
	zpFixedArrayList< zpGUIWidget*, ZP_GUI_NUM_WIDGETS > m_freeWidgets;
	zpFixedArrayList< zpGUIWidget*, ZP_GUI_NUM_WIDGETS > m_usedWidgets;

	zpFixedArrayList< zpGUIWidget*, 8 > m_widgetStack;

	zpColor4f m_mainColor;
	zpColor4f m_backgroundColor;
	zpVector2f m_margin;

	zpMaterialResourceInstance m_guiMaterial;
	zpFontResourceInstance m_guiFont;
};

#endif
