#include "zpDX11.h"
#include <D3DX11.h>

zpDepthStencilBufferImpl::zpDepthStencilBufferImpl()
	: m_depthStencilView( ZP_NULL )
	, m_depthTexture( ZP_NULL )
	, m_format( ZP_DISPLAY_FORMAT_UNKNOWN )
	, m_width( 0 )
	, m_height( 0 )
{}
zpDepthStencilBufferImpl::~zpDepthStencilBufferImpl()
{
	ZP_SAFE_RELEASE( m_depthStencilView );
	ZP_SAFE_RELEASE( m_depthTexture );
}

zp_uint zpDepthStencilBufferImpl::getWidth() const
{
	return m_width;
}
zp_uint zpDepthStencilBufferImpl::getHeight() const
{
	return m_height;
}

zpDisplayFormat zpDepthStencilBufferImpl::getDisplayFormat() const
{
	return m_format;
}
