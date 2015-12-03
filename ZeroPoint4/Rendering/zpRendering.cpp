#include "zpRendering.h"

const zp_char* VertexFormatShortNames[] =
{
	"",		// zpVertexFormat_None,

	"VC",	// ZP_VERTEX_FORMAT_VERTEX_COLOR,
	"VU",	// ZP_VERTEX_FORMAT_VERTEX_UV,
	"VCU",	// ZP_VERTEX_FORMAT_VERTEX_COLOR_UV,
	"VNU",	// ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV,
	"VNUU",	// ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2,
	"VNCU",	// ZP_VERTEX_FORMAT_VERTEX_NORMAL_COLOR_UV,

	"",		// zpVertexFormat_Count,
};

const zp_char* g_renderingQueues[] =
{
	"Background",
	"Opaque",
	"OpaqueDebug",
	"Skybox",
	"AlphaTest",
	"Transparent",
	"TransparentDebug",
	"Overlay",
	"UI",
	"UIDebug"
};

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
	, depthFunc( ZP_COMPARISON_FUNC_LESS_EQUAL )
	, stencilEnabled( false )
	, stencilReadMask( 0xFF )
	, stencilWriteMask( 0xFF )
{}
//ZP_SERIALIZE_POOLED_OBJECT( CameraComponent )
//ZP_SERIALIZE_POOLED_OBJECT( LightComponent )
//ZP_SERIALIZE_POOLED_OBJECT( StaticMeshRenderingComponent )

zpBlendTargetDesc::zpBlendTargetDesc()
	: enabled( false )
	, srcBlend( ZP_BLEND_ONE )
	, destBlend( ZP_BLEND_ZERO )
	, blendOp( ZP_BLEND_OP_ADD )
	, srcBlendAlpha( ZP_BLEND_ONE )
	, destBlendAlpha( ZP_BLEND_ZERO )
	, blendOpAlpha( ZP_BLEND_OP_ADD )
	, renderTargetWriteMask( ZP_COLOR_WRITE_ENABLE_ALL )
{}

zpBlendStateDesc::zpBlendStateDesc()
	: alphaToConverageEnabled( false )
	, independentBlendEnabled( false )
{}

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
