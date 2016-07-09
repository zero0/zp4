#include "zpRendering.h"

zpPlaneOps::zpPlaneOps()
    : width( 1.0f )
    , height( 1.0f )
    , widthSegments( 1 )
    , heightSegments( 1 )
    , up( zpMath::Vector4( 0, 1, 0, 0 ) )
    , uvRect( 0, 0, 1, 1 )
    , rotationAngle( 0.0f )
    , isTopLeftCenter( false )
{}

void zpGenerateMesh::Plane( zpArrayList< zpVector4f >& verts, zpArrayList< zpVector4f >& normals, zpArrayList< zpVector2f >& uvs, zpArrayList< zp_short >& triangles, const zpPlaneOps& ops )
{
    zp_uint totalVerts = ( ops.widthSegments + 1 ) * ( ops.heightSegments + 1 );
    zp_uint offset = verts.size();

    verts.reserve( offset + totalVerts );
    normals.reserve( offset + totalVerts );
    uvs.reserve( offset + totalVerts );
    triangles.reserve( offset + 6 * ops.widthSegments * ops.heightSegments );
    
    zpVector2f uv, size;
    zpVector4f up( ops.up ), vx, vy, vw, vh, vert;
    up = zpMath::Vector4Normalize3( up );
    vx = zpMath::Vector4Perpendicular3( up );
    vy = zpMath::Vector4Cross3( up, vx );

    zpScalar wDelta = zpMath::Scalar( ops.width );
    zpScalar hDelta = zpMath::Scalar( ops.height );

    zpVector4f orig = zpMath::Vector4( 0.f, 0.f, 0.f, 1.f );
    if( !ops.isTopLeftCenter )
    {
        zpVector4f a, b, c;
        a = zpMath::Vector4Mul( vx, wDelta );
        a = zpMath::Vector4Mul( a, zpMath::Scalar( -2.0f ) );

        b = zpMath::Vector4Mul( vy, hDelta );
        b = zpMath::Vector4Mul( b, zpMath::Scalar( -2.0f ) );

        c = zpMath::Vector4Add( a, b );
        orig = zpMath::Vector4Add( orig, c );
    }

    zpScalar uMin = zpMath::Scalar( ops.uvRect.getLeft() );
    zpScalar uMax = zpMath::Scalar( ops.uvRect.getRight() );
    zpScalar vMin = zpMath::Scalar( ops.uvRect.getTop() );
    zpScalar vMax = zpMath::Scalar( ops.uvRect.getBottom() );

    for( zp_uint w = 0; w <= ops.widthSegments; ++w )
    {
        for( zp_uint h = 0; h <= ops.heightSegments; ++h )
        {
            zpScalar wd = zpMath::Scalar( (zp_float)w );
            zpScalar hd = zpMath::Scalar( (zp_float)h ), u, v;

            wd = zpMath::ScalarDiv( wd, zpMath::Scalar( (zp_float)ops.widthSegments ) );
            hd = zpMath::ScalarDiv( hd, zpMath::Scalar( (zp_float)ops.heightSegments ) );

            u = zpMath::ScalarLerp( uMin, uMax, wd );
            v = zpMath::ScalarLerp( vMin, vMax, hd );

            wd = zpMath::ScalarMul( wd, wDelta );
            hd = zpMath::ScalarMul( hd, hDelta );

            vw = zpMath::Vector4Mul( vx, wd );
            vh = zpMath::Vector4Mul( vy, hd );

            vert = zpMath::Vector4Add( orig, vw );
            vert = zpMath::Vector4Add( vert, vh );

            verts.pushBack( vert );
            normals.pushBack( up );
            uvs.pushBack( zpVector2f( zpMath::AsFloat( u ), zpMath::AsFloat( v ) ) );
        }
    }

    for( zp_uint w = 0; w < ops.widthSegments; ++w )
    {
        for( zp_uint h = 0; h < ops.heightSegments; ++h )
        {
            triangles.pushBack( offset );
            triangles.pushBack( offset + ops.heightSegments + 1 );
            triangles.pushBack( offset + 1 );

            triangles.pushBack( offset + 1 );
            triangles.pushBack( offset + ops.heightSegments + 1 );
            triangles.pushBack( offset + ops.heightSegments + 2 );
            ++offset;
        }
        ++offset;
    }
}

void zpGenerateMesh::Rebuild( zpRenderingEngine* engine, zpMesh* mesh, zpArrayList< zpVector4f >& verts, zpArrayList< zpVector4f >& normals, zpArrayList< zpVector2f >& uvs, zpArrayList< zp_short >& triangles )
{
    zpDataBuffer vertexData( 64 * verts.size() );
    zpDataBuffer indexData( sizeof( zp_short ) * triangles.size() );

    zpBoundingAABB box;

    // full vertex and index data
    for( zp_uint i = 0; i < verts.size(); ++i )
    {
        box.add( verts[i] );
        vertexData.write( verts[i] );
        vertexData.write( normals[i] );
        vertexData.write( uvs[i] );
    }
    indexData.writeBulk( triangles.begin(), triangles.size() );

    // destroy old data
    engine->destroyBuffer( mesh->m_vertex );
    engine->destroyBuffer( mesh->m_index );
    mesh->m_parts.clear();
    
    // create new data
    mesh->m_format = ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV;
    engine->createBuffer( mesh->m_vertex, ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, vertexData.size(), 0, vertexData.getData() );
    engine->createBuffer( mesh->m_index, ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_IMMUTABLE, indexData.size(), sizeof( zp_short ), indexData.getData() );
    
    mesh->m_parts.reserve( 1 );
    zpMeshPart& mp = mesh->m_parts.pushBackEmpty();
    mp.m_indexOffset = 0;
    mp.m_indexCount = triangles.size();
    mp.m_vertexOffset = 0;
    mp.m_vertexCount = verts.size();
    mp.m_boundingBox = box;
}
