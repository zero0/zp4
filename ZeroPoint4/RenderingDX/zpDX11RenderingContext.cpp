#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

#define MAX_SET_BUFFER_VERTEX_COUNT	8

zpDX11RenderingContext::zpDX11RenderingContext() :
	m_context( ZP_NULL ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name()
{}
zpDX11RenderingContext::zpDX11RenderingContext( ID3D11DeviceContext* context, const zpString& name ) :
	m_context( context ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name( name )
{}
zpDX11RenderingContext::~zpDX11RenderingContext() {
}

const zpString& zpDX11RenderingContext::getName() const {
	return m_name;
}

void zpDX11RenderingContext::setRenderTarget( zpRenderTarget* target ) {
	if( m_renderTarget ) m_renderTarget->removeReference();
	if( target ) target->addReference();

	m_renderTarget = target;
}
zpRenderTarget* zpDX11RenderingContext::getRenderTarget() const {
	return m_renderTarget;
}

void zpDX11RenderingContext::setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) {
	if( m_depthStencilBuffer ) m_depthStencilBuffer->removeReference();
	if( depthBuffer ) depthBuffer->addReference();

	m_depthStencilBuffer = depthBuffer;
}
zpDepthStencilBuffer* zpDX11RenderingContext::getDepthStencilBuffer() const {
	return m_depthStencilBuffer;
}

void zpDX11RenderingContext::bindRenderTargetAndDepthBuffer() {
	m_context->OMSetRenderTargets( m_renderTarget ? m_renderTarget->getNumberOfTargets() : 0, m_renderTarget ? ( (zpDX11RenderTarget*)m_renderTarget )->getRenderTargets() : ZP_NULL, m_depthStencilBuffer ? ( (zpDX11DepthStencilBuffer*)m_depthStencilBuffer )->getDepthStencilView() : ZP_NULL );
}
void zpDX11RenderingContext::unbindRenderTargetAndDepthBuffer() {
	m_context->OMSetRenderTargets( 0, ZP_NULL, ZP_NULL );
}

void zpDX11RenderingContext::clearRenderTarget( const zpColor4f* colors, zp_uint count ) {
	zp_uint numTargets = m_renderTarget->getNumberOfTargets();
	zp_uint num = ZP_MIN( count, numTargets );
	zp_uint i;

	ID3D11RenderTargetView** views = ( (zpDX11RenderTarget*)m_renderTarget )->getRenderTargets();
	
	// clear the targets up to the minimum number of either colors or targets
	for( i = 0; i < num; ++i ) {
		m_context->ClearRenderTargetView( views[ i ], colors[ i ] );
	}
	// clear the rest of the targets using the last color provided
	for( ; i < numTargets; ++i ) {
		m_context->ClearRenderTargetView( views[ i ], colors[ count - 1 ] );
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
void zpDX11RenderingContext::setTexture( zpResourceBindSlot bindType, zp_uint slot, const zpResourceInstance<zpTextureResource>* texture ) {
	ID3D11ShaderResourceView* view = texture ? ( (zpDX11Texture*)texture->getResource()->getTexture() )->getResourceView() : ZP_NULL;
	switch( bindType ) {
	case ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER:
		m_context->VSSetShaderResources( slot, 1, view == ZP_NULL ? ZP_NULL : &view );
		break;
	case ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER:
		m_context->PSSetShaderResources( slot, 1, view == ZP_NULL ? ZP_NULL : &view );
		break;
	}
}

void zpDX11RenderingContext::setTopology( zpTopology topology ) {
	m_context->IASetPrimitiveTopology( __zpToDX( topology ) );
}

void zpDX11RenderingContext::setViewport( const zpViewport& viewport ) {
	D3D11_VIEWPORT v;
	v.Width = viewport.getWidth();
	v.Height = viewport.getHeight();
	v.MinDepth = viewport.getMinDepth();
	v.MaxDepth = viewport.getMaxDepth();
	v.TopLeftX = viewport.getTopX();
	v.TopLeftY = viewport.getTopY();

	m_context->RSSetViewports( 1, &v );
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
	m_renderTarget->addReference();
	m_depthStencilBuffer->addReference();
}
zp_bool zpDX11RenderingContext::removeReference() const {
	if( m_referenceCount == 0 ) return true;

	--m_referenceCount;
	m_context->Release();
	m_renderTarget->removeReference();
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