#include "Main.h"
#include <stdio.h>

#define TO_STR( v )    #v
#define STR_BUFF    255

struct MaterialPart
{
    zpString name;
    zpColor4f ambientColor;
    zpColor4f diffuseColor;
    zpColor4f specularColor;
    zp_float specularPower;
    zpString diffuseMap;
    zpString specularMap;
    zpString normalMap;
};

void _mtlToMaterial( const zpString& materialFile, zpArrayList< MaterialPart >& materialParts )
{
    zp_float r, g, b;
    zp_char buff[ STR_BUFF ];

    zpStringBuffer line;
    zpFile mtlFile( materialFile );
    if( mtlFile.open( ZP_FILE_MODE_ASCII_READ ) )
    {
        while( !mtlFile.isEOF() )
        {
            zp_int count = mtlFile.readLine( line );
            if( count == 0 || line[ 0 ] == '#' ) continue;

            const zp_char* l = line.str();

            if( zp_strstr( l, "newmtl " ) == l )
            {
                MaterialPart& part = materialParts.pushBackEmpty();
                sscanf_s( l, "newmtl %s", buff );
                part.name = buff;
            }
            else if( zp_strstr( l, "Ka " ) == l )
            {
                sscanf_s( l, "Ka %f %f %f", &r, &g, &b );
                MaterialPart& part = materialParts.back();
                part.ambientColor.set( r, g, b, 1.f );
            }
            else if( zp_strstr( l, "Kd " ) == l )
            {
                sscanf_s( l, "Kd %f %f %f", &r, &g, &b );
                MaterialPart& part = materialParts.back();
                part.diffuseColor.set( r, g, b, 1.f );
            }
            else if( zp_strstr( l, "Ks " ) == l )
            {
                sscanf_s( l, "Ks %f %f %f", &r, &g, &b );
                MaterialPart& part = materialParts.back();
                part.specularColor.set( r, g, b, 1.f );
            }
            else if( zp_strstr( l, "Ns " ) == l )
            {
                sscanf_s( l, "Ns %f", &r );
                MaterialPart& part = materialParts.back();
                part.specularPower = r;
            }
            else if( zp_strstr( l, "d " ) == l )
            {
                sscanf_s( l, "d %f", &r );
                MaterialPart& part = materialParts.back();
                part.diffuseColor.setAlpha( r );
            }
            else if( zp_strstr( l, "map_Kd " ) == l )
            {
                MaterialPart& part = materialParts.back();
                sscanf_s( l, "map_Kd %s", buff );
                part.diffuseMap = buff;
            }
            else if( zp_strstr( l, "map_Ks " ) == l )
            {
                MaterialPart& part = materialParts.back();
                sscanf_s( l, "map_Ks %s", buff );
                part.specularMap = buff;
            }
            else if( zp_strstr( l, "map_bump " ) == l )
            {
                MaterialPart& part = materialParts.back();
                sscanf_s( l, "map_bump %s", buff );
                part.normalMap = buff;
            }
        }
    }
}

VertexFormat _objToMeshData( const zpString& inputFile, MeshData& meshData, zp_bool flipUV )
{
    zp_int stride;
    stride = sizeof( zpVector4f ) + sizeof( zpVector4f ) + sizeof( zpVector2f );

    VertexFormat format = VF_NONE;

    zpArrayList< zpVector4f > verts;
    zpArrayList< zpVector4f > normals;
    zpArrayList< zpVector2f > uvs;

    zp_float x, y, z;
    zp_int v[4], t[4], n[4];
    zp_int uniqueCount = 0;
    zp_int indexCount = 0;
    zp_int index = 0;

    zpArrayList< zpString > materialFiles;
    zpArrayList< zp_hash > newUniqueVertexHashes;

    zpStringBuffer line;
    zpFile objMeshFile( inputFile );
    if( objMeshFile.open( ZP_FILE_MODE_ASCII_READ ) )
    {
        while( !objMeshFile.isEOF() )
        {
            zp_int count = objMeshFile.readLine( line );
            if( count == 0 || line[ 0 ] == '#' ) continue;

            const zp_char* l = line.str();

            if( zp_strstr( l, "v " ) == l )
            {
                sscanf_s( l, "v %f %f %f", &x, &y, &z );
                verts.pushBack( zpMath::Vector4( x, y, z, 1.0f ) );
            }
            else if( zp_strstr( l, "vn " ) == l )
            {
                sscanf_s( l, "vn %f %f %f", &x, &y, &z );
                normals.pushBack( zpMath::Vector4( x, y, z, 0.0f ) );
            }
            else if( zp_strstr( l, "vt " ) == l )
            {
                sscanf_s( l, "vt %f %f", &x, &y );
                if( x < 0.0f ) x = 1.0f + x;
                if( flipUV ) y = 1.0f - y;
                uvs.pushBack( zpVector2f( x, y ) );
            }
            else if( zp_strstr( l, "f " ) == l )
            {
                if( sscanf_s( l, "f %d %d %d", &v[0], &v[1], &v[2] ) == 3 )
                {
                    if( format == VF_NONE )
                    {
                        format = VF_VERTEX;
                    }
                    else if( format != VF_VERTEX )
                    {
                        ZP_ASSERT( false, "Vertex format already set to " );
                    }

                    for( zp_int i = 0; i < 3; ++i )
                    {
                        v[i]--;

                        zp_hash h = 0;
                        h = zp_fnv1_32( v[i], h );

                        index = newUniqueVertexHashes.indexOf( h );
                        if( index < 0 )
                        {
                            index = newUniqueVertexHashes.size();
                            newUniqueVertexHashes.pushBack( h );

                            // write vertex data
                            meshData.vertex.write( verts[ v[i] ] );

                            ++uniqueCount;

                            meshData.parts.back().boundingBox.add( verts[ v[i] ] );
                        }

                        meshData.index.write< zp_ushort >( index );
                        ++indexCount;
                    }
                }
                else if( sscanf_s( l, "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2] ) == 6 )
                {
                    if( format == VF_NONE )
                    {
                        format = VF_VERTEX_NORMAL;
                    }
                    else if( format != VF_VERTEX_NORMAL )
                    {
                        ZP_ASSERT( false, "Vertex format already set to " );
                    }

                    for( zp_int i = 0; i < 3; ++i )
                    {
                        v[i]--;
                        n[i]--;

                        zp_hash h = 0;
                        h = zp_fnv1_32( v[i], h );
                        h = zp_fnv1_32( n[i], h );

                        index = newUniqueVertexHashes.indexOf( h );
                        if( index < 0 )
                        {
                            index = newUniqueVertexHashes.size();
                            newUniqueVertexHashes.pushBack( h );

                            // write vertex data
                            meshData.vertex.write( verts[ v[i] ] );
                            meshData.vertex.write( normals[ n[i] ] );

                            ++uniqueCount;

                            meshData.parts.back().boundingBox.add( verts[ v[i] ] );
                        }

                        meshData.index.write< zp_ushort >( index );
                        ++indexCount;
                    }
                }
                else if( sscanf_s( l, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2] ) == 9 )
                {
                    if( format == VF_NONE )
                    {
                        format = VF_VERTEX_NORMAL_TEXTURE;
                    }
                    else if( format != VF_VERTEX_NORMAL_TEXTURE )
                    {
                        ZP_ASSERT( false, "Vertex format already set to " );
                    }
                    
                    for( zp_int i = 0; i < 3; ++i )
                    {
                        v[i]--;
                        t[i]--;
                        n[i]--;

                        zp_hash h = 0;
                        h = zp_fnv1_32( v[i], h );
                        h = zp_fnv1_32( t[i], h );
                        h = zp_fnv1_32( n[i], h );

                        index = newUniqueVertexHashes.indexOf( h );
                        if( index < 0 )
                        {
                            index = newUniqueVertexHashes.size();
                            newUniqueVertexHashes.pushBack( h );

                            // write vertex data
                            meshData.vertex.write( verts[ v[i] ] );
                            meshData.vertex.write( normals[ n[i] ] );
                            meshData.vertex.write( uvs[ t[i] ] );

                            ++uniqueCount;

                            meshData.parts.back().boundingBox.add( verts[ v[i] ] );
                        }

                        meshData.index.write< zp_ushort >( index );
                        ++indexCount;
                    }
                }
#if 0
                else if( sscanf_s( l, "f %d %d %d %d", &v[0], &v[1], &v[2], &v[3] ) == 4 )
                {
                    format = VF_VERTEX;
                    zp_float* vp = verts.begin();

                    zp_int vi0 = ( v[0] - 1 ) * 3;
                    zp_int vi1 = ( v[1] - 1 ) * 3;
                    zp_int vi2 = ( v[2] - 1 ) * 3;
                    zp_int vi3 = ( v[3] - 1 ) * 3;

                    zpVector4f v0( *( vp + vi0 + 0 ), *( vp + vi0 + 1 ), *( vp + vi0 + 2 ), 1.0f );
                    zpVector4f v1( *( vp + vi1 + 0 ), *( vp + vi1 + 1 ), *( vp + vi1 + 2 ), 1.0f );
                    zpVector4f v2( *( vp + vi2 + 0 ), *( vp + vi2 + 1 ), *( vp + vi2 + 2 ), 1.0f );
                    zpVector4f v3( *( vp + vi3 + 0 ), *( vp + vi3 + 1 ), *( vp + vi3 + 2 ), 1.0f );

                    groups.back().verts.pushBackEmpty().v = v0;
                    groups.back().verts.pushBackEmpty().v = v1;
                    groups.back().verts.pushBackEmpty().v = v2;

                    groups.back().verts.pushBackEmpty().v = v2;
                    groups.back().verts.pushBackEmpty().v = v3;
                    groups.back().verts.pushBackEmpty().v = v0;
                }
                else if( sscanf_s( l, "f %d//%d %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2], &v[3], &n[3] ) == 8 )
                {
                    format = VF_VERTEX_NORMAL;
                    zp_float* vp = verts.begin();
                    zp_float* np = normals.begin();

                    zp_int vi0 = ( v[0] - 1 ) * 3;
                    zp_int vi1 = ( v[1] - 1 ) * 3;
                    zp_int vi2 = ( v[2] - 1 ) * 3;
                    zp_int vi3 = ( v[3] - 1 ) * 3;

                    zp_int ni0 = ( n[0] - 1 ) * 3;
                    zp_int ni1 = ( n[1] - 1 ) * 3;
                    zp_int ni2 = ( n[2] - 1 ) * 3;
                    zp_int ni3 = ( n[3] - 1 ) * 3;

                    zpVector4f v0( *( vp + vi0 + 0 ), *( vp + vi0 + 1 ), *( vp + vi0 + 2 ), 1.0f );
                    zpVector4f v1( *( vp + vi1 + 0 ), *( vp + vi1 + 1 ), *( vp + vi1 + 2 ), 1.0f );
                    zpVector4f v2( *( vp + vi2 + 0 ), *( vp + vi2 + 1 ), *( vp + vi2 + 2 ), 1.0f );
                    zpVector4f v3( *( vp + vi3 + 0 ), *( vp + vi3 + 1 ), *( vp + vi3 + 2 ), 1.0f );

                    zpVector4f n0( *( np + ni0 + 0 ), *( np + ni0 + 1 ), *( np + ni0 + 2 ), 0.0f );
                    zpVector4f n1( *( np + ni1 + 0 ), *( np + ni1 + 1 ), *( np + ni1 + 2 ), 0.0f );
                    zpVector4f n2( *( np + ni2 + 0 ), *( np + ni2 + 1 ), *( np + ni2 + 2 ), 0.0f );
                    zpVector4f n3( *( np + ni3 + 0 ), *( np + ni3 + 1 ), *( np + ni3 + 2 ), 0.0f );

                    Vertex* vert;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v1; vert->n = n1;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2;

                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v3; vert->n = n3;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0;
                }
                else if( sscanf_s( l, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2], &v[3], &t[3], &n[3] ) == 12 )
                {
                    format = VF_VERTEX_NORMAL_TEXTURE;
                    zp_float* vp = verts.begin();
                    zp_float* np = normals.begin();
                    zp_float* tp = uvs.begin();

                    zp_int vi0 = ( v[0] - 1 ) * 3;
                    zp_int vi1 = ( v[1] - 1 ) * 3;
                    zp_int vi2 = ( v[2] - 1 ) * 3;
                    zp_int vi3 = ( v[3] - 1 ) * 3;

                    zp_int ni0 = ( n[0] - 1 ) * 3;
                    zp_int ni1 = ( n[1] - 1 ) * 3;
                    zp_int ni2 = ( n[2] - 1 ) * 3;
                    zp_int ni3 = ( n[3] - 1 ) * 3;

                    zp_int ti0 = ( t[0] - 1 ) * 2;
                    zp_int ti1 = ( t[1] - 1 ) * 2;
                    zp_int ti2 = ( t[2] - 1 ) * 2;
                    zp_int ti3 = ( t[3] - 1 ) * 2;

                    zpVector4f v0( *( vp + vi0 + 0 ), *( vp + vi0 + 1 ), *( vp + vi0 + 2 ), 1.0f );
                    zpVector4f v1( *( vp + vi1 + 0 ), *( vp + vi1 + 1 ), *( vp + vi1 + 2 ), 1.0f );
                    zpVector4f v2( *( vp + vi2 + 0 ), *( vp + vi2 + 1 ), *( vp + vi2 + 2 ), 1.0f );
                    zpVector4f v3( *( vp + vi3 + 0 ), *( vp + vi3 + 1 ), *( vp + vi3 + 2 ), 1.0f );

                    zpVector4f n0( *( np + ni0 + 0 ), *( np + ni0 + 1 ), *( np + ni0 + 2 ), 0.0f );
                    zpVector4f n1( *( np + ni1 + 0 ), *( np + ni1 + 1 ), *( np + ni1 + 2 ), 0.0f );
                    zpVector4f n2( *( np + ni2 + 0 ), *( np + ni2 + 1 ), *( np + ni2 + 2 ), 0.0f );
                    zpVector4f n3( *( np + ni3 + 0 ), *( np + ni3 + 1 ), *( np + ni3 + 2 ), 0.0f );

                    zpVector2f t0( *( tp + ti0 + 0 ), *( tp + ti0 + 1 ) );
                    zpVector2f t1( *( tp + ti1 + 0 ), *( tp + ti1 + 1 ) );
                    zpVector2f t2( *( tp + ti2 + 0 ), *( tp + ti2 + 1 ) );
                    zpVector2f t3( *( tp + ti3 + 0 ), *( tp + ti3 + 1 ) );

                    Vertex* vert;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0; vert->t = t0;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v1; vert->n = n1; vert->t = t1;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2; vert->t = t2;

                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2; vert->t = t2;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v3; vert->n = n3; vert->t = t3;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0; vert->t = t0;
                }
#endif
            }
            //else if( zp_strstr( l, "o " ) == l )
            //{
            //
            //}
            else if( zp_strstr( l, "g " ) == l )
            {
                if( !meshData.parts.isEmpty() )
                {
                    MeshDataPart& end = meshData.parts.back();
                    end.vertexCount = uniqueCount;
                    end.indexCount = indexCount;
                }

                MeshDataPart& part = meshData.parts.pushBackEmpty();
                part.vertexOffset = meshData.vertex.size();
                part.indexOffset = meshData.index.size();
                part.boundingBox.setMin( zpMath::Vector4( ZP_FLT_MAX, ZP_FLT_MAX, ZP_FLT_MAX, 1.0f ) );
                part.boundingBox.setMax( zpMath::Vector4( ZP_FLT_MIN, ZP_FLT_MIN, ZP_FLT_MIN, 1.0f ) );

                uniqueCount = 0;
                indexCount = 0;
            }
            else if( zp_strstr( l, "usemtl " ) == l )
            {
                meshData.parts.back().material = ( l + 7 );
            }
            else if( zp_strstr( l, "mtllib " ) == l )
            {
                materialFiles.pushBackEmpty() = ( l + 7 );
            }
        }

        if( !meshData.parts.isEmpty() )
        {
            MeshDataPart& end = meshData.parts.back();
            end.vertexCount = uniqueCount;
            end.indexCount = indexCount;
        }
    }

    // remove empty parts
    meshData.parts.eraseIf( []( MeshDataPart& part ) {
        return part.indexCount == 0 || part.vertexCount == 0;
    } );

    // convert each material to a material file

    return format;
}

ObjMessCompiler::~ObjMessCompiler()
{
}

zp_bool ObjMessCompiler::compileMesh()
{
    VertexFormat format = VF_NONE;

    format = _objToMeshData( m_inputFile, m_mesh, true );
    
    formatToString( format, m_mesh.format );

#if 0
    zpArrayList< zp_float > verts;
    zpArrayList< zp_float > normals;
    zpArrayList< zp_float > uvs;

    VertexFormat format = VF_NONE;
    zpArrayList< Group > groups;
    groups.reserve( 10 );

    zp_float x, y, z;
    zp_int v[4], t[4], n[4];

    zpStringBuffer line;
    zpFile objMeshFile( m_inputFile );
    if( objMeshFile.open( ZP_FILE_MODE_ASCII_READ ) )
    {
        ok = true;
        while( !objMeshFile.isEOF() )
        {
            zp_int count = objMeshFile.readLine( line );
            if( count == 0 || line[ 0 ] == '#' ) continue;

            const zp_char* l = line.str();

            if( zp_strstr( l, "v " ) == l )
            {
                sscanf_s( l, "v %f %f %f", &x, &y, &z );
                verts.pushBack( x );
                verts.pushBack( y );
                verts.pushBack( z );
            }
            else if( zp_strstr( l, "vn " ) == l )
            {
                sscanf_s( l, "vn %f %f %f", &x, &y, &z );
                normals.pushBack( x );
                normals.pushBack( y );
                normals.pushBack( z );
            }
            else if( zp_strstr( l, "vt " ) == l )
            {
                sscanf_s( l, "vt %f %f", &x, &y );
                if( x < 0.0f ) x = 1.0f + x;
                y = 1.0f - y;
                uvs.pushBack( x );
                uvs.pushBack( y );
            }
            else if( zp_strstr( l, "f " ) == l )
            {
                if( sscanf_s( l, "f %d %d %d", &v[0], &v[1], &v[2] ) == 3 )
                {
                    format = VF_VERTEX;
                    zp_float* vp = verts.begin();

                    for( zp_int i = 0; i < 3; ++i )
                    {
                        v[i]--;
                        v[i] *= 3;

                        Vertex& v0 = groups.back().verts.pushBackEmpty();
                        v0.v = zpVector4f( *( vp + v[i] + 0 ), *( vp + v[i] + 1 ), *( vp + v[i] + 2 ), 1.0f );
                    }
                }
                else if( sscanf_s( l, "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2] ) == 6 )
                {
                    format = VF_VERTEX_NORMAL;
                    zp_float* vp = verts.begin();
                    zp_float* np = normals.begin();

                    for( zp_int i = 0; i < 3; ++i )
                    {
                        v[i]--;
                        n[i]--;
                        v[i] *= 3;
                        n[i] *= 3;

                        Vertex& v0 = groups.back().verts.pushBackEmpty();
                        v0.v = zpVector4f( *( vp + v[i] + 0 ), *( vp + v[i] + 1 ), *( vp + v[i] + 2 ), 1.0f );
                        v0.n = zpVector4f( *( np + n[i] + 0 ), *( np + n[i] + 1 ), *( np + n[i] + 2 ), 0.0f );
                    }
                }
                else if( sscanf_s( l, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2] ) == 9 )
                {
                    format = VF_VERTEX_NORMAL_TEXTURE;
                    zp_float* vp = verts.begin();
                    zp_float* np = normals.begin();
                    zp_float* tp = uvs.begin();

                    for( zp_int i = 0; i < 3; ++i )
                    {
                        v[i]--;
                        n[i]--;
                        t[i]--;
                        v[i] *= 3;
                        n[i] *= 3;
                        t[i] *= 2;

                        Vertex& v0 = groups.back().verts.pushBackEmpty();
                        v0.v = zpVector4f( *( vp + v[i] + 0 ), *( vp + v[i] + 1 ), *( vp + v[i] + 2 ), 1.0f );
                        v0.n = zpVector4f( *( np + n[i] + 0 ), *( np + n[i] + 1 ), *( np + n[i] + 2 ), 0.0f );
                        v0.t = zpVector2f( *( tp + t[i] + 0 ), *( tp + t[i] + 1 ) );
                    }
                }
                if( sscanf_s( l, "f %d %d %d %d", &v[0], &v[1], &v[2], &v[3] ) == 4 )
                {
                    format = VF_VERTEX;
                    zp_float* vp = verts.begin();

                    zp_int vi0 = ( v[0] - 1 ) * 3;
                    zp_int vi1 = ( v[1] - 1 ) * 3;
                    zp_int vi2 = ( v[2] - 1 ) * 3;
                    zp_int vi3 = ( v[3] - 1 ) * 3;

                    zpVector4f v0( *( vp + vi0 + 0 ), *( vp + vi0 + 1 ), *( vp + vi0 + 2 ), 1.0f );
                    zpVector4f v1( *( vp + vi1 + 0 ), *( vp + vi1 + 1 ), *( vp + vi1 + 2 ), 1.0f );
                    zpVector4f v2( *( vp + vi2 + 0 ), *( vp + vi2 + 1 ), *( vp + vi2 + 2 ), 1.0f );
                    zpVector4f v3( *( vp + vi3 + 0 ), *( vp + vi3 + 1 ), *( vp + vi3 + 2 ), 1.0f );

                    groups.back().verts.pushBackEmpty().v = v0;
                    groups.back().verts.pushBackEmpty().v = v1;
                    groups.back().verts.pushBackEmpty().v = v2;

                    groups.back().verts.pushBackEmpty().v = v2;
                    groups.back().verts.pushBackEmpty().v = v3;
                    groups.back().verts.pushBackEmpty().v = v0;
                }
                else if( sscanf_s( l, "f %d//%d %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2], &v[3], &n[3] ) == 8 )
                {
                    format = VF_VERTEX_NORMAL;
                    zp_float* vp = verts.begin();
                    zp_float* np = normals.begin();

                    zp_int vi0 = ( v[0] - 1 ) * 3;
                    zp_int vi1 = ( v[1] - 1 ) * 3;
                    zp_int vi2 = ( v[2] - 1 ) * 3;
                    zp_int vi3 = ( v[3] - 1 ) * 3;

                    zp_int ni0 = ( n[0] - 1 ) * 3;
                    zp_int ni1 = ( n[1] - 1 ) * 3;
                    zp_int ni2 = ( n[2] - 1 ) * 3;
                    zp_int ni3 = ( n[3] - 1 ) * 3;

                    zpVector4f v0( *( vp + vi0 + 0 ), *( vp + vi0 + 1 ), *( vp + vi0 + 2 ), 1.0f );
                    zpVector4f v1( *( vp + vi1 + 0 ), *( vp + vi1 + 1 ), *( vp + vi1 + 2 ), 1.0f );
                    zpVector4f v2( *( vp + vi2 + 0 ), *( vp + vi2 + 1 ), *( vp + vi2 + 2 ), 1.0f );
                    zpVector4f v3( *( vp + vi3 + 0 ), *( vp + vi3 + 1 ), *( vp + vi3 + 2 ), 1.0f );

                    zpVector4f n0( *( np + ni0 + 0 ), *( np + ni0 + 1 ), *( np + ni0 + 2 ), 0.0f );
                    zpVector4f n1( *( np + ni1 + 0 ), *( np + ni1 + 1 ), *( np + ni1 + 2 ), 0.0f );
                    zpVector4f n2( *( np + ni2 + 0 ), *( np + ni2 + 1 ), *( np + ni2 + 2 ), 0.0f );
                    zpVector4f n3( *( np + ni3 + 0 ), *( np + ni3 + 1 ), *( np + ni3 + 2 ), 0.0f );

                    Vertex* vert;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v1; vert->n = n1;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2;

                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v3; vert->n = n3;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0;
                }
                else if( sscanf_s( l, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2], &v[3], &t[3], &n[3] ) == 12 )
                {
                    format = VF_VERTEX_NORMAL_TEXTURE;
                    zp_float* vp = verts.begin();
                    zp_float* np = normals.begin();
                    zp_float* tp = uvs.begin();

                    zp_int vi0 = ( v[0] - 1 ) * 3;
                    zp_int vi1 = ( v[1] - 1 ) * 3;
                    zp_int vi2 = ( v[2] - 1 ) * 3;
                    zp_int vi3 = ( v[3] - 1 ) * 3;

                    zp_int ni0 = ( n[0] - 1 ) * 3;
                    zp_int ni1 = ( n[1] - 1 ) * 3;
                    zp_int ni2 = ( n[2] - 1 ) * 3;
                    zp_int ni3 = ( n[3] - 1 ) * 3;

                    zp_int ti0 = ( t[0] - 1 ) * 2;
                    zp_int ti1 = ( t[1] - 1 ) * 2;
                    zp_int ti2 = ( t[2] - 1 ) * 2;
                    zp_int ti3 = ( t[3] - 1 ) * 2;

                    zpVector4f v0( *( vp + vi0 + 0 ), *( vp + vi0 + 1 ), *( vp + vi0 + 2 ), 1.0f );
                    zpVector4f v1( *( vp + vi1 + 0 ), *( vp + vi1 + 1 ), *( vp + vi1 + 2 ), 1.0f );
                    zpVector4f v2( *( vp + vi2 + 0 ), *( vp + vi2 + 1 ), *( vp + vi2 + 2 ), 1.0f );
                    zpVector4f v3( *( vp + vi3 + 0 ), *( vp + vi3 + 1 ), *( vp + vi3 + 2 ), 1.0f );

                    zpVector4f n0( *( np + ni0 + 0 ), *( np + ni0 + 1 ), *( np + ni0 + 2 ), 0.0f );
                    zpVector4f n1( *( np + ni1 + 0 ), *( np + ni1 + 1 ), *( np + ni1 + 2 ), 0.0f );
                    zpVector4f n2( *( np + ni2 + 0 ), *( np + ni2 + 1 ), *( np + ni2 + 2 ), 0.0f );
                    zpVector4f n3( *( np + ni3 + 0 ), *( np + ni3 + 1 ), *( np + ni3 + 2 ), 0.0f );

                    zpVector2f t0( *( tp + ti0 + 0 ), *( tp + ti0 + 1 ) );
                    zpVector2f t1( *( tp + ti1 + 0 ), *( tp + ti1 + 1 ) );
                    zpVector2f t2( *( tp + ti2 + 0 ), *( tp + ti2 + 1 ) );
                    zpVector2f t3( *( tp + ti3 + 0 ), *( tp + ti3 + 1 ) );

                    Vertex* vert;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0; vert->t = t0;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v1; vert->n = n1; vert->t = t1;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2; vert->t = t2;

                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v2; vert->n = n2; vert->t = t2;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v3; vert->n = n3; vert->t = t3;
                    vert = &groups.back().verts.pushBackEmpty(); vert->v = v0; vert->n = n0; vert->t = t0;
                }
            }
            //else if( zp_strstr( l, "o " ) == l )
            //{
            //
            //}
            else if( zp_strstr( l, "g " ) == l )
            {
                groups.pushBackEmpty().name =  ( l + 1 );
            }
            else if( zp_strstr( l, "usemtl " ) == l )
            {
                groups.back().material = ( l + 7 );
            }
            //else if( zp_strstr( l, "mtllib " ) == l )
            //{
            //
            //}
        }
    }
#endif

#if 0
    if( !groups.isEmpty() )
    {
        // group all vertices for materials together
        zpHashMap< zpString, zpArrayList< const Vertex* > > materialVertices;
        for( Group* gb = groups.begin(), *ge = groups.end(); gb != ge; ++gb )
        {
            Group& group = *gb;
            zpArrayList< const Vertex* >* v = &materialVertices[ group.material ];
            //zpArrayList< const Vertex* >* v;
            //materialVertices
            v->reserve( v->size() + group.verts.size() );
            for( const Vertex* start = group.verts.begin(), *vend = group.verts.end(); start != vend; ++start )
            {
                v->pushBack( start );
            }
        }

        // push all ordered verts to single buffer and create mesh parts from full buffer
        zp_int indexCount = 0;
        zp_int vertexCount = 0;
        m_mesh.parts.reserve( materialVertices.size() );

        zpArrayList< zpString > keys;
        materialVertices.keys( keys );

        for( zpString* kb = keys.begin(), *ke = keys.end(); kb != ke; ++kb )
        {
            const zpArrayList< const Vertex* >& mv = materialVertices[ *kb ];
            if( mv.isEmpty() ) continue;

            MeshDataPart& part = m_mesh.parts.pushBackEmpty();
            part.material = *kb;
            part.indexOffset = indexCount;
            part.vertexOffset = vertexCount;

            // write verts and indexes in order
            for( const Vertex* const* start = mv.begin(), * const* end = mv.end(); start != end; ++start )
            {
                const Vertex* v = *start;
                m_mesh.vertex.write( v->v );
                m_mesh.vertex.write( v->n );
                m_mesh.vertex.write( v->t );
                ++vertexCount;

                m_mesh.index.write< zp_ushort >( indexCount );
                ++indexCount;

                part.boundingBox.add( v->v );
            }
        
            part.vertexCount = mv.size();
            part.indexCount = mv.size();
        }

        // select vertex format
        formatToString( format, m_mesh.format );
        
        ok = format != VF_NONE;
    }
#endif
    return format != VF_NONE;
}
