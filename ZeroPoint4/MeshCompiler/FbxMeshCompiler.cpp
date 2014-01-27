#include "Main.h"
#include <fbxsdk.h>

#pragma comment( lib, "libfbxsdk.lib" )

FbxMessCompiler::~FbxMessCompiler()
{
}

void _getFbxInfo( FbxMeshData* data, FbxNode* node )
{
	zp_int numChildren = node->GetChildCount();
	for( zp_int i = 0; i < numChildren; ++i )
	{
		FbxNode* child = node->GetChild( i );
		FbxMesh* mesh = child->GetMesh();
		if( mesh )
		{
			FbxVector4* controlPoints = mesh->GetControlPoints();
			
			zp_int vertexId = 0;
			zp_int polygonCount = mesh->GetPolygonCount();
			for( zp_int p = 0; p < polygonCount; ++p )
			{
				zp_int polygonSize = mesh->GetPolygonSize( p );
				zp_int cap = polygonCount * polygonSize;
				data->verts.reserve( data->verts.size() + cap );
				data->indecies.reserve( data->indecies.size() + cap );
				data->uvs.reserve( data->uvs.size() + cap );
				data->normals.reserve( data->normals.size() + cap );

				for( zp_int s = 0; s < polygonSize; ++s )
				{
					zp_int controlPointIndex = mesh->GetPolygonVertex( p, s );

					vertexId = data->verts.size();

					// index
					data->indecies.pushBack( vertexId );

					// vertex
					FbxVector4& vertex = controlPoints[ controlPointIndex ];
					data->verts.pushBack( zpVector4f( (zp_float)vertex.mData[0], (zp_float)vertex.mData[1], (zp_float)vertex.mData[2], 1.0f ) );

					// uvs
					for( zp_int l = 0; l < mesh->GetElementUVCount(); ++l )
					{
						FbxGeometryElementUV* elemUv = mesh->GetElementUV( l );
						zp_int index = -1;
						switch( elemUv->GetMappingMode() )
						{
						case FbxGeometryElement::eByControlPoint:
							{
								switch( elemUv->GetReferenceMode() )
								{
								case FbxGeometryElement::eDirect:
									index = controlPointIndex;
									break;	
								case FbxGeometryElement::eIndexToDirect:
									index = elemUv->GetIndexArray().GetAt( controlPointIndex );
									break;
								}
							}
							break;
						case FbxGeometryElement::eByPolygonVertex:
							index = mesh->GetTextureUVIndex( p, s );
							break;
						default:
							break;
						}

						FbxVector2 uv = elemUv->GetDirectArray().GetAt( index );
						data->uvs.pushBack( zpVector2f( (zp_float)uv.mData[0], (zp_float)uv.mData[1] ) );
					}

					// normals
					for( zp_int l = 0; l < mesh->GetElementNormalCount(); ++l )
					{
						FbxGeometryElementNormal* elemNormal = mesh->GetElementNormal( l );
						zp_int index = -1;
						FbxVector4 normal;
						switch( elemNormal->GetMappingMode() )
						{
						case FbxGeometryElement::eByControlPoint:
							{
								switch( elemNormal->GetReferenceMode() )
								{
								case FbxGeometryElement::eDirect:
									index = controlPointIndex;
									break;	
								case FbxGeometryElement::eIndexToDirect:
									index = elemNormal->GetIndexArray().GetAt( controlPointIndex );
									break;
								}
							}
							break;
						case FbxGeometryElement::eByPolygonVertex:
							mesh->GetPolygonVertexNormal( p, s, normal );
							break;
						default:
							break;
						}

						if( index > -1 ) normal = elemNormal->GetDirectArray().GetAt( index );
						data->normals.pushBack( zpVector4f( (zp_float)normal.mData[0], (zp_float)normal.mData[1], (zp_float)normal.mData[2], 0.0f ) );
					}

					// colors
					for( zp_int l = 0; l < mesh->GetElementVertexColorCount(); ++l )
					{
						FbxGeometryElementVertexColor* elem = mesh->GetElementVertexColor( l );
						zp_int index = -1;
						switch( elem->GetMappingMode() )
						{
						case FbxGeometryElement::eByControlPoint:
							switch( elem->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = controlPointIndex;
								break;
							case FbxGeometryElement::eIndexToDirect:
								index = elem->GetIndexArray().GetAt( controlPointIndex );
								break;
							}
							break;
						case FbxGeometryElement::eByPolygonVertex:
							switch( elem->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = vertexId;
								break;
							case FbxGeometryElement::eIndexToDirect:
								index = elem->GetIndexArray().GetAt( vertexId );
								break;
							}
							break;
						}

						FbxColor color = elem->GetDirectArray().GetAt( index );
						data->colors.pushBack( zpColor4f( (zp_float)color.mRed, (zp_float)color.mGreen, (zp_float)color.mBlue, (zp_float)color.mAlpha ) );
					}
				}
			}
		}

		_getFbxInfo( data, child );
	}
}

zp_bool FbxMessCompiler::compileMesh()
{
	zp_bool ok = false;

	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* ioSettings = FbxIOSettings::Create( manager, IOSROOT );
	manager->SetIOSettings( ioSettings );

	FbxImporter* importer = FbxImporter::Create( manager, "" );
	importer->Initialize( m_inputFile.str(), -1, manager->GetIOSettings() );

	FbxScene* scene = FbxScene::Create( manager, "tempName" );

	importer->Import( scene );
	importer->Destroy();

	FbxAxisSystem axis( FbxAxisSystem::eDirectX );
	axis.ConvertScene( scene );

	FbxNode* rootNode = scene->GetRootNode();
	_getFbxInfo( &m_fbxData, rootNode );

	

	return ok;
}