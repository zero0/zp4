#include "Main.h"

#include <fbxsdk.h>

#pragma comment( lib, "libfbxsdk-md" )

FbxMessCompiler::~FbxMessCompiler()
{
}

void _getFbxInfo( FbxMeshData* data, FbxNode* node, zp_bool flipUVs )
{
	zp_int numChildren = node->GetChildCount();
	for( zp_int i = 0; i < numChildren; ++i )
	{
		FbxNode* child = node->GetChild( i );
		FbxMesh* mesh = child->GetMesh();
		if( mesh )
		{
			FbxMeshDataPart& meshPart = data->parts.pushBackEmpty();

			zp_int controlPointCount = mesh->GetControlPointsCount();
			zp_int polygonCount = mesh->GetPolygonCount();
			zp_int normalCount = mesh->GetElementNormalCount();
			zp_int binormalCount = mesh->GetElementBinormalCount();
			zp_int tangentCount = mesh->GetElementTangentCount();
			zp_int colorCount = mesh->GetElementVertexColorCount();
			zp_int uvCount = mesh->GetElementUVCount();
			zp_int materialCount = mesh->GetElementMaterialCount();

			meshPart.verts.reserve( controlPointCount );
			
			// verts
			FbxVector4* controlPoints = mesh->GetControlPoints();
			for( zp_int c = 0; c < controlPointCount; ++c )
			{
				FbxVector4& v = controlPoints[ c ];
				meshPart.verts.pushBack( zpVector4f( (zp_float)v[0], (zp_float)v[1], (zp_float)v[2], 1 ) );
			}

			// vert indecies
			meshPart.vertIndecies.reserve( polygonCount * mesh->GetPolygonSize( 0 ) );
			for( zp_int p = 0; p < polygonCount; ++p )
			{
				zp_int index;
				zp_int polygonSize = mesh->GetPolygonSize( p );
				for( zp_int s = 0; s < polygonSize; ++s )
				{
					index = mesh->GetPolygonVertex( p, s );
					meshPart.vertIndecies.pushBack( index );
				}
			}

			// uvs
			for( zp_int u = 0; u < uvCount; ++u )
			{
				FbxGeometryElementUV* elemUv = mesh->GetElementUV( u );
				zp_int index = -1;
				switch( elemUv->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					{
						meshPart.uvs.reserve( controlPointCount );
						meshPart.uvIndecies.reserve( controlPointCount );
						for( zp_int i = 0; i < controlPointCount; ++i )
						{
							switch( elemUv->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = i;
								break;	
							case FbxGeometryElement::eIndexToDirect:
								index = elemUv->GetIndexArray().GetAt( i );
								break;
							}

							meshPart.uvIndecies.pushBack( index );

							FbxVector2 uv = elemUv->GetDirectArray().GetAt( i );
							meshPart.uvs.pushBack( zpVector2f( (zp_float)uv.mData[0], (zp_float)uv.mData[1] ) );
						}
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
					{
						zp_int uvCount = elemUv->GetDirectArray().GetCount();
						meshPart.uvs.reserve( uvCount );
						for( zp_int u = 0; u < uvCount; ++u )
						{
							FbxVector2 uv = elemUv->GetDirectArray().GetAt( u );
							meshPart.uvs.pushBack( zpVector2f( (zp_float)uv.mData[0], (zp_float)uv.mData[1] ) );
						}

						meshPart.uvIndecies.reserve( polygonCount * mesh->GetPolygonSize( 0 ) );
						for( zp_int p = 0; p < polygonCount; ++p )
						{
							zp_int polygonSize = mesh->GetPolygonSize( p );
							for( zp_int s = 0; s < polygonSize; ++s )
							{
								index = mesh->GetTextureUVIndex( p, s );
								meshPart.uvIndecies.pushBack( index );
							}
						}
					}
					break;
				}
			}

			// flip uvs
			if( flipUVs )
			{
				zpVector2f* b = meshPart.uvs.begin();
				zpVector2f* e = meshPart.uvs.end();
				for( ; b != e; ++b )
				{
					b->setY( 1.0f - b->getY() );
				}
			}

			// normals
			for( zp_int n = 0; n < normalCount; ++n )
			{
				FbxGeometryElementNormal* elemNormal = mesh->GetElementNormal( n );
				zp_int index = -1;
				switch( elemNormal->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					{
						meshPart.normals.reserve( controlPointCount );
						meshPart.normIndecies.reserve( controlPointCount );
						for( zp_int i = 0; i < controlPointCount; ++i )
						{
							switch( elemNormal->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = i;
								break;	
							case FbxGeometryElement::eIndexToDirect:
								index = elemNormal->GetIndexArray().GetAt( i );
								break;
							}

							meshPart.normIndecies.pushBack( index );

							FbxVector4 norm = elemNormal->GetDirectArray().GetAt( i );
							meshPart.normals.pushBack( zpVector4f( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 ) );
						}
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
					{
						//zp_int normCount = elemNormal->GetDirectArray().GetCount();
						//meshPart.normals.reserve( normCount );
						//for( zp_int u = 0; u < normCount; ++u )
						//{
						//	FbxVector4 norm = elemNormal->GetDirectArray().GetAt( u );
						//	meshPart.normals.pushBack( zpVector4f( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 ) );
						//}

						meshPart.normIndecies.reserve( meshPart.vertIndecies.size() );
						meshPart.normals.reserve( polygonCount * mesh->GetPolygonSize( 0 ) );
						meshPart.normals.resize( meshPart.verts.size() );
						for( zp_int p = 0; p < polygonCount; ++p )
						{
							zp_int polygonSize = mesh->GetPolygonSize( p );
							for( zp_int s = 0; s < polygonSize; ++s )
							{
								FbxVector4 norm;
								zp_int idx = mesh->GetPolygonVertex( p, s );
								mesh->GetPolygonVertexNormal( p, s, norm );
								meshPart.normals[ idx ] = ( zpVector4f( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 ) );
							}
						}
					}
					break;
				}
			}

			// binormals
			for( zp_int b = 0; b < binormalCount; ++b )
			{
				FbxGeometryElementBinormal* elemBinormal = mesh->GetElementBinormal( b );
				zp_int index = -1;
				switch( elemBinormal->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					{
						meshPart.normals.reserve( controlPointCount );
						for( zp_int i = 0; i < controlPointCount; ++i )
						{
							switch( elemBinormal->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = i;
								break;	
							case FbxGeometryElement::eIndexToDirect:
								index = elemBinormal->GetIndexArray().GetAt( i );
								break;
							}

							FbxVector4 norm = elemBinormal->GetDirectArray().GetAt( index );
							meshPart.binormals.pushBack( zpVector4f( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 ) );
						}
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
					break;
				}
			}

			// tangents
			for( zp_int t = 0; t < tangentCount; ++t )
			{
				FbxGeometryElementTangent* elemTangent = mesh->GetElementTangent( t );
				zp_int index = -1;
				switch( elemTangent->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					{
						meshPart.tangents.reserve( controlPointCount );
						for( zp_int i = 0; i < controlPointCount; ++i )
						{
							switch( elemTangent->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = i;
								break;	
							case FbxGeometryElement::eIndexToDirect:
								index = elemTangent->GetIndexArray().GetAt( i );
								break;
							}

							FbxVector4 norm = elemTangent->GetDirectArray().GetAt( index );
							meshPart.tangents.pushBack( zpVector4f( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 ) );
						}
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
					break;
				}
			}

			// colors
			for( zp_int c = 0; c < colorCount; ++c )
			{
				FbxGeometryElementVertexColor* elemColor = mesh->GetElementVertexColor( c );
				zp_int index = -1;
				switch( elemColor->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					{
						meshPart.colors.reserve( controlPointCount );
						for( zp_int i = 0; i < controlPointCount; ++i )
						{
							switch( elemColor->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = i;
								break;	
							case FbxGeometryElement::eIndexToDirect:
								index = elemColor->GetIndexArray().GetAt( i );
								break;
							}

							FbxColor color = elemColor->GetDirectArray().GetAt( index );
							meshPart.colors.pushBack( zpColor4f( (zp_float)color.mRed, (zp_float)color.mGreen, (zp_float)color.mBlue, (zp_float)color.mAlpha ) );
						}
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
					break;
				}
			}

			// materials
			for( zp_int m = 0; m < materialCount; ++m )
			{
				FbxGeometryElementMaterial* elemMaterial = mesh->GetElementMaterial( m );
				zp_int index = -1;
				switch( elemMaterial->GetMappingMode() )
				{
				case FbxGeometryElement::eByControlPoint:
					{
						//meshPart.colors.reserve( meshPart.colors.size() + controlPointCount );
						//for( zp_int i = 0; i < controlPointCount; ++i )
						//{
						//	switch( elemMaterial->GetReferenceMode() )
						//	{
						//	case FbxGeometryElement::eDirect:
						//		index = i;
						//		break;	
						//	case FbxGeometryElement::eIndexToDirect:
						//		index = elemMaterial->GetIndexArray().GetAt( i );
						//		break;
						//	}
						//
						//	FbxColor color = elemMaterial->GetDirectArray().GetAt( index );
						//	meshPart.colors.pushBack( zpColor4f( (zp_float)color.mRed, (zp_float)color.mGreen, (zp_float)color.mBlue, (zp_float)color.mAlpha ) );
						//}
					}
					break;
				case FbxGeometryElement::eAllSame:
					{
						switch( elemMaterial->GetReferenceMode() )
						{
						case FbxGeometryElement::eDirect:
							index = 0;
							break;	
						case FbxGeometryElement::eIndexToDirect:
							index = elemMaterial->GetIndexArray().GetAt( 0 );
							break;
						}

						zp_int matCount = mesh->GetNode()->GetMaterialCount();
						meshPart.materialData.materialNames.reserve( matCount );

						for( zp_int m = 0; m < matCount; ++m )
						{
							FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial( index );
							const zp_char* name = surfaceMaterial->GetName();
							meshPart.materialData.materialNames.pushBackEmpty() = name;
						}

						meshPart.materialData.polygonIndexToMaterialName.reserve( polygonCount );
						for( zp_int p = 0; p < polygonCount; ++p )
						{
							meshPart.materialData.polygonIndexToMaterialName.pushBack( index );
						}
					}
					break;
				case FbxGeometryElement::eByPolygon:
					{
						zp_int matCount = mesh->GetNode()->GetMaterialCount();
						meshPart.materialData.materialNames.reserve( matCount );

						for( zp_int m = 0; m < matCount; ++m )
						{
							FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial( m );
							const zp_char* name = surfaceMaterial->GetName();
							meshPart.materialData.materialNames.pushBackEmpty() = name;
						}

						meshPart.materialData.polygonIndexToMaterialName.reserve( polygonCount );
						for( zp_int p = 0; p < polygonCount; ++p )
						{
							switch( elemMaterial->GetReferenceMode() )
							{
							case FbxGeometryElement::eDirect:
								index = p;
								break;	
							case FbxGeometryElement::eIndexToDirect:
								index = elemMaterial->GetIndexArray().GetAt( p );
								break;
							}
							
							meshPart.materialData.polygonIndexToMaterialName.pushBack( index );
							//FbxProperty p = surfaceMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse );
							//
							//zp_int count;
							//count = p.GetSrcObjectCount< FbxLayeredTexture >();
							//if( count > 0 )
							//{
							//	for( zp_int lc = 0; lc < count; ++lc )
							//	{
							//		FbxLayeredTexture* t = p.GetSrcObject< FbxLayeredTexture >( lc );
							//		zp_printf( t->GetName() );
							//	}
							//}
							//else
							//{
							//	count = p.GetSrcObjectCount< FbxFileTexture >();
							//	if( count > 0 )
							//	{
							//		for( zp_int tc = 0; tc < count; ++tc )
							//		{
							//			FbxFileTexture* t = p.GetSrcObject< FbxFileTexture >( tc );
							//			zp_printf( t->GetFileName() );
							//		}
							//	}
							//}
						}
						
					}
					break;
				}
			}
		}

		_getFbxInfo( data, child, flipUVs );
	}
}

VertexFormat _fbxToMesh( FbxMeshData* data, MeshData* mesh )
{
	zp_int fmt = VF_NONE;
	FbxMeshDataPart& part = data->parts[ 0 ];

	if( !part.verts.isEmpty() )
	{
		fmt |= VF_VERTEX;
	}
	
	if( !part.normals.isEmpty() )
	{
		fmt |= VF_NORMAL;
	}
	
	if( !part.uvs.isEmpty() )
	{
		fmt |= VF_TEXTURE;
	}
	
	if( !part.colors.isEmpty() )
	{
		fmt |= VF_COLOR;
	}

	mesh->parts.reserve( data->parts.size() );

	zp_int materialIndex = -1;
	zp_int vertexOffset = 0;
	zp_int vertexCount = 0;
	zp_int indexOffset = 0;
	zp_int indexCount = 0;

	for( zp_int p = 0, pmax = data->parts.size(); p < pmax; ++p )
	{
		switch( fmt )
		{
		case VF_VERTEX:
		
			break;
		case VF_VERTEX_COLOR:

			break;
		case VF_VERTEX_NORMAL:

			break;
		case VF_VERTEX_TEXTURE:

			break;
		case VF_VERTEX_NORMAL_TEXTURE:
			{
				zp_int stride = sizeof( zpVector4f ) + sizeof( zpVector4f ) + sizeof( zpVector2f );

				mesh->parts.reserve( part.materialData.materialNames.size() );

				for( zp_int i = 0, imax = part.vertIndecies.size(); i < imax; ++i )
				{
					zp_int idx = part.vertIndecies[ i ];
					mesh->index.write< zp_ushort >( idx );
				}

				for( zp_int i = 0, imax = part.verts.size(); i < imax; ++i )
				{
					zpVector4f& vert = part.verts[ i ];
					zpVector4f& norm = part.normals[ i ];
					zpVector2f& uv   = part.uvs[ i ];

					mesh->vertex.write( vert );
					mesh->vertex.write( norm );
					mesh->vertex.write( zpVector2f( 0, 0 ) );
				}

				part.normIndecies.sort( []( zp_int a, zp_int b ) { return a < b; } );
				for( zp_int i = 0, imax = part.normIndecies.size(); i < imax; ++i )
				{
					zp_int offset = part.normIndecies[ i ];
					mesh->vertex.writeAt( part.normals[ offset ], offset * stride + sizeof( zpVector4f ) );
				}

				//for( zp_int i = 0, imax = part.uvIndecies.size(); i < imax; ++i )
				//{
				//	zp_int offset = part.uvIndecies[ i ];
				//	mesh->vertex.writeAt( part.uvs[ offset ], offset * stride + sizeof( zpVector4f ) + sizeof( zpVector4f ) );
				//}

				for( zp_int i = 0, imax = part.vertIndecies.size(); i < imax; ++i )
				{
					zp_int matIdx = part.materialData.polygonIndexToMaterialName[ i / 3 ];
					if( materialIndex != matIdx )
					{
						if( !mesh->parts.isEmpty() )
						{
							MeshDataPart& end = mesh->parts.back();
							end.indexCount = indexCount;
							end.vertexCount = vertexCount;
						}

						MeshDataPart& dp = mesh->parts.pushBackEmpty();

						zpFixedStringBuffer< 255 > mat;
						mat.append( "materials" );
						mat.append( zpFile::sep );
						mat.append( part.materialData.materialNames[ matIdx ] );
						mat.append( ".materialb" );

						dp.material = mat.str();

						dp.indexOffset = indexOffset * sizeof( zp_ushort );//mesh->index.size();
						dp.vertexOffset = 0;//mesh->vertex.size();

						vertexCount = 0;
						indexCount = 0;

						materialIndex = matIdx;
					}

					//zp_uint offset = vertexOffset * stride;
					//mesh->vertex.writeAt( part.normals[ part.normIndecies[ i ] ], offset + sizeof( zpVector4f ) );
					//mesh->vertex.writeAt( part.uvs[ part.uvIndecies[ i ] ], offset + sizeof( zpVector4f ) + sizeof( zpVector4f ) );

					++vertexCount;

					++vertexOffset;

					++indexCount;

					++indexOffset;

					zpVector4f& vert = part.verts[ part.vertIndecies[ i ] ];
					mesh->parts.back().boundingBox.add( vert );
				}

				MeshDataPart& end = mesh->parts.back();
				end.indexCount = indexCount;
				end.vertexCount = vertexCount;
			}
			break;
		default:
			ZP_ASSERT( false, "Unknown vertex format for FBX" );
			break;
		}
	}

	return (VertexFormat)fmt;
}

zp_bool FbxMessCompiler::compileMesh()
{
	zp_bool ok = false;

	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* ioSettings = FbxIOSettings::Create( manager, IOSROOT );
	ioSettings->SetBoolProp( IMP_FBX_MATERIAL, true );
	ioSettings->SetBoolProp( IMP_FBX_TEXTURE, true );
	ioSettings->SetBoolProp( IMP_FBX_LINK, true );
	ioSettings->SetBoolProp( IMP_FBX_SHAPE, true );
	ioSettings->SetBoolProp( IMP_FBX_GOBO, true );
	ioSettings->SetBoolProp( IMP_FBX_ANIMATION, true );
	ioSettings->SetBoolProp( IMP_FBX_MATERIAL, true );
	ioSettings->SetBoolProp( IMP_FBX_GLOBAL_SETTINGS, true );
	manager->SetIOSettings( ioSettings );

	FbxImporter* importer = FbxImporter::Create( manager, "" );
	importer->Initialize( m_inputFile.str(), -1, manager->GetIOSettings() );

	zp_int animStackCount = importer->GetAnimStackCount();

	FbxScene* scene = FbxScene::Create( manager, "tempName" );

	importer->Import( scene );
	importer->Destroy();

	FbxAxisSystem axis( FbxAxisSystem::eDirectX );
	axis.ConvertScene( scene );

	FbxNode* rootNode = scene->GetRootNode();
	m_fbxData.parts.reserve( 10 );
	_getFbxInfo( &m_fbxData, rootNode, true );

	VertexFormat fmt = _fbxToMesh( &m_fbxData, &m_data );
	formatToString( fmt, m_data.format );

	ok = fmt != VF_NONE;

	manager->Destroy();

	return ok;
}
