#pragma once
#ifndef ZP_UI_RENDERING_COMPONENT_H
#define ZP_UI_RENDERING_COMPONENT_H

#if 0
class zpUIRenderingComponent : public zpRenderingComponent {
public:
	zpUIRenderingComponent();
	virtual ~zpUIRenderingComponent();

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

	void onShow();
	void onHide();

private:
	void* m_webView;
	zpTexture* m_texture;
	zpVector2i m_size;

	zpString m_uiAlias;
	zpResourceInstance<zpUIResource> m_ui;

	zpResourceInstance<zpShaderResource> m_uiShader;

	zpImmediateBuffer<zpVertexPositionNormalUV, zp_short> m_uiVertexBuffer;
};
#endif

#endif