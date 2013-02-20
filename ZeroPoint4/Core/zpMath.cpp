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
