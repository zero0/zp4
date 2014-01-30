#pragma once
#ifndef ZP_MESH_RENDERER_COMPONENT_H
#define ZP_MESH_RENDERER_COMPONENT_H

class zpMeshRendererComponent : public zpComponent
{
public:
	zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpMeshRendererComponent();

	void render();

	void setRenderLayer( zp_uint layer );
	zp_uint getRenderLayer() const;

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_uint m_layer;
	zpMeshResourceInstance m_mesh;
};


class zpMeshRendererComponentPool : public zpContentPool< zpMeshRendererComponent, 8 >
{
public:
	zpMeshRendererComponentPool();
	virtual ~zpMeshRendererComponentPool();

	void update();
	void simulate();

	void render();
};

#endif