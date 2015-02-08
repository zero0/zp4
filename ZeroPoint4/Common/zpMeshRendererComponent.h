#pragma once
#ifndef ZP_MESH_RENDERER_COMPONENT_H
#define ZP_MESH_RENDERER_COMPONENT_H

class zpMeshRendererComponent : public zpComponent
{
public:
	zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpMeshRendererComponent();

	void render( zpRenderingContext* i );

	void setRenderLayer( zp_uint layer );
	zp_uint getRenderLayer() const;

	zp_bool hasMaterialOverride() const;
	void setMaterialOverride( const zp_char* materialFile );
	void resetMaterialOverride();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_uint m_layer;

	zpMeshResourceInstance m_mesh;
	zpArrayList< zpMaterialResourceInstance > m_materials;
};


class zpMeshRendererComponentPool : public zpContentPool< zpMeshRendererComponent, 8 >
{
public:
	zpMeshRendererComponentPool();
	virtual ~zpMeshRendererComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
	void simulate();

	void render( zpRenderingContext* i );
};

#endif
