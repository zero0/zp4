#pragma once
#ifndef ZP_RENDERING_MANAGER_H
#define ZP_RENDERING_MANAGER_H

class zpRenderingManager : public zpGameManager, public zpRenderable {
public:
	zpRenderingManager();
	virtual ~zpRenderingManager();

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	zpRenderingEngine* getRenderingEngine() const;

	void receiveMessage( const zpMessage& message );

	void render();

	zp_bool addRenderingComponent( zpRenderingComponent* component );
	zp_bool removeRenderingComponent( zpRenderingComponent* component );

	void renderLayer( zp_uint layer );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpRenderingEngine* m_engine;

	zpCamera* m_currentCamera;
	zpArrayList<zpCameraComponent*> m_cameraStack;

	zpArray<zpArrayList<zpRenderingComponent*>, ZP_RENDERING_LAYER_COUNT> m_renderingComponents;

};

#endif