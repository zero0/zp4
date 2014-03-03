#pragma once
#ifndef ZP_GUI_H
#define ZP_GUI_H

struct zpGUIStyle
{

};

enum
{
	ZP_GUI_NUM_WIDGETS = 64
};

class zpGUI
{
public:

	void create();
	void destroy();

	void beginWindow( const zp_char* title, const zpRectf& rect, zpRectf& outPos );
	void endWindow();

	void label( const zp_char* text );

	zp_bool button( const zp_char* text );

	zp_bool text( const zp_char* text, zp_char* outText );

	void setApplication( zpApplication* app ) { m_application = app; }
	zpApplication* getApplication() const { return m_application; }

	const zpColor4f& getMainColor() const { return m_mainColor; }
	void setMainColor( const zpColor4f& color ) { m_mainColor = color; }

	const zpColor4f& getBackgroundColor() const { return m_backgroundColor; }
	void setBackgroundColor( const zpColor4f& color ) { m_backgroundColor = color; }

	void startGUI();
	void endGUI();

private:
	struct zpGUIWidget
	{
		zpRectf localRect;
		zpGUIWidget* parent;
		zpArrayList< zpGUIWidget* > children;
	};

	zpGUIWidget* addWidget( zp_float height );

	void getWorldRect( zpGUIWidget* widget, zpRectf& outWorldRect ) const;
	zp_bool isMouseOverWidget( zpGUIWidget* widget, zpRectf& outWorldRect, zp_bool& isDown ) const;

	zpApplication* m_application;
	zpRenderingContext* m_renderingContext;

	zpGUIWidget* m_selectedWidget;
	zpFixedArrayList< zpGUIWidget, ZP_GUI_NUM_WIDGETS > m_allWidgets;
	zpFixedArrayList< zpGUIWidget*, ZP_GUI_NUM_WIDGETS > m_widgetStack;

	zpColor4f m_mainColor;
	zpColor4f m_backgroundColor;

	zpMaterialResourceInstance m_guiMaterial;
};

#endif
