#pragma once
#ifndef ZP_CANVAS_COMPONENT_H
#define ZP_CANVAS_COMPONENT_H

enum zpCanvasRenderType
{
	ZP_CANVAS_RENDER_TYPE_2D_UI,
	ZP_CANVAS_RENDER_TYPE_3D_UI,
	ZP_CANVAS_RENDER_TYPE_WORLD,
};

struct zpCanvasDrawCall
{
	zpMaterialResourceInstance material;
	zpArrayList< zpVector4f > pos;
	zpArrayList< zpColor4f > cols;
	zpArrayList< zpVector2f > uvs;
};

class zpCanvasComponent : public zpComponent
{
public:
	zpCanvasComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpCanvasComponent();

	void render( zpRenderingContext* context );

	void addWidget( zpWidgetComponent* w );
	void removeWidget( zpWidgetComponent* w );
	void removeAllWidgets();

	
protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	void getWidgets( zpTransformComponent* t, zpArrayList< zpWidgetComponent* >& widgets ) const;

	zp_int m_layer;
	zpColor4f m_backgroundColor;

	zpRenderingQueue m_queue;
	zpCanvasRenderType m_canvasType;

	zpMaterialResourceInstance m_material;

	zpArrayList< zpWidgetComponent* > m_widgets;
};

class zpCanvasComponentPool : public zpContentPool< zpCanvasComponent, 8 >
{
public:
	zpCanvasComponentPool();
	virtual ~zpCanvasComponentPool();

	void render( zpRenderingContext* context );

	void update();
};

#endif
