#include "zpRendering.h"

#include "zpRenderingImpl.inl"

zpDepthStencilBuffer::zpDepthStencilBuffer()
    : m_impl( ZP_NULL )
{}
zpDepthStencilBuffer::~zpDepthStencilBuffer()
{
    m_impl = ZP_NULL;
}

zp_uint zpDepthStencilBuffer::getWidth() const
{
    return m_impl->getWidth();
}
zp_uint zpDepthStencilBuffer::getHeight() const
{
    return m_impl->getHeight();
}

zpDisplayFormat zpDepthStencilBuffer::getDisplayFormat() const
{
    return m_impl->getDisplayFormat();
}

zpDepthStencilBufferImpl* zpDepthStencilBuffer::getDepthStencilBufferImpl() const
{
    return m_impl;
}
