#pragma once
#ifndef ZP_ANIMATED_MESH_RENDERER_COMPONENT_H
#define ZP_ANIMATED_MESH_RENDERER_COMPONENT_H

class zpAnimatedMeshRendererComponent : public zpComponent
{
public:
	zpAnimatedMeshRendererComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpAnimatedMeshRendererComponent();

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
	zpSkeletonResourceInstance m_skeleton;
	zpAnimationResourceInstance m_animation;

	zpAnimationController m_controller;

	zpMaterialResourceInstance m_material;
};


class zpAnimatedMeshRendererComponentPool : public zpContentPool< zpAnimatedMeshRendererComponent, 8 >
{
public:
	zpAnimatedMeshRendererComponentPool();
	virtual ~zpAnimatedMeshRendererComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
	void simulate();

	void render( zpRenderingContext* i );
};

#endif