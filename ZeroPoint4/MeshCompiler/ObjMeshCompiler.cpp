#include "Main.h"
#include <stdio.h>

template<typename T>
struct Tuple2
{
	T a, b;
};
template<typename T>
struct Tuple3
{
	T a, b, c;
};

struct Vertex
{
	zpVector4f v, n;
	zpVector2f t;
};

struct Group
{
	zpString name;
	zpString material;
	zpArrayList< Vertex > verts;
};

enum VertexFormat
{
	VF_NONE,
	VF_VERTEX,
	VF_VERTEX_NORMAL,
	VF_VERTEX_NORMAL_TEXTURE,
};

ObjMessCompiler::~ObjMessCompiler()
{
}

zp_bool ObjMessCompiler::compileMesh()
{
	zp_bool ok = false;

	zpArrayList< zp_float > verts;
	zpArrayList< zp_float > normals;
	zpArrayList< zp_float > uvs;

	VertexFormat format = VF_NONE;
	zpArrayList< Group > groups;
	groups.reserve( 10 );
	groups.pushBackEmpty();
	groups.back().material = "default";

	zp_float x, y, z;
	zp_int v[3], t[3], n[3];

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
			}
			//else if( zp_strstr( l, "o " ) == l )
			//{
			//
			//}
			else if( zp_strstr( l, "g " ) == l )
			{
				groups.pushBackEmpty().name =  ( l + 1 );
				groups.back().material = "default";
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
		m_data.parts.reserve( materialVertices.size() );

		zpArrayList< zpString > keys;
		materialVertices.keys( keys );

		for( zpString* kb = keys.begin(), *ke = keys.end(); kb != ke; ++kb )
		{
			const zpArrayList< const Vertex* >& mv = materialVertices[ *kb ];

			MeshDataPart& part = m_data.parts.pushBackEmpty();
			part.material = *kb;
			part.indexOffset = indexCount;
			part.vertexOffset = vertexCount;

			// write verts and indexes in order
			for( const Vertex* const* start = mv.begin(), * const* end = mv.end(); start != end; ++start )
			{
				const Vertex* v = *start;
				m_data.vertex.write( v->v );
				m_data.vertex.write( v->n );
				m_data.vertex.write( v->t );
				++vertexCount;

				m_data.index.write< zp_short >( indexCount );
				++indexCount;

				part.boundingBox.add( v->v );
			}
		
			part.vertexCount = mv.size();
			part.indexCount = mv.size();
		}

		// select vertex format
		switch( format )
		{
		case VF_VERTEX:
			m_data.format = "V";
			break;
		case VF_VERTEX_NORMAL:
			m_data.format = "VN";
			break;
		case VF_VERTEX_NORMAL_TEXTURE:
			m_data.format = "VNU";
			break;
		default:
			ZP_ASSERT( false, "Unknown format" );
			break;
		}
		
		ok = format != VF_NONE;
	}

	return ok;
}
