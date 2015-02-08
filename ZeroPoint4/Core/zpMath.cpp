#include "zpCore.h"
#include <math.h>

zp_float zp_cos( zp_float v )
{
	return cosf( v );
}
zp_float zp_sin( zp_float v )
{
	return sinf( v );
}
zp_float zp_tan( zp_float v )
{
	return tanf( v );
}

zp_float zp_acos( zp_float v )
{
	return acosf( v );
}
zp_float zp_asin( zp_float v )
{
	return asinf( v );
}
zp_float zp_atan( zp_float v )
{
	return atanf( v );
}

zp_float zp_pow( zp_float b, zp_float p )
{
	return powf( b, p );
}

zp_float zp_sqrt( zp_float v )
{
	return sqrtf( v );
}

zp_float zp_atan2( zp_float y, zp_float x )
{
	return atan2f( y, x );
}

zp_float zp_ceil( zp_float v )
{
	return ceilf( v );
}
zp_float zp_floor( zp_float v )
{
	return floorf( v );
}

zp_int zp_ceil_to_int( zp_float v )
{
	return static_cast< zp_int >( ceilf( v ) );
}
zp_int zp_floor_to_int( zp_float v )
{
	return static_cast< zp_int >( floorf( v ) );
}

zp_float zp_fmod( zp_float x, zp_float y )
{
	return fmodf( x, y );
}


zp_bool zp_approximate( zp_float a, zp_float b )
{
	zp_float c = a - b;
	return ( c < ZP_EPSILON ) && ( c > -ZP_EPSILON );
}