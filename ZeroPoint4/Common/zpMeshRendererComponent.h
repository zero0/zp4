#pragma once
#ifndef ZP_MESH_RENDERER_COMPONENT_H
#define ZP_MESH_RENDERER_COMPONENT_H

class zpMeshRendererComponent : public zpComponent
{
public:
	zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpMeshRendererComponent();

	void render();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
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