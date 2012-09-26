#pragma once
#ifndef ZP_RENDERING_COMPONENT_H
#define ZP_RENDERING_COMPONENT_H

ZP_ABSTRACT_CLASS zpRenderingComponent : public zpComponent, public zpRenderable {
public:
	zpRenderingComponent();
	virtual ~zpRenderingComponent();
	
	const zpRenderLayer& getRenderLayers() const;

	void setVisible( zp_bool isVisible );
	zp_bool isVisible() const;

protected:
	virtual void onShow() = 0;
	virtual void onHide() = 0;

	void setRenderLayers( const zpRenderLayer& layers );

private:
	zp_bool m_isVisible;
	zpRenderLayer m_renderLayers;
};

#endif