#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"
#include "Rendering/zpRendering.h"

#define MAX_SET_BUFFER_VERTEX_COUNT    8

zpRenderingContextImpl::zpRenderingContextImpl()
    : m_context( ZP_NULL )
{}
zpRenderingContextImpl::~zpRenderingContextImpl()
{
    ZP_SAFE_RELEASE( m_context );
}

void zpRenderingContextImpl::setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture* const* targets, zpDepthStencilBuffer* depthStencilBuffer )
{
    ID3D11RenderTargetView* rtvs[ ZP_RENDER_TARGET_MAX_COUNT ];
    ID3D11DepthStencilView* d = depthStencilBuffer == ZP_NULL ? ZP_NULL : depthStencilBuffer->getDepthStencilBufferImpl()->m_depthStencilView;

    zpTexture* t;
    for( zp_uint i = 0; i < count; ++i )
    {
        t = targets[ i ];
        rtvs[ i ] = t == ZP_NULL ? ZP_NULL : t->getTextureImpl()->m_textureRenderTarget;
    }

    m_context->OMSetRenderTargets( count, rtvs, d );
}

void zpRenderingContextImpl::clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor )
{
    zpTextureImpl* t = renderTarget->getTextureImpl();
    m_context->ClearRenderTargetView( t->m_textureRenderTarget, clearColor.asFloat4() );
}
void zpRenderingContextImpl::clearDepthStencilBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_float clearDepth, zp_uint clearStencil )
{
    ID3D11DepthStencilView* d = depthStencilBuffer == ZP_NULL ? ZP_NULL : depthStencilBuffer->getDepthStencilBufferImpl()->m_depthStencilView;
    m_context->ClearDepthStencilView( d, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil );
}
void zpRenderingContextImpl::clearDepthBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_float clearDepth )
{
    ID3D11DepthStencilView* d = depthStencilBuffer == ZP_NULL ? ZP_NULL : depthStencilBuffer->getDepthStencilBufferImpl()->m_depthStencilView;
    m_context->ClearDepthStencilView( d, D3D11_CLEAR_DEPTH, clearDepth, 0 );
}
void zpRenderingContextImpl::clearStencilBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_uint clearStencil )
{
    ID3D11DepthStencilView* d = depthStencilBuffer == ZP_NULL ? ZP_NULL : depthStencilBuffer->getDepthStencilBufferImpl()->m_depthStencilView;
    m_context->ClearDepthStencilView( d, D3D11_CLEAR_STENCIL, 0.0f, clearStencil );
}
void zpRenderingContextImpl::clearState()
{
    m_prevMaterial = ZP_NULL;
    m_context->ClearState();
}
void zpRenderingContextImpl::flush()
{
    m_context->Flush();
}

void zpRenderingContextImpl::setViewport( const zpViewport& viewport )
{
    D3D11_VIEWPORT vp;
    vp.TopLeftX = viewport.topX;
    vp.TopLeftY = viewport.topY;
    vp.Width    = viewport.width;
    vp.Height   = viewport.height;
    vp.MinDepth = viewport.minDepth;
    vp.MaxDepth = viewport.maxDepth;

    m_context->RSSetViewports( 1, &vp );
}
void zpRenderingContextImpl::setScissorRect( const zpRecti& rect )
{
    D3D11_RECT r;
    r.left   = rect.getLeft();
    r.top    = rect.getTop();
    r.right  = rect.getRight();
    r.bottom = rect.getBottom();

    m_context->RSSetScissorRects( 1, &r );
}

void zpRenderingContextImpl::setRasterState( zpRasterState* raster )
{
    ID3D11RasterizerState* state = raster == ZP_NULL ? ZP_NULL : raster->getRasterStateImpl()->m_raster;

    m_context->RSSetState( state );
}
void zpRenderingContextImpl::setDepthStencilState( zpDepthStencilState* depthStencil, zp_uint stencilRef )
{
    ID3D11DepthStencilState* state = depthStencil == ZP_NULL ? ZP_NULL : depthStencil->getDepthStencilStateImpl()->m_depthStencil;

    m_context->OMSetDepthStencilState( state, stencilRef );
}
void zpRenderingContextImpl::setBlendState( zpBlendState* blend, zp_float blendFactor[4], zp_uint sampleMask )
{
    ID3D11BlendState* state = blend == ZP_NULL ? ZP_NULL : blend->getBlendStateImpl()->m_blend;

    m_context->OMSetBlendState( state, blendFactor, sampleMask );
}

void zpRenderingContextImpl::setSamplerState( zp_uint bindSlots, zp_uint index, zpSamplerState* sampler )
{
    ID3D11SamplerState* state = sampler == ZP_NULL ? ZP_NULL : sampler->getSamplerStateImpl()->m_sampler;;

    if( bindSlots & ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER )
    {
        m_context->VSSetSamplers( index, 1, &state );
    }
    if( bindSlots & ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER )
    {
        m_context->GSSetSamplers( index, 1, &state );
    }
    if( bindSlots & ZP_RESOURCE_BIND_SLOT_COMPUTE_SHADER )
    {
        m_context->CSSetSamplers( index, 1, &state );
    }
    if( bindSlots & ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER )
    {
        m_context->PSSetSamplers( index, 1, &state );
    }
}
void zpRenderingContextImpl::setConstantBuffer( zp_uint bindSlots, zp_uint index, zpBuffer* buffer )
{
    ID3D11Buffer* buff = buffer == ZP_NULL ? ZP_NULL : buffer->getBufferImpl()->m_buffer;

    if( bindSlots & ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER )
    {
        m_context->VSSetConstantBuffers( index, 1, &buff );
    }
    if( bindSlots & ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER )
    {
        m_context->GSSetConstantBuffers( index, 1, &buff );
    }
    if( bindSlots & ZP_RESOURCE_BIND_SLOT_COMPUTE_SHADER )
    {
        m_context->CSSetConstantBuffers( index, 1, &buff );
    }
    if( bindSlots & ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER )
    {
        m_context->PSSetConstantBuffers( index, 1, &buff );
    }
}

void zpRenderingContextImpl::map( zpBufferImpl* buffer, void** data, zpMapType mapType, zp_uint subResource )
{
    HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE m;

    hr = m_context->Map( buffer->m_buffer, subResource, __zpToDX( mapType ), 0, &m );
    ZP_ASSERT( SUCCEEDED( hr ), "" );

    *data = m.pData;
}
void zpRenderingContextImpl::unmap( zpBufferImpl* buffer, zp_uint subResource )
{
    m_context->Unmap( buffer->m_buffer, subResource );
}

void zpRenderingContextImpl::update( zpBufferImpl* buffer, const void* data, zp_uint size )
{
    m_context->UpdateSubresource( buffer->m_buffer, 0, ZP_NULL, data, size, 0 );
}
void zpRenderingContextImpl::processCommand( zpRenderingEngineImpl* engine, const zpRenderingCommand* command )
{
    switch( command->type )
    {
    case ZP_RENDERING_COMMNAD_NOOP:
        break;

    case ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT:
        {
            //setScissorRect( command->scissor );
        }
        break;

    case ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE: 
    case ZP_RENDERING_COMMNAD_DRAW_BUFFERED:
        if( command->material != ZP_NULL )
        {
            ID3D11Buffer* buffer = command->vertexBuffer->m_buffer;
            ID3D11Buffer* index = command->indexBuffer->m_buffer;
            const zpDynamicInputLayout* inputLayout = engine->getDynamicInputLayout( command->vertexFormatHash );

            const zpMaterial* mat = command->material->getData();
            if( m_prevMaterial != mat )
            {
                m_prevMaterial = mat;

                bindMaterial( mat );
            }

            zp_uint offset = command->vertexOffset;
            m_context->IASetPrimitiveTopology( __zpToDX( command->topology ) );
            m_context->IASetIndexBuffer( index, __zpToDX( command->indexBuffer->getFormat() ), command->indexOffset );
            m_context->IASetVertexBuffers( 0, 1, &buffer, &inputLayout->stride, &offset );
            m_context->IASetInputLayout( inputLayout->layout );

            m_context->DrawIndexed( command->indexCount, 0, 0 );
        }
        break;

    case ZP_RENDERING_COMMNAD_DRAW_INSTANCED:
        {
        }
        break;
    }
}
void zpRenderingContextImpl::processCommands( zpRenderingEngineImpl* engine, const zpArrayList< zpRenderingCommand* >& renderCommands )
{
    zpRenderingCommand* const* cmd = renderCommands.begin();
    zpRenderingCommand* const* end = renderCommands.end();

    for( ; cmd != end; ++cmd )
    {
        const zpRenderingCommand* command = *cmd;

        processCommand( engine, command );
    }
}

void zpRenderingContextImpl::bindMaterial( const zpMaterial* material )
{
    const zpShaderImpl* shader = material->shader.getResource()->getData()->getShaderImpl();

    const zp_float factor[] = { 1, 1, 1, 1 };
    const zpBlendStateImpl* blend = material->blend.getBlendStateImpl();
    m_context->OMSetBlendState( blend ? blend->m_blend : ZP_NULL, factor, -1 );

    const zpDepthStencilStateImpl* depth = material->depth.getDepthStencilStateImpl();
    m_context->OMSetDepthStencilState( depth ? depth->m_depthStencil : ZP_NULL, 0 );

    const zpRasterStateImpl* raster = material->raster.getRasterStateImpl();
    m_context->RSSetState( raster ? raster->m_raster : ZP_NULL );

    const zpBufferImpl* b = material->globalVariables.getBufferImpl();
    ID3D11Buffer* globalBuffer[1] = { b == ZP_NULL ? ZP_NULL : b->m_buffer };

    if( shader->m_vertexShader )
    {
        m_context->VSSetShader( shader->m_vertexShader, ZP_NULL, 0 );
        m_context->VSSetConstantBuffers( ZP_CONSTANT_BUFFER_SLOT_GLOBAL, 1, globalBuffer );
    }

    if( shader->m_geometryShader )
    {
        m_context->GSSetShader( shader->m_geometryShader, ZP_NULL, 0 );
        m_context->GSSetConstantBuffers( ZP_CONSTANT_BUFFER_SLOT_GLOBAL, 1, globalBuffer );
    }

    if( shader->m_pixelShader )
    {
        m_context->PSSetShader( shader->m_pixelShader, ZP_NULL, 0 );
        m_context->PSSetConstantBuffers( ZP_CONSTANT_BUFFER_SLOT_GLOBAL, 1, globalBuffer );
        
        const zpMaterialTexture* b = material->materialTextures.begin();
        const zpMaterialTexture* e = material->materialTextures.end();

        for( ; b != e; ++b )
        {
            m_context->PSSetSamplers( b->slot, 1, &b->sampler.getSamplerStateImpl()->m_sampler );
            m_context->PSSetShaderResources( b->slot, 1, &b->texture.getResource()->getData()->getTextureImpl()->m_textureResourceView );
        }
    }

#if 0
    const zpMaterial::zpMaterialTextureSampler* b = material->textures.begin();
    const zpMaterial::zpMaterialTextureSampler* e = material->textures.end();
    for( zp_uint i = 0; b != e; ++i, ++b )
    {
        const zpTexture* t = b->texture.getResource()->getData();
        const zpSamplerState& s = b->sampler;
        zpResourceBindSlotType slot = b->bindSlots;

        if( slot & ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER )
        {
            m_context->VSSetSamplers( i, 1, &s.getSamplerStateImpl()->m_sampler );
            m_context->VSSetShaderResources( i, 1, &t->getTextureImpl()->m_textureResourceView );
        }
        if( slot & ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER )
        {
            m_context->GSSetSamplers( i, 1, &s.getSamplerStateImpl()->m_sampler );
            m_context->GSSetShaderResources( i, 1, &t->getTextureImpl()->m_textureResourceView );
        }
        if( slot & ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER )
        {
            m_context->PSSetSamplers( i, 1, &s.getSamplerStateImpl()->m_sampler );
            m_context->PSSetShaderResources( i, 1, &t->getTextureImpl()->m_textureResourceView );
        }

    }
#endif
}


void zpRenderingContextImpl::set( ID3D11DeviceContext* context )
{
    m_context = context;
}
ID3D11DeviceContext* zpRenderingContextImpl::get() const
{
    return m_context;
}

#if 0
zpDX11RenderingContext::zpDX11RenderingContext() :
    m_context( ZP_NULL ),
    m_depthStencilBuffer( ZP_NULL ),
    m_referenceCount( 1 ),
    m_name()
{}
zpDX11RenderingContext::zpDX11RenderingContext( ID3D11DeviceContext* context, const zpString& name ) :
    m_context( context ),
    m_renderTargets(),
    m_depthStencilBuffer( ZP_NULL ),
    m_referenceCount( 1 ),
    m_name( name )
{}
zpDX11RenderingContext::~zpDX11RenderingContext() {
}

const zpString& zpDX11RenderingContext::getName() const {
    return m_name;
}

void zpDX11RenderingContext::setRenderTarget( zpTexture* target, zp_uint index ) {
     m_renderTargets[ index ] = target;
     m_targets[ index ] = target == ZP_NULL ? ZP_NULL : ( (zpDX11Texture*)target )->getRenderTargetView();
}
zpTexture* zpDX11RenderingContext::getRenderTarget( zp_uint index ) const {
    return m_renderTargets[ index ];
}

void zpDX11RenderingContext::setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) {
    m_depthStencilBuffer = depthBuffer;
}
zpDepthStencilBuffer* zpDX11RenderingContext::getDepthStencilBuffer() const {
    return m_depthStencilBuffer;
}

void zpDX11RenderingContext::bindRenderTargetAndDepthBuffer() {
    m_context->OMSetRenderTargets( ZP_RENDER_TARGET_MAX_COUNT, m_targets.data(), m_depthStencilBuffer ? ( (zpDX11DepthStencilBuffer*)m_depthStencilBuffer )->getDepthStencilView() : ZP_NULL );
}
void zpDX11RenderingContext::unbindRenderTargetAndDepthBuffer() {
    m_context->OMSetRenderTargets( 0, ZP_NULL, ZP_NULL );
}

void zpDX11RenderingContext::clearRenderTarget( const zpColor4f* colors, zp_uint count ) {
    zp_uint numTargets = ZP_RENDER_TARGET_MAX_COUNT;
    zp_uint num = ZP_MIN( count, numTargets );
    zp_uint i;
    zp_uint last = count - 1;

    // clear the targets up to the minimum number of either colors or targets
    for( i = 0; i < num; ++i ) {
        if( m_targets[ i ] ) m_context->ClearRenderTargetView( m_targets[ i ], colors[ i ] );
    }
    // clear the rest of the targets using the last color provided
    for( ; i < numTargets; ++i ) {
        if( m_targets[ i ] ) m_context->ClearRenderTargetView( m_targets[ i ], colors[ last ] );
    }
}
void zpDX11RenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil ) {
    m_context->ClearDepthStencilView( ( (zpDX11DepthStencilBuffer*)m_depthStencilBuffer )->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, (zp_byte)clearStencil );
}

void zpDX11RenderingContext::setBuffer( const zpBuffer* buffer, zp_uint slot ) {
    zp_uint offset = 0;
    ID3D11Buffer* b = buffer == ZP_NULL ? ZP_NULL : ( (zpDX11Buffer*)buffer )->getBuffer();
    zp_uint stride = buffer->getStride();

    switch( buffer->getBufferType() ) {
    case ZP_BUFFER_TYPE_VERTEX:
        m_context->IASetVertexBuffers( slot, 1, &b, &stride, &offset );
        break;
    case ZP_BUFFER_TYPE_INDEX:
        m_context->IASetIndexBuffer( b, __zpToDX( buffer->getFormat() ), offset );
        break;
    case ZP_BUFFER_TYPE_CONSTANT:
        m_context->VSSetConstantBuffers( slot, 1, &b );
        break;
    }
}
void zpDX11RenderingContext::setBuffers( const zpBuffer** buffers, zp_uint count, zp_uint slot ) {
    switch( buffers[ 0 ]->getBufferBindType() ) {
    case ZP_BUFFER_TYPE_VERTEX:
        {
            count = ZP_MIN( count, MAX_SET_BUFFER_VERTEX_COUNT );
            ID3D11Buffer* buffs[ MAX_SET_BUFFER_VERTEX_COUNT ];
            zp_uint strides[ MAX_SET_BUFFER_VERTEX_COUNT ];
            for( zp_uint i = 0; i < count; ++i ) {
                buffs[ i ] = ( (zpDX11Buffer*)buffers[ i ] )->getBuffer();
                strides[ i ] = buffers[ i ]->getStride();
            }

            m_context->IASetVertexBuffers( slot, count, buffs, strides, 0 );
        }
        break;
    case ZP_BUFFER_TYPE_INDEX:
        m_context->IASetIndexBuffer( ( (zpDX11Buffer*)buffers[ 0 ] )->getBuffer(), DXGI_FORMAT_R16_UINT, 0 );
        break;
    }
}

void zpDX11RenderingContext::setVertexLayout( const zpVertexLayout* layout ) {
    ID3D11InputLayout* i = layout == ZP_NULL ? ZP_NULL : ( (zpDX11VertexLayout*)layout )->getInputLayout();
    
    m_context->IASetInputLayout( i );
}

void zpDX11RenderingContext::setShader( const zpResourceInstance<zpShaderResource>* shader ) {
    if( shader == ZP_NULL ) {
        m_context->VSSetShader( ZP_NULL, ZP_NULL, 0 );
        m_context->PSSetShader( ZP_NULL, ZP_NULL, 0 );
        m_context->GSSetShader( ZP_NULL, ZP_NULL, 0 );
        m_context->CSSetShader( ZP_NULL, ZP_NULL, 0 );
    } else {
        zpDX11ShaderResource* s = (zpDX11ShaderResource*)shader->getResource();
        if( s->getVertexShader() ) {
            setVertexLayout( s->getVertexLayout() );
            m_context->VSSetShader( s->getVertexShader(), ZP_NULL, 0 );
        }
        if( s->getPixelShader() ) {
            m_context->PSSetShader( s->getPixelShader(), ZP_NULL, 0 );
        }
        if( s->getGeometryShader() ) {
            m_context->GSSetShader( s->getGeometryShader(), ZP_NULL, 0 );
        }
        if( s->getComputeShader() ) {
            m_context->CSSetShader( s->getComputeShader(), ZP_NULL, 0 );
        }
    }
}
void zpDX11RenderingContext::setTexture( zpResourceBindSlot bindType, zp_uint slot, const zpTexture* texture ) {
    ID3D11ShaderResourceView* view = texture ? ( (zpDX11Texture*)texture )->getResourceView() : ZP_NULL;
    switch( bindType ) {
    case ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER:
        m_context->VSSetShaderResources( slot, 1, view == ZP_NULL ? ZP_NULL : &view );
        break;
    case ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER:
        m_context->GSSetShaderResources( slot, 1, view == ZP_NULL ? ZP_NULL : &view );
        break;
    case ZP_RESOURCE_BIND_SLOT_COMPUTE_SHADER:
        m_context->CSSetShaderResources( slot, 1, view == ZP_NULL ? ZP_NULL : &view );
        break;
    case ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER:
        m_context->PSSetShaderResources( slot, 1, view == ZP_NULL ? ZP_NULL : &view );
        break;
    }
}

void zpDX11RenderingContext::setTopology( zpTopology topology ) {
    m_context->IASetPrimitiveTopology( __zpToDX( topology ) );
}

void zpDX11RenderingContext::setViewport( const zpViewport& viewport )
{
    D3D11_VIEWPORT v;
    v.Width =    viewport.width;
    v.Height =   viewport.height;
    v.MinDepth = viewport.minDepth;
    v.MaxDepth = viewport.maxDepth;
    v.TopLeftX = viewport.topX;
    v.TopLeftY = viewport.topY;

    D3D11_RECT r;
    r.left =   (LONG)v.TopLeftX;
    r.right =  (LONG)( v.TopLeftX + v.Width );
    r.top =    (LONG)v.TopLeftY;
    r.bottom = (LONG)( v.TopLeftY + v.Height );

    m_context->RSSetViewports( 1, &v );
    m_context->RSSetScissorRects( 1, & r );
}
void zpDX11RenderingContext::setScissorRect( const zpRecti& rect )
{
    D3D11_RECT r;
    r.left =   (LONG)rect.getPosition().getX();
    r.right =  (LONG)( rect.getPosition().getX() + rect.getSize().getX() );
    r.top =    (LONG)rect.getPosition().getY();
    r.bottom = (LONG)( rect.getPosition().getY() + rect.getSize().getY() );

    m_context->RSSetScissorRects( 1, &r );
}
void zpDX11RenderingContext::getScissorRect( zpRecti& rect ) const
{
    zp_uint num;
    D3D11_RECT r;

    m_context->RSGetScissorRects( &num, &r );

    rect.setPosition( zpVector2f( (zp_float)r.left, (zp_float)r.top ) );
    rect.setSize( zpVector2f( (zp_float)( r.right - r.left ), (zp_float)( r.bottom - r.top ) ) );
}

void zpDX11RenderingContext::setSamplerState( zpResourceBindSlot bindSlot, zp_uint slot, zpSamplerState* state ) {
    zpDX11SamplerState* s = (zpDX11SamplerState*)state;

    switch( bindSlot ) {
    case ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER:
        m_context->VSSetSamplers( slot, 1, state ? &s->m_sampler : ZP_NULL );
        break;
    case ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER:
        m_context->GSSetSamplers( slot, 1, state ? &s->m_sampler : ZP_NULL );
        break;
    case ZP_RESOURCE_BIND_SLOT_COMPUTE_SHADER:
        m_context->CSSetSamplers( slot, 1, state ? &s->m_sampler : ZP_NULL );
        break;
    case ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER:
        m_context->PSSetSamplers( slot, 1, state ? &s->m_sampler : ZP_NULL );
        break;
    }
}
void zpDX11RenderingContext::setRasterState( zpRasterState* raster ) {
    m_context->RSSetState( raster ? ( (zpDX11RasterState*)raster )->m_raster : ZP_NULL );
}

void zpDX11RenderingContext::map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data ) {
    D3D11_MAPPED_SUBRESOURCE r;
    HRESULT hr;

    hr = m_context->Map( ( (zpDX11Buffer*)buffer )->getBuffer(), subResource, __zpToDX( mapType ), 0, &r );
    if( FAILED( hr ) ) zpLog::error() << "Failed to map buffer" << zpLog::endl;

    data = &r.pData;
}
void zpDX11RenderingContext::unmap( zpBuffer* buffer, zp_uint subResource ) {
    m_context->Unmap( ( (zpDX11Buffer*)buffer )->getBuffer(), subResource );
}
void zpDX11RenderingContext::updateBuffer( zpBuffer* buffer, const void* data ) {
    m_context->UpdateSubresource( ( (zpDX11Buffer*)buffer )->getBuffer(), 0, ZP_NULL, data, 0, 0 );
}
void zpDX11RenderingContext::updateTexture( zpTexture* texture, const void* data ) {
    m_context->UpdateSubresource( ( ( zpDX11Texture*)texture )->getTexture(), 0, NULL, data, 0, 0 );
}

void zpDX11RenderingContext::draw( zp_uint vertexCount, zp_uint startIndex ) {
    m_context->Draw( vertexCount, startIndex );
}
void zpDX11RenderingContext::drawAuto() {
    m_context->DrawAuto();
}
void zpDX11RenderingContext::drawIndexed( zp_uint indexCount, zp_uint startIndex, zp_uint startVertex ) {
    m_context->DrawIndexed( indexCount, startIndex, startVertex );
}



void zpDX11RenderingContext::addReference() const {
    ++m_referenceCount;
    m_context->AddRef();
    m_depthStencilBuffer->addReference();
}
zp_bool zpDX11RenderingContext::removeReference() const {
    if( m_referenceCount == 0 ) return true;
    --m_referenceCount;
    m_context->Release();
    m_depthStencilBuffer->removeReference();
    return m_referenceCount == 0;
}

zp_uint zpDX11RenderingContext::getReferenceCount() const {
    return m_referenceCount;
}

void zpDX11RenderingContext::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpDX11RenderingContext::isMarkedForAutoDelete() const {
    return false;
}
ID3D11DeviceContext* zpDX11RenderingContext::getContext() const {
    return m_context;
}
#endif
