#pragma once
#ifndef ZP_RENDERING_MANAGER_H
#define ZP_RENDERING_MANAGER_H

#if 0
class zpRenderingManager : public zpGameManager {
public:
	zpRenderingManager();
	virtual ~zpRenderingManager();

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	zpRenderingEngine* getRenderingEngine() const;

	void receiveMessage( const zpMessage& message );

	zp_bool addRenderingComponent( zpRenderingComponent* component );
	zp_bool removeRenderingComponent( zpRenderingComponent* component );

	zp_bool addLightComponent( zpLightComponent* light );
	zp_bool removeLightComponent( zpLightComponent* light );

	void renderLayer( zp_uint layer );
	void renderLights();

	zpBuffer* getGlobalBuffer( zp_uint index );

	void setFrustumCullingEnabled( zp_bool frustumCulling );
	zp_bool isFrustumCullingEnabled() const;

	void setCamera( zpCamera* camer );
	zpCamera* getCamera() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zp_bool m_isFrustimCullingEnabled;

	zpRenderingEngine* m_engine;

	zpCamera* m_currentCamera;

	zpArrayList<zpLightComponent*> m_lightComponents;
	zpArray<zpArrayList<zpRenderingComponent*>, 4> m_renderingComponents;
	zpArray<zpBuffer*, ZP_RENDERING_GLOBAL_BUFFER_Count> m_globalBuffers;
};
#endif

#endif