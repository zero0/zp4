#pragma once
#ifndef ZP_DEFERRED_RENDERING_COMPONENT_H
#define ZP_DEFERRED_RENDERING_COMPONENT_H

#if 0
enum zpGBufferTarget {
	ZP_GBUFFER_TARGET_DIFFUSE,
	ZP_GBUFFER_TARGET_NORMAL,
	ZP_GBUFFER_TARGET_DEPTH,
	ZP_GBUFFER_TARGET_LIGHT,

	ZP_GBUFFER_TARGET_Count,
};

class zpDeferredRenderingComponent : public zpComponent, public zpRenderable {
public:
	zpDeferredRenderingComponent();
	virtual ~zpDeferredRenderingComponent();

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
	zpRenderingManager* m_renderingManager;
	zpRenderingEngine* m_renderingEngine;
	zpContentManager* m_contentManager;

	zpBuffer* m_screenBuffer;

	zpArray<zpTexture*, ZP_GBUFFER_TARGET_Count> m_gbuffer;
	zpArray<zpColor4f, ZP_GBUFFER_TARGET_Count> m_clearColors;

	zpSamplerState* m_pointSampler;
	zpSamplerState* m_linearSampler;

	zpResourceInstance<zpShaderResource> m_fullscreenShader;
	zpResourceInstance<zpTextureResource> m_texture;

	zpViewport m_viewport;
};
#endif

#endif