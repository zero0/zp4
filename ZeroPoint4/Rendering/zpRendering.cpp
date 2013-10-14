#include "zpRendering.h"

//#pragma comment( lib, "Awesomium" )

zpSamplerStateDesc::zpSamplerStateDesc()
	: cmpFunc( ZP_COMPARISON_FUNC_NEVER )
	, minFilter (ZP_TEXTURE_FILTER_POINT )
	, magFilter( ZP_TEXTURE_FILTER_POINT )
	, mipFilter( ZP_TEXTURE_FILTER_POINT )
	, texWrapU( ZP_TEXTURE_WRAP_CLAMP )
	, texWrapV( ZP_TEXTURE_WRAP_CLAMP )
	, texWrapW( ZP_TEXTURE_WRAP_CLAMP )
	, maxAnisotrpy( 16 )
	, lodMin( -1000.f )
	, lodMax( 1000.f )
	, lodBias( 0.f )
	, borderColor( 0.f, 0.f, 0.f, 0.f )
{}

zpRasterStateDesc::zpRasterStateDesc()
	: cullMode( ZP_CULL_MODE_BACK )
	, fillMode( ZP_FILL_MODE_SOLID )
	, frontFace( ZP_FRONT_FACE_CW )
	, depthBias( 0 )
	, depthBiasClamp( 0.f )
	, slopeScaledDepthBias( 0.f )
	, depthClipEnable( true )
	, scissorEnable( false )
	, multisampleEnable( false )
	, antialiasedLineEnable( false )
{}

zpDepthStencilOp::zpDepthStencilOp()
	: stencilFail( ZP_STENCIL_OP_KEEP )
	, stencilPassDepthFail( ZP_STENCIL_OP_KEEP )
	, stencilPassDepthPass( ZP_STENCIL_OP_KEEP )
	, stencilFunc( ZP_COMPARISON_FUNC_ALWAYS )
{}

zpDepthStencilStateDesc::zpDepthStencilStateDesc()
	: depthEnabled( true )
	, depthWriteMaskAll( true )
	, depthFunc( ZP_COMPARISON_FUNC_LESS )
	, stencilEnabled( false )
	, stencilReadMask( 0xFF )
	, stencilWriteMask( 0xFF )
{}
//ZP_SERIALIZE_POOLED_OBJECT( CameraComponent )
//ZP_SERIALIZE_POOLED_OBJECT( LightComponent )
//ZP_SERIALIZE_POOLED_OBJECT( StaticMeshRenderingComponent )

void zpRenderingRegisterSerializables() {
	//zpRegisterSerializable::registerSerializable<zpRenderingManager>();
	//zpRegisterSerializable::registerSerializable<zpUIManager>();
	//zpRegisterSerializable::registerSerializable<zpRenderingResourceCreator>();
	//
	//ZP_REGISTER_SERIALZED_POOL( CameraComponent );
	//ZP_REGISTER_SERIALZED_POOL( LightComponent );
	//ZP_REGISTER_SERIALZED_POOL( StaticMeshRenderingComponent );
	//
	//zpRegisterSerializable::registerSerializable<zpDeferredRenderingComponent>();
}
