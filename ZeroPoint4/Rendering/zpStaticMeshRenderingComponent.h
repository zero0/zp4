#pragma once
#ifndef ZP_STATIC_MESH_RENDERING_COMPONENT_H
#define ZP_STATIC_MESH_RENDERING_COMPONENT_H

class zpStaticMeshRenderingComponent : public zpComponent, public zpRenderable {
public:
	zpStaticMeshRenderingComponent();
	virtual ~zpStaticMeshRenderingComponent();

	void render();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:

	zpResourceInstance<zpStaticMeshResource> m_mesh;

	zpMatrix4f m_localTransform;
};

#endif