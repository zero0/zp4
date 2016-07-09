#include "zpCore.h"

zpRandom::zpRandom()
{
}
zpRandom::~zpRandom()
{}

zpRandom* zpRandom::getInstance()
{
    static zpRandom instance;
    return &instance;
}

zp_int zpRandom::randomInt()
{
    return zp_rand();
}
zp_int zpRandom::randomInt( zp_int low, zp_int high )
{
    return ( randomInt() % high ) + low;
}

zp_uint zpRandom::randomUInt()
{
    return (zp_uint)zp_rand();
}
zp_uint zpRandom::randomUInt( zp_uint low, zp_uint high )
{
    return ( randomUInt() % high ) + low;
}

zp_float zpRandom::randomFloat()
{
    return zp_randf();
}
zp_float zpRandom::randomFloat( zp_float low, zp_float high )
{
    return ( randomFloat() * ( high - low ) ) + low; 
}

zpScalar zpRandom::randomScalar( const zpScalar& low, const zpScalar& high )
{
    return zpMath::Scalar( randomFloat( zpMath::AsFloat( low ), zpMath::AsFloat( high ) ) );
}

zpVector4f zpRandom::randomVector( const zpVector4f& low, const zpVector4f& high )
{
    return zpMath::Vector4(
        randomScalar( zpMath::Vector4GetX( low ), zpMath::Vector4GetX( high ) ),
        randomScalar( zpMath::Vector4GetY( low ), zpMath::Vector4GetY( high ) ),
        randomScalar( zpMath::Vector4GetZ( low ), zpMath::Vector4GetZ( high ) ),
        randomScalar( zpMath::Vector4GetW( low ), zpMath::Vector4GetW( high ) ) );
}
zpVector4f zpRandom::randomUnitSphere( zp_float w )
{
    return zpMath::Vector4(
        randomFloat( -1.f, 1.f ),
        randomFloat( -1.f, 1.f ),
        randomFloat( -1.f, 1.f ),
        w );
}
zpVector4f zpRandom::randomUnitCircle( zp_float z, zp_float w )
{
    return zpMath::Vector4(
        randomFloat( -1.f, 1.f ),
        randomFloat( -1.f, 1.f ),
        z,
        w );
}
zpColor4f zpRandom::randomColor( zp_float alpha )
{
    return zpColor4f(
        randomFloat( 0.f, 1.f ),
        randomFloat( 0.f, 1.f ),
        randomFloat( 0.f, 1.f ),
        alpha );
}

zp_bool zpRandom::randomBoolean()
{
    return ( randomInt() % 2 ) == 0;
}

zp_int noise_permutation[] = { 
    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208,89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    23,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,43,172,9,
    129,22,39,253,19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
    // repeated
    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208,89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    23,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,43,172,9,
    129,22,39,253,19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

ZP_FORCE_INLINE zp_float fade( zp_float t ) { 
    return t * t * t * ( t * ( t * 6.f - 15.f ) + 10.f );
}

ZP_FORCE_INLINE zp_float lerp( zp_float t, zp_float a, zp_float b ) { 
    return a + t * ( b - a );
}

ZP_FORCE_INLINE zp_float grad( zp_int hash, zp_float x, zp_float y, zp_float z ) {
    zp_int h = hash & 15;
    // CONVERT LO 4 BITS OF HASH CODE
    zp_float u = h<8||h==12||h==13 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
        v = h < 4||h == 12||h == 13 ? y : z;
    return ((h & 1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

zp_float zpRandom::noise( zp_float x, zp_float y, zp_float z ) {
    zp_int X = (zp_int)x & 255, // FIND UNIT CUBE THAT
        Y = (zp_int)y & 255, // CONTAINS POINT.
        Z = (zp_int)z & 255;
    x -= (zp_int)x;                   // FIND RELATIVE X,Y,Z
    y -= (zp_int)y;                   // OF POINT IN CUBE.
    z -= (zp_int)z;
    zp_float u = fade( x ),              // COMPUTE FADE CURVES
        v = fade( y ),              // FOR EACH OF X,Y,Z.
        w = fade( z );
    int A = noise_permutation[ X ] + Y,    // HASH COORDINATES OF
        AA = noise_permutation[ A ] + Z,   // THE 8 CUBE CORNERS,
        AB = noise_permutation[ A + 1 ] + Z, 
        B = noise_permutation[ X + 1 ] + Y, 
        BA = noise_permutation[ B ] + Z, 
        BB = noise_permutation[ B + 1 ] + Z;

    return 
        lerp( w, lerp( v, lerp( u, grad( noise_permutation[ AA ], x, y, z ),      // AND ADD  
        grad( noise_permutation[ BA ], x - 1, y, z ) ),    // BLENDED
        lerp( u, grad( noise_permutation[ AB ], x, y - 1, z ),     // RESULTS
        grad( noise_permutation[ BB ], x - 1, y - 1, z ) ) ), // FROM 8
        lerp( v, lerp( u, grad( noise_permutation[ AA + 1 ], x, y, z - 1 ),   // CORNERS
        grad( noise_permutation[ BA + 1 ], x - 1, y, z - 1 ) ),// OF CUBE
        lerp( u, grad( noise_permutation[ AB + 1 ], x, y - 1, z - 1  ),
        grad( noise_permutation[ BB + 1 ], x - 1, y - 1, z - 1  ) ) ) );
}
