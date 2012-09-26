#include "zpRendering.h"

zpRenderingComponent::zpRenderingComponent() 
	: m_isVisible( true )
	, m_renderLayers( 0 )
{}
zpRenderingComponent::~zpRenderingComponent() {}

const zpRenderLayer& zpRenderingComponent::getRenderLayers() const {
	return m_renderLayers;
}

void zpRenderingComponent::setVisible( zp_bool isVisible ) {
	zp_bool wasVisible = m_isVisible;
	m_isVisible = isVisible;

	if( wasVisible && !isVisible ) {
		onHide();
	} else if( !wasVisible && isVisible ) {
		onShow();
	}
}
zp_bool zpRenderingComponent::isVisible() const {
	return m_isVisible;
}

void zpRenderingComponent::setRenderLayers( const zpRenderLayer& layers ) {
	m_renderLayers = layers;
}
