#include "zpRendering.h"
#include <stdio.h>

zpOBJStaticMeshResource::zpOBJStaticMeshResource() {}
zpOBJStaticMeshResource::~zpOBJStaticMeshResource() {}

zp_bool zpOBJStaticMeshResource::load() {
#if 0
	// that data passed was the name of the file to load
	zpFile objFile( getFilename() );
	zpArrayList<zpVertexPositionNormalTexture> triangles;

	if( objFile.open( ZP_FILE_MODE_READ ) ) {
		// store the vertex, normal, and texture coordinate data
		zpArrayList<zpVector4f> verticies;
		zpArrayList<zpVector4f> normals;
		zpArrayList<zpVector2f> texCoords;
		zpString line;
	
		// read each line
		zpStringBuffer buff;
		while( objFile.readLine( buff ) > 0 ) {
			line = buff.toString();

			// trim the line (left trim only needed)
			line.ltrim();

			// if the line is empty or a comment, continue to the next line
			if( line.isEmpty() || line[0] == '#' ) {
				buff.clear();
				continue;
			}

			if( line[0] == 'v' ) {
				if( line[1] == 'n' ) {
					zp_float x, y, z;
					sscanf_s( line.getChars(),"vn %f %f %f", &x, &y, &z  );

					//zpLog::debug() << "vn " << x << ' ' << y << ' ' << z << zpLog::endl;
					
					normals.pushBack( zpVector4f( x, y, z, 1 ) );
				} else if( line[1] == 't' ) {
					zp_float x, y;
					sscanf_s( line.getChars(),"vt %f %f", &x, &y  );

					//zpLog::debug() << "vt " << x << ' ' << y << ' ' << zpLog::endl;

					texCoords.pushBack( zpVector2f( x, y ) );
				} else {
					zp_float x, y, z;
					sscanf_s( line.getChars(),"v %f %f %f", &x, &y, &z  );

					//zpLog::debug() << "v  " << x << ' ' << y << ' ' << z << zpLog::endl;

					verticies.pushBack( zpVector4f( x, y, z, 1 ) );
				}
			} else if( line[0] == 'f' ) {
				zp_uint pos = line.indexOf( ' ' );
				zp_uint end = zpString::npos;
				zp_uint i = 0;

				zpVertexPositionNormalTexture vnt[3];

				while( i < 3 ) {
					end = line.indexOf( ' ', pos + 1 );// ) != zpString::npos;
					zpString face = line.substring( pos + 1, end );

					zp_uint v, t, n;
					sscanf_s( face.getChars(), "%d/%d/%d", &v, &t, &n );

					vnt[i].position = verticies[v - 1];
					vnt[i].normal = normals[n - 1];
					vnt[i].texCoord0 = texCoords[t - 1];

					++i;

					pos = end;
				}

				triangles.pushBack( vnt[0] );
				triangles.pushBack( vnt[1] );
				triangles.pushBack( vnt[2] );
			}
			// clear the buffer so it can be used again
			buff.clear();
		}

		// close the file as it has been parsed
		objFile.close();
	}

	// if there are no triangles, return
	if( triangles.isEmpty() ) return false;

	zpStaticMeshPart part;

	// create the buffer and fill with verticies
	part.vertexBuffer = zpRenderingFactory::getRenderingEngine()->createBuffer();
	part.vertexBuffer->create( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, triangles.size(), sizeof( zpVertexPositionNormalTexture ), triangles.begin() );
	
	m_meshParts.pushBack( part );
#endif
	return true;
}
