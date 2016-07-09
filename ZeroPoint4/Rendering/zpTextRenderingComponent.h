#pragma once
#ifndef ZP_TEXT_RENDERING_COMPONENT_H
#define ZP_TEXT_RENDERING_COMPONENT_H

enum zpTextAlignment {
    ZP_TEXT_ALIGNMENT_TOP_LEFT = 1,
    ZP_TEXT_ALIGNMENT_TOP_CENTER,
    ZP_TEXT_ALIGNMENT_TOP_RIGHT,

    ZP_TEXT_ALIGNMENT_LEFT,
    ZP_TEXT_ALIGNMENT_CENTER,
    ZP_TEXT_ALIGNMENT_RIGHT,

    ZP_TEXT_ALIGNMENT_BOTTOM_LEFT,
    ZP_TEXT_ALIGNMENT_BOTTOM_CENTER,
    ZP_TEXT_ALIGNMENT_BOTTOM_RIGHT,
};

#if 0
class zpTextRenderingComponent : public zpRenderingComponent {
public:
    zpTextRenderingComponent();
    virtual ~zpTextRenderingComponent();

    void render();

    void receiveMessage( const zpMessage& message );

    void serialize( zpSerializedOutput* out );
    void deserialize( zpSerializedInput* in );

    void setText( const zpString& text );
    const zpString& getText() const;

protected:
    void onCreate();
    void onDestroy();

    void onUpdate();

    void onEnabled();
    void onDisabled();

    void onShow();
    void onHide();

private:
    zpTextAlignment m_alignment;
    zp_bool m_isLocalToGameObject;
    zp_bool m_isTextDirty;
    
    zpVector2f m_size;
    zpMatrix4f m_localTransform;
    zpString m_text;
    zpString m_fontAlias;
    zpString m_shaderAlias;

    zpResourceInstance<zpFontResource> m_font;
    zpResourceInstance<zpShaderResource> m_shader;

    zpImmediateBuffer<zpVertexPositionNormalUV, zp_ushort> m_buffer;
};
#endif

#endif