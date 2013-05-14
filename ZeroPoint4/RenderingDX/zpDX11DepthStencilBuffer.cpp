#include "zpDX11.h"
#include <D3DX11.h>

zpDepthStencilBufferImpl::zpDepthStencilBufferImpl()
	: m_depthStencilView( ZP_NULL )
	, m_depthTexture( ZP_NULL )
	, m_format( ZP_DISPLAY_FORMAT_UNKNOWN )
	, m_width( 0 )
	, m_height( 0 )
{}
zpDepthStencilBufferImpl::zpDepthStencilBufferImpl( zpDisplayFormat format, ID3D11Texture2D* texture, ID3D11DepthStencilView* view, zp_uint width, zp_uint height )
	: m_depthStencilView( view )
	, m_depthTexture( texture )
	, m_format( format )
	, m_width( width )
	, m_height( height )
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

ID3D11DepthStencilView* zpDepthStencilBufferImpl::getDepthStencilView()
{
	return m_depthStencilView;
}
ID3D11Texture2D* zpDepthStencilBufferImpl::getDepthTexture()
{
	return m_depthTexture;
}