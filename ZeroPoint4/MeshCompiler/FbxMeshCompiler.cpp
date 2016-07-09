#include "Main.h"

#include <fbxsdk.h>

#pragma comment( lib, "libfbxsdk-md" )

FbxMeshCompiler::~FbxMeshCompiler()
{
}

void _getFbxAnimationSkeletonData( zpFbxAnimationKeyFrames* keyFrames, FbxAnimEvaluator* eval, FbxNode* node, zp_float frameRate, zp_float startTime, zp_float endTime )
{
    FbxSkeleton* skeleton = node->GetSkeleton();
    if( skeleton )
    {
        const zp_char* nodeName = node->GetName();
        if( keyFrames->boneName == nodeName )
        {
            FbxTime time;
            zp_float fps = 1.f / frameRate;
            zp_float t = startTime;
            ZP_ALIGN16 zp_float frameData[ 16 ];

            while( t <= endTime )
            {
                time.SetSecondDouble( t );

                FbxAMatrix globalMatrix = eval->GetNodeGlobalTransform( node, time );
                FbxAMatrix globalParentMatrix = eval->GetNodeGlobalTransform( node->GetParent(), time );
                FbxAMatrix globalInvParentMatrix = globalParentMatrix.Inverse();
                FbxAMatrix localMatrix = globalInvParentMatrix * globalMatrix;

                FbxVector4 trans = localMatrix.GetT();
                FbxQuaternion rot = localMatrix.GetQ();
                FbxVector4 scale = localMatrix.GetS();

                zpFbxAnimationKeyFrame& keyFrame = keyFrames->frames.pushBackEmpty();
                keyFrame.position = zpMath::Vector4( trans[ 0 ], trans[ 1 ], trans[ 2 ], 1.f );
                keyFrame.rotataion = zpMath::Quaternion( rot[ 0 ], rot[ 1 ], rot[ 2 ], rot[ 3 ] );
                keyFrame.scale = zpMath::Vector4( scale[ 0 ], scale[ 1 ], scale[ 2 ], 0.f );

                t += fps;
            }

            return;
        }
    }

    zp_int numChildren = node->GetChildCount();
    for( zp_int i = 0; i < numChildren; ++i )
    {
        FbxNode* child = node->GetChild( i );
        _getFbxAnimationSkeletonData( keyFrames, eval, child, frameRate, startTime, endTime );
    }
}

void _getFbxAnimationData( zpFbxMeshData* data, const zp_char* animationName, FbxAnimEvaluator* eval, FbxNode* root, zp_float frameRate, zp_float startTime, zp_float endTime )
{
    zpFbxAnimation& anim = data->animations.pushBackEmpty();

    anim.frameRate = frameRate;
    anim.name = animationName;

    zp_int keyFrames = zp_floor_to_int( ( endTime - startTime ) * frameRate );
    zp_int numBones = data->skeleton.bones.size();

    anim.boneFrames.reserve( numBones );
    for( zp_int b = 0; b < numBones; ++b )
    {
        zpFbxAnimationKeyFrames& boneFrames = anim.boneFrames.pushBackEmpty();
        boneFrames.boneName = data->skeleton.bones[ b ].name;

        boneFrames.frames.reserve( keyFrames );

        _getFbxAnimationSkeletonData( &boneFrames, eval, root, frameRate, startTime, endTime );
    }
}

void _getFbxAnimationsData( zpFbxMeshData* data, FbxScene* scene, FbxImporter* importer, FbxNode* root )
{
    zp_float frameRate = (zp_float)FbxTime::GetFrameRate( scene->GetGlobalSettings().GetTimeMode() );
    
    FbxAnimEvaluator* eval = scene->GetAnimationEvaluator();

    FbxArray< FbxString* > takeArray;
    scene->FillAnimStackNameArray( takeArray );

    zp_int numAnimations = scene->GetSrcObjectCount< FbxAnimStack >();
    for( zp_int i = 0; i < numAnimations; ++i )
    {
        FbxAnimStack* animStack = scene->GetSrcObject< FbxAnimStack >( i );

        for( zp_int t = 0; t < takeArray.GetCount(); ++t )
        {
            FbxString* takeName = takeArray.GetAt( t );

            FbxTakeInfo* takeInfo = scene->GetTakeInfo( *takeName );

            FbxTime startTime;
            FbxTime endTime;
            
            if( takeInfo )
            {
                startTime = takeInfo->mLocalTimeSpan.GetStart();
                endTime = takeInfo->mLocalTimeSpan.GetStop();
            }
            else
            {
                FbxTimeSpan timeSpan;
                scene->GetGlobalSettings().GetTimelineDefaultTimeSpan( timeSpan );
                startTime = timeSpan.GetStart();
                endTime = timeSpan.GetStop();
            }

            zp_float s = (zp_float)startTime.GetSecondDouble();
            zp_float e = (zp_float)endTime.GetSecondDouble();

            if( s < e )
            {
                _getFbxAnimationData( data, takeName->Buffer(), eval, root, frameRate, s, e );
            }
        }

#if 0
        const zp_char* animName = animStack->GetName();

        zp_int numLayers = animStack->GetMemberCount();
        for( zp_int l = 0; l < numLayers; ++l )
        {
            FbxAnimLayer* animLayer = animStack->GetMember< FbxAnimLayer >( l );

        }
#endif
    }
}

void _getFbxMeshSkinData( zpFbxMeshData* data, FbxNode* node, FbxNode* parent, zp_int depth )
{
    FbxMesh* mesh = node->GetMesh();
    if( mesh )
    {
        ZP_ALIGN16 zp_float boneData[ 16 ];

        zp_int deformerCount = mesh->GetDeformerCount();
        for( zp_int i = 0; i < deformerCount; ++i )
        {
            FbxDeformer* deformer = mesh->GetDeformer( i );

            if( deformer->GetDeformerType() != FbxDeformer::eSkin ) continue;

            FbxSkin* skin = (FbxSkin*)deformer;
            zp_int clusterCount = skin->GetClusterCount();

            data->skeleton.bones.reserve( data->skeleton.bones.size() + clusterCount );
            for( zp_int c = 0; c < clusterCount; ++c )
            {
                FbxCluster* cluster = skin->GetCluster( c );
                FbxCluster::ELinkMode linkMode = cluster->GetLinkMode();
                const zp_char* boneName = cluster->GetLink()->GetName();

                zpFbxBone* bone;
                zp_bool found = data->skeleton.bones.findIf( [ boneName ]( zpFbxBone& bone )
                {
                    return bone.name == boneName;
                }, &bone );

                // if the bone was found, set bind pose
                if( found )
                {
                    FbxAMatrix linkMatrix;
                    cluster->GetTransformLinkMatrix( linkMatrix );
                    zp_double* matData = (zp_double*)linkMatrix;

                    for( zp_int i = 0; i < 16; ++i )
                    {
                        boneData[i] = (zp_float)matData[i];
                    }

                    bone->bindPose = zpMath::MatrixLoadOpenGL( boneData );

                    zp_int indexCount = cluster->GetControlPointIndicesCount();
                    zp_int *indices = cluster->GetControlPointIndices();
                    zp_double *weights = cluster->GetControlPointWeights();

                    bone->controlPointIndices.reserve( indexCount );
                    bone->controlPointWeights.reserve( indexCount );

                    for( zp_int j = 0 ; j < indexCount ; ++j )
                    {
                        zp_int vertex = indices[ j ];
                        zp_float weight = (zp_float)weights[ j ];

                        bone->controlPointIndices.pushBack( vertex );
                        bone->controlPointWeights.pushBack( weight );
                    }
                }
                else
                {
                    zpLog::error() << "Unable to find bone: " << boneName << zpLog::endl;
                }
            }
        }
    }

    zp_int numChildren = node->GetChildCount();
    for( zp_int i = 0; i < numChildren; ++i )
    {
        FbxNode* child = node->GetChild( i );
        _getFbxMeshSkinData( data, child, node, depth + 1 );
    }
}

void _getFbxMeshSkeletonData( zpFbxMeshData* data, FbxNode* node, FbxNode* parent, zp_int depth )
{
    FbxSkeleton* skeleton = node->GetSkeleton();
    if( skeleton )
    {
        zpFbxBone& bone = data->skeleton.bones.pushBackEmpty();
        bone.name = node->GetName();

        if( parent )
        {
            bone.parent = parent->GetName();
        }
        else if( data->skeleton.rootBone.isEmpty() )
        {
            data->skeleton.rootBone = bone.name;
        }

        //for( zp_int i = 0; i < depth; ++i ) zp_printf( ".." );
        //zp_printfln( "%s -> %s", bone.name.str(), bone.parent.str() );
    }

    zp_int numChildren = node->GetChildCount();
    for( zp_int i = 0; i < numChildren; ++i )
    {
        FbxNode* child = node->GetChild( i );
        _getFbxMeshSkeletonData( data, child, skeleton ? node : ZP_NULL, depth + 1 );
    }
}

void _getFbxMeshData( zpFbxMeshData* data, FbxNode* node, zp_bool flipUVs )
{
    FbxMesh* mesh = node->GetMesh();
    if( mesh )
    {
        zpFbxMeshDataPart& meshPart = data->parts.pushBackEmpty();

        zp_int controlPointCount = mesh->GetControlPointsCount();
        zp_int polygonCount = mesh->GetPolygonCount();
        zp_int normalCount = mesh->GetElementNormalCount();
        zp_int binormalCount = mesh->GetElementBinormalCount();
        zp_int tangentCount = mesh->GetElementTangentCount();
        zp_int colorCount = mesh->GetElementVertexColorCount();
        zp_int uvCount = mesh->GetElementUVCount();
        zp_int materialCount = mesh->GetElementMaterialCount();
        zp_int polygonSize = mesh->GetPolygonSize( 0 );

        // verts
        {
            meshPart.verts.reserve( controlPointCount );

            FbxVector4* controlPoints = mesh->GetControlPoints();
            for( zp_int c = 0; c < controlPointCount; ++c )
            {
                FbxVector4& v = controlPoints[ c ];
                meshPart.verts.pushBack( zpMath::Vector4( (zp_float)v[0], (zp_float)v[1], (zp_float)v[2], 1 ) );
            }

            // vert indices
            meshPart.vertIndecies.reserve( polygonCount * polygonSize );
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
        }

        // uvs
        {
            for( zp_int u = 0; u < uvCount; ++u )
            {
                FbxGeometryElementUV* elemUv = mesh->GetElementUV( u );

                zp_int uvCount = elemUv->GetDirectArray().GetCount();
                meshPart.uvs.reserve( uvCount );
                for( zp_int u = 0; u < uvCount; ++u )
                {
                    FbxVector2 uv = elemUv->GetDirectArray().GetAt( u );
                    meshPart.uvs.pushBack( zpVector2f( (zp_float)uv.mData[0], (zp_float)uv.mData[1] ) );
                }

                zp_int index = -1;
                switch( elemUv->GetMappingMode() )
                {
                case FbxGeometryElement::eByControlPoint:
                    {
                        meshPart.uvIndecies.reserve( polygonCount * polygonSize );
                        for( zp_int p = 0; p < polygonCount; ++p )
                        {
                            zp_int polygonSize = mesh->GetPolygonSize( p );
                            for( zp_int s = 0; s < polygonSize; ++s )
                            {
                                zp_int polyIdx = mesh->GetPolygonVertex( p, s );
                                switch( elemUv->GetReferenceMode() )
                                {
                                case FbxGeometryElement::eDirect:
                                    index = elemUv->GetIndexArray().GetAt( polyIdx );
                                    break;    
                                case FbxGeometryElement::eIndexToDirect:
                                    index = polyIdx;
                                    break;
                                }

                                meshPart.uvIndecies.pushBack( index );
                            }
                        }
                    }
                    break;
                case FbxGeometryElement::eByPolygonVertex:
                    {
                        meshPart.uvIndecies.reserve( polygonCount * mesh->GetPolygonSize( 0 ) );
                        for( zp_int p = 0; p < polygonCount; ++p )
                        {
                            zp_int polygonSize = mesh->GetPolygonSize( p );
                            for( zp_int s = 0; s < polygonSize; ++s )
                            {
                                zp_int polyIdx = mesh->GetTextureUVIndex( p, s );
                                switch( elemUv->GetReferenceMode() )
                                {
                                case FbxGeometryElement::eDirect:
                                    index = elemUv->GetIndexArray().GetAt( polyIdx );
                                    break;    
                                case FbxGeometryElement::eIndexToDirect:
                                    index = polyIdx;
                                    break;
                                }

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
        }

        // normals
        {
            zpArrayList< zp_hash > allNormalHashes;
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
                            meshPart.normals.pushBack( zpMath::Vector4( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 ) );
                        }
                    }
                    break;
                case FbxGeometryElement::eByPolygonVertex:
                    {
                        zp_int normIdx = 0;
                        meshPart.normIndecies.reserve( meshPart.vertIndecies.size() );
                        meshPart.normals.reserve( polygonCount * polygonSize );
                        for( zp_int p = 0; p < polygonCount; ++p )
                        {
                            zp_int polygonSize = mesh->GetPolygonSize( p );
                            for( zp_int s = 0; s < polygonSize; ++s )
                            {
                                FbxVector4 norm;
                                mesh->GetPolygonVertexNormal( p, s, norm );

                                zpVector4f normal = zpMath::Vector4( (zp_float)norm.mData[0], (zp_float)norm.mData[1], (zp_float)norm.mData[2], 0 );

                                zp_hash normalHash = zp_fnv1_32( normal, 0 );

                                zp_int idx = allNormalHashes.indexOf( normalHash );
                                if( idx < 0 )
                                {
                                    idx = allNormalHashes.size();
                                    allNormalHashes.pushBack( normalHash );
                                    meshPart.normals.pushBack( normal );
                                }

                                meshPart.normIndecies.pushBack( idx );
                            }
                        }
                    }
                    break;
                }
            }
        }

        // material
        {
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
                        //    switch( elemMaterial->GetReferenceMode() )
                        //    {
                        //    case FbxGeometryElement::eDirect:
                        //        index = i;
                        //        break;    
                        //    case FbxGeometryElement::eIndexToDirect:
                        //        index = elemMaterial->GetIndexArray().GetAt( i );
                        //        break;
                        //    }
                        //
                        //    FbxColor color = elemMaterial->GetDirectArray().GetAt( index );
                        //    meshPart.colors.pushBack( zpColor4f( (zp_float)color.mRed, (zp_float)color.mGreen, (zp_float)color.mBlue, (zp_float)color.mAlpha ) );
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
                            //    for( zp_int lc = 0; lc < count; ++lc )
                            //    {
                            //        FbxLayeredTexture* t = p.GetSrcObject< FbxLayeredTexture >( lc );
                            //        zp_printf( t->GetName() );
                            //    }
                            //}
                            //else
                            //{
                            //    count = p.GetSrcObjectCount< FbxFileTexture >();
                            //    if( count > 0 )
                            //    {
                            //        for( zp_int tc = 0; tc < count; ++tc )
                            //        {
                            //            FbxFileTexture* t = p.GetSrcObject< FbxFileTexture >( tc );
                            //            zp_printf( t->GetFileName() );
                            //        }
                            //    }
                            //}
                        }
                    }
                }
                break;
            }
        }
    }

    zp_int numChildren = node->GetChildCount();
    for( zp_int i = 0; i < numChildren; ++i )
    {
        FbxNode* child = node->GetChild( i );
        _getFbxMeshData( data, child, flipUVs );
    }
}

VertexFormat _fbxToMeshData( const zpFbxMeshData* data, MeshData* mesh, MeshSkeleton* skeleton, MeshAnimation* animation )
{
    zp_int fmt = VF_NONE;
    const zpFbxMeshDataPart* part = &data->parts[ 0 ];

    if( !part->verts.isEmpty() )
    {
        fmt |= VF_VERTEX;
    }

    if( !part->normals.isEmpty() )
    {
        fmt |= VF_NORMAL;
    }

    if( !part->uvs.isEmpty() )
    {
        fmt |= VF_TEXTURE;
    }

    if( !part->colors.isEmpty() )
    {
        fmt |= VF_COLOR;
    }
    
    zpArrayList< zp_int > newIndexBuffer;
    zpArrayList< zp_int > newIndexToMaterialName;
    zpArrayList< zpVertexNormalTexture > newUniqueVertexBuffer;
    zpArrayList< zp_hash > newUniqueVertexHashes;
    zpArrayList< zpArrayList< zp_int > > vertexIndexToUniqueVertex;

    zp_uint stride = 0;
    zp_uint vertexOffset = 0;
    zp_uint vertexCount = 0;
    zp_uint indexOffset = 0;
    zp_uint indexCount = 0;

    struct CompressedMeshPart
    {
        zp_uint vertexOffset;
        zp_uint vertexCount;
        zp_uint indexOffset;
        zp_uint indexCount;
    };
    zpArrayList< CompressedMeshPart > newMeshParts;
    newMeshParts.reserve( data->parts.size() );

    // fill unified vertex and index buffers
    for( zp_int p = 0, pmax = data->parts.size(); p < pmax; ++p )
    {
        CompressedMeshPart& compPart = newMeshParts.pushBackEmpty();
        compPart.vertexOffset = mesh->vertex.size();
        compPart.indexOffset = mesh->index.size();

        part = &data->parts[ p ];

        zp_int uniqueCount = 0;

        switch( fmt )
        {
        case VF_VERTEX:
            {
                ZP_ASSERT( false, "" );
            }
            break;
        case VF_VERTEX_COLOR:
            {
                ZP_ASSERT( false, "" );
            }
            break;
        case VF_VERTEX_NORMAL:
            {
                ZP_ASSERT( false, "" );
            }
            break;
        case VF_VERTEX_TEXTURE:
            {
                ZP_ASSERT( false, "" );
            }
            break;
        case VF_VERTEX_NORMAL_TEXTURE:
            {
                stride = sizeof( zpVector4f ) + sizeof( zpVector4f ) + sizeof( zpVector2f );

                // find all unique vertex/norm/uv combos
                for( zp_uint v = 0; v < part->vertIndecies.size(); ++v )
                {
                    zp_int vertIdx = part->vertIndecies[ v ];
                    zp_int normIdx = part->normIndecies[ v ];
                    zp_int uvIdx   = part->uvIndecies[ v ];

                    zp_hash h = 0;
                    h = zp_fnv1_32( vertIdx, h );
                    h = zp_fnv1_32( normIdx, h );
                    h = zp_fnv1_32( uvIdx, h );

                    if( newUniqueVertexHashes.indexOf( h ) < 0 )
                    {
                        newUniqueVertexHashes.pushBack( h );

                        ZP_ALIGN16 zp_float floatData[4];
                        ZP_ALIGN16 zp_float normalData[4];
                        zpMath::Vector4Store4( part->verts[ vertIdx ], floatData );
                        zpMath::Vector4Store4( part->normals[ normIdx ], normalData );

                        // write vertex data
                        mesh->vertex.writeBulk( floatData, 4 );
                        mesh->vertex.writeBulk( normalData, 4 );
                        mesh->vertex.write( part->uvs[ uvIdx ] );

                        ++uniqueCount;
                        
                        zpVertexNormalTexture& o = newUniqueVertexBuffer.pushBackEmpty();
                        o.v = part->verts[ vertIdx ];
                        o.n = part->normals[ normIdx ];
                        o.t = part->uvs[ uvIdx ];
                    }
                }

                // rebuild index buffer by finding unique vertex indices
                newIndexBuffer.reserve( newIndexBuffer.size() + part->vertIndecies.size() );
                newIndexToMaterialName.reserve( newIndexToMaterialName.size() + part->vertIndecies.size() );
                for( zp_uint v = 0; v < part->vertIndecies.size(); ++v )
                {
                    zp_int vertIdx = part->vertIndecies[ v ];
                    zp_int normIdx = part->normIndecies[ v ];
                    zp_int uvIdx   = part->uvIndecies[ v ];
                    zp_int matIdx  = part->materialData.polygonIndexToMaterialName[ v / 3 ];

                    zp_hash h = 0;
                    h = zp_fnv1_32( vertIdx, h );
                    h = zp_fnv1_32( normIdx, h );
                    h = zp_fnv1_32( uvIdx, h );

                    zp_int idx = newUniqueVertexHashes.indexOf( h );
                    newIndexBuffer.pushBack( idx );
                    newIndexToMaterialName.pushBack( matIdx );

                    // write index data
                    mesh->index.write< zp_ushort >( idx );

                    // map old vertex index to new unified index
                    while( vertexIndexToUniqueVertex.size() < (zp_uint)( vertIdx + 1 ) )
                    {
                        vertexIndexToUniqueVertex.pushBackEmpty();
                    }

                    // don't add multiple remapped verts
                    if( vertexIndexToUniqueVertex[ vertIdx ].indexOf( idx ) < 0 )
                    {
                        vertexIndexToUniqueVertex[ vertIdx ].pushBackEmpty() = idx;
                    }
                }
            }
            break;
        default:
            ZP_ASSERT( false, "Unknown vertex format for FBX" );
            break;
        }

        compPart.vertexCount = uniqueCount;
        compPart.indexCount = part->vertIndecies.size();
    }

    // reserve for each part
    mesh->parts.reserve( data->parts.size() );

    // fill out mesh parts from all parts and material changes in parts
    for( zp_int p = 0, pmax = data->parts.size(); p < pmax; ++p )
    {
        part = &data->parts[ p ];
        if( part->materialData.materialNames.isEmpty() ) continue;

        zp_int materialIndex = -1;

        // reserver for more more than one material
        mesh->parts.reserve( mesh->parts.size() + ( part->materialData.materialNames.size() - 1 ) );

        // sub-divide compressed parts into final material parts
        CompressedMeshPart& compPart = newMeshParts[ p ];
        zp_int idx = compPart.indexOffset / sizeof( zp_ushort );
        zp_int count = 0;
        for( zp_int i = 0, imax = compPart.indexCount; i < imax; ++i, ++count )
        {
            zp_int matIdx = newIndexToMaterialName[ idx + i ];
            if( materialIndex != matIdx )
            {
                if( !mesh->parts.isEmpty() )
                {
                    MeshDataPart& end = mesh->parts.back();
                    end.indexCount = count;
                    end.vertexCount = count;
                }

                MeshDataPart& dp = mesh->parts.pushBackEmpty();

                zpFixedStringBuffer< 255 > mat;
                mat.append( "materials" );
                mat.append( zpFile::sep );
                mat.append( part->materialData.materialNames[ matIdx ] );
                mat.append( ".materialb" );

                dp.material = mat.str();
                dp.boundingBox.reset();

                dp.indexOffset = compPart.indexOffset + i * sizeof( zp_ushort );//mesh->index.size();
                dp.vertexOffset = 0;

                count = 0;

                materialIndex = matIdx;
            }

            zpVector4f& vert = newUniqueVertexBuffer[ newIndexBuffer[ idx + i ] ].v;
            mesh->parts.back().boundingBox.add( vert );
        }

        MeshDataPart& end = mesh->parts.back();
        end.indexCount = count;
        end.vertexCount = count;
    }

    // remap skinned data vertex to compressed indexes
    if( !data->skeleton.bones.isEmpty() )
    {
        zp_int numBones = data->skeleton.bones.size();
        skeleton->bones.reserve( numBones );
        skeleton->boneNames.reserve( numBones );

        // store bone names
        for( zp_int s = 0; s < numBones; ++s )
        {
            skeleton->boneNames.pushBackEmpty() = data->skeleton.bones[ s ].name;
        }

        // build bone structure
        for( zp_int s = 0; s < numBones; ++s )
        {
            const zpFbxBone& bone = data->skeleton.bones[ s ];
            MeshSkeletonBone& b = skeleton->bones.pushBackEmpty();

            zp_int boneIndex = skeleton->boneNames.indexOf( bone.name );
            zp_int parentIndex = skeleton->boneNames.indexOf( bone.parent );

            b.name = boneIndex;
            b.parent = parentIndex;

            b.bindPose = bone.bindPose;

            b.controlPointIndicies.reserve( bone.controlPointIndices.size() );
            b.controlPointWeights.reserve( bone.controlPointIndices.size() );

            for( zp_int c = 0, cmax = bone.controlPointIndices.size(); c < cmax; ++c )
            {
                zp_int index = bone.controlPointIndices[ c ];
                zp_float weight = bone.controlPointWeights[ c ];

                const zpArrayList< zp_int >& vee = vertexIndexToUniqueVertex[ index ];

                vee.foreach( [ &b, &weight ]( zp_int idx ) {
                    b.controlPointIndicies.pushBackEmpty() = idx;
                    b.controlPointWeights.pushBackEmpty() = weight;
                } );
            }
        }
    }

    // store animations
    if( !data->animations.isEmpty() )
    {
        zp_int numAnimations = data->animations.size();
        animation->clips.reserve( numAnimations );

        for( zp_int a = 0; a < numAnimations; ++a )
        {
            const zpFbxAnimation& anim = data->animations[ a ];

            MeshAnimationClip& meshAnim = animation->clips.pushBackEmpty();
            meshAnim.name = anim.name;
            meshAnim.frameRate = anim.frameRate;
            meshAnim.boneFrames.reserve( anim.boneFrames.size() );

            for( zp_uint f = 0; f < anim.boneFrames.size(); ++f )
            {
                MeshBoneAnimation& meshBoneAnim = meshAnim.boneFrames.pushBackEmpty();
                meshBoneAnim.boneName = anim.boneFrames[ f ].boneName;

                const zpArrayList< zpFbxAnimationKeyFrame >& fbxFrames = anim.boneFrames[ f ].frames;
                meshBoneAnim.keyFrames.reserve( fbxFrames.size() );

                for( zp_uint k = 0; k < fbxFrames.size(); ++k )
                {
                    AnimationKeyFrame& boneKeyFrame = meshBoneAnim.keyFrames.pushBackEmpty();
                    const zpFbxAnimationKeyFrame& fbxKeyFrame = fbxFrames[ k ];

                    boneKeyFrame.position = fbxKeyFrame.position;
                    boneKeyFrame.rotataion = fbxKeyFrame.rotataion;
                    boneKeyFrame.scale = fbxKeyFrame.scale;
                }
            }
        }
    }

    return (VertexFormat)fmt;
}

bool _ProgressCallback( void* pArgs, float pPercentage, const char* pStatus )
{
    zp_printfln( "%f %s", pPercentage, pStatus );
    return true;
}

zp_bool FbxMeshCompiler::compileMesh()
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
    importer->SetProgressCallback( _ProgressCallback );

    zp_int animStackCount = importer->GetAnimStackCount();
    
    FbxScene* scene = FbxScene::Create( manager, "tempName" );

    importer->Import( scene );
    importer->Destroy();

    FbxAxisSystem axis( FbxAxisSystem::eDirectX );
    axis.ConvertScene( scene );

    FbxNode* rootNode = scene->GetRootNode();
    //m_fbxData.parts.reserve( 10 );
    //_getFbxInfo( &m_fbxData, rootNode, true );

    zpFbxMeshData meshData;
    meshData.parts.reserve( 10 );

    _getFbxMeshData( &meshData, rootNode, true );

    _getFbxMeshSkeletonData( &meshData, rootNode, ZP_NULL, 0 );

    _getFbxMeshSkinData( &meshData, rootNode, ZP_NULL, 0 );

    _getFbxAnimationsData( &meshData, scene, importer, rootNode );

    VertexFormat fmt;
    fmt = _fbxToMeshData( &meshData, &m_mesh, &m_skeleton, &m_animation );
    //fmt = _fbxToMesh( &m_fbxData, &m_data );


    formatToString( fmt, m_mesh.format );

    ok = fmt != VF_NONE;

    manager->Destroy();

    return ok;
}
