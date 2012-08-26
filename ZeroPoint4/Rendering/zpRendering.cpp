#include "zpRendering.h"

zpSamplerStateDesc::zpSamplerStateDesc() :
	cmpFunc( ZP_COMPARISON_FUNC_NONE ),
	minFilter (ZP_TEXTURE_FILTER_POINT ),
	magFilter( ZP_TEXTURE_FILTER_POINT ),
	mipFilter( ZP_TEXTURE_FILTER_POINT ),
	texWrapU( ZP_TEXTURE_WRAP_CLAMP ),
	texWrapV( ZP_TEXTURE_WRAP_CLAMP ),
	texWrapW( ZP_TEXTURE_WRAP_CLAMP ),
	maxAnisotrpy( 16 ),
	lodMin( -1000.f ),
	lodMax( 1000.f ),
	lodBias( 0.f ),
	borderColor( 0.f, 0.f, 0.f, 0.f )
{}