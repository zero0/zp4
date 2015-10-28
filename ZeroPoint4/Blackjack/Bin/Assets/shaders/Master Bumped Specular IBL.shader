
#define ZP_MASTER_HQ
#define ZP_MASTER_SPECULAR_IBL
#define ZP_MASTER_SPECULAR_DIRECT
#define ZP_MASTER_NORMAL_MAP
//#define ZP_MASTER_APPROX_VIEW
#define ZP_MASTER_DIFFUSE_IBL
#define ZP_MASTER_DIFFUSE_DIRECT

#include <ZeroPoint.shaderinc>

#include <MasterInput.shaderinc>
#include <MasterCore.shaderinc>
#include <MasterSurface.shaderinc>
#include <MasterLightingModel.shaderinc>
#include <MasterDirect.shaderinc>

#pragma format VNCU
#pragma vertex MasterVert
#pragma fragment MasterFrag
