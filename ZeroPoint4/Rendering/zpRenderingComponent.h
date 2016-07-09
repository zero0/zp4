#pragma once
#ifndef ZP_RENDERING_COMPONENT_H
#define ZP_RENDERING_COMPONENT_H

#if 0
ZP_ABSTRACT_CLASS zpRenderingComponent : public zpComponent, public zpRenderable {
public:
    zpRenderingComponent();
    virtual ~zpRenderingComponent();
    
    const zpRenderLayer& getRenderLayers() const;
    const zpBoundingSphere& getBoundingSphere() const;

    void setVisible( zp_bool isVisible );
    zp_bool isVisible() const;

protected:
    virtual void onShow() = 0;
    virtual void onHide() = 0;

    void setRenderLayers( const zpRenderLayer& layers );
    void setBoundingSphere( const zpBoundingSphere& sphere );

    zp_bool m_isVisible;
    zpRenderLayer m_renderLayers;
    zpBoundingSphere m_boundingSphere;
};
#endif

#endif