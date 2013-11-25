#include "zpRendering.h"

zpPlaneOps::zpPlaneOps()
	: width( 1.0f )
	, height( 1.0f )
	, widthSegments( 1 )
	, heightSegments( 1 )
	, up( zpVector4f( 0, 1, 0 ) )
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
	zpMath::Normalize3( up, up );
	zpMath::Perpendicular3( vx, up );
	zpMath::Cross3( vy, up, vx );

	zpScalar wDelta( ops.width ), hDelta( ops.height );

	zpVector4f orig( 0.f );
	if( !ops.isTopLeftCenter )
	{
		zpVector4f a, b;
		zpMath::Mul( a, vx, wDelta );
		zpMath::Mul( a, a, zpScalar( -2.0f ) );

		zpMath::Mul( b, vy, hDelta );
		zpMath::Mul( b, b, zpScalar( -2.0f ) );

		zpMath::Add( orig, a, b );
	}

	zpScalar uMin( ops.uvRect.getLeft() ), uMax( ops.uvRect.getRight() );
	zpScalar vMin( ops.uvRect.getTop() ), vMax( ops.uvRect.getBottom() );

	for( zp_uint w = 0; w <= ops.widthSegments; ++w )
	{
		for( zp_uint h = 0; h <= ops.heightSegments; ++h )
		{
			zpScalar wd( (zp_float)w ), hd( (zp_float)h ), u, v;
			zpMath::Div( wd, wd, zpScalar( (zp_float)ops.widthSegments ) );
			zpMath::Div( hd, hd, zpScalar( (zp_float)ops.heightSegments ) );

			zpMath::Lerp( u, uMin, uMax, wd );
			zpMath::Lerp( v, vMin, vMax, hd );

			zpMath::Mul( wd, wd, wDelta );
			zpMath::Mul( hd, hd, hDelta );

			zpMath::Mul( vw, vx, wd );
			zpMath::Mul( vh, vy, hd );

			zpMath::Add( vert, orig, vw );
			zpMath::Add( vert, vert, vh );

			verts.pushBack( vert );
			normals.pushBack( up );
			uvs.pushBack( zpVector2f( u.getFloat(), v.getFloat() ) );
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
	zpDataBuffer vertexData( ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV * verts.size() );
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
