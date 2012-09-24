
#if ZP_DEBUG

#if ZP_WIN_32
#pragma comment( lib, "BulletCollision_debug" )
#pragma comment( lib, "BulletDynamics_debug" )
#pragma comment( lib, "LinearMath_debug" )
#else
#pragma comment( lib, "BulletCollision_x64_debug" )
#pragma comment( lib, "BulletDynamics_x64_debug" )
#pragma comment( lib, "LinearMath_x64_debug" )
#endif

#else

#if ZP_WIN_32
#pragma comment( lib, "BulletCollision" )
#pragma comment( lib, "BulletDynamics" )
#pragma comment( lib, "LinearMath" )
#else
#pragma comment( lib, "BulletCollision_x64" )
#pragma comment( lib, "BulletDynamics_x64" )
#pragma comment( lib, "LinearMath_x64" )
#endif

#endif
