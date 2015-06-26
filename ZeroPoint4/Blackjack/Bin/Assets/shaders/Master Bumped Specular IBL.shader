
#include <ZeroPoint.shaderinc>

#define ZP_MASTER_HQ
#define ZP_MASTER_SPECULAR_IBL
#define ZP_MASTER_SPECULAR_DIRECT
#define ZP_MASTER_NORMAL_MAP
//#define ZP_MASTER_APPROX_VIEW
#define ZP_MASTER_DIFFUSE_IBL
#define ZP_MASTER_DIFFUSE_DIRECT

#include <MasterInput.shaderinc>
#include <MasterCore.shaderinc>
#include <MasterLightingModel.shaderinc>
#include <MasterSurface.shaderinc>

#pragma format VNCU
#pragma vert MasterVert
#pragma frag MasterFrag
