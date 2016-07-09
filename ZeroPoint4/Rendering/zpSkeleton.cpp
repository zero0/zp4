#include "zpRendering.h"

zp_bool zpSkeletonResource::load( const zp_char* filename )
{
    zp_bool ok = false;
    
    zpBison skeletonData;
    ok = skeletonData.readFromFile( zpString( filename ) );
    if( ok )
    {
        const zpBison::Value& data = skeletonData.root();

        const zpBison::Value& indecies = data[ "Indices" ];
        const zpBison::Value& weights = data[ "Weights" ];
        const zpBison::Value& bones = data[ "Bones" ];
        const zpBison::Value& boneNames = data[ "BoneNames" ];

        // copy bone names
        m_resource.boneNames.reserve( boneNames.size() );
        boneNames.foreachArray( [ this ]( const zpBison::Value& n ) {
            m_resource.boneNames.pushBackEmpty() = n.asCString();
        } );

        // build bones
        m_resource.bones.reserve( bones.size() );
        bones.foreachArray( [ this ]( const zpBison::Value& b ) {
            zpSkeletonBone& bone = m_resource.bones.pushBackEmpty();
            bone.parent = b[ "Parent" ].asInt();
            bone.indexStart = b[ "IndexStart" ].asInt();
            bone.indexCount = b[ "IndexCount" ].asInt();
            bone.weightStart = b[ "WeightStart" ].asInt();
            bone.weightCount = b[ "WeightCount" ].asInt();
            bone.bindPose = *(const zpMatrix4f*)b[ "BindPose" ].asData();

            if( bone.parent < 0 )
            {
                m_resource.rootBoneIndex = m_resource.bones.size() - 1;
            }
        } );

        // build indecies
        const zp_int* index = (const zp_int*)indecies.asData();
        m_resource.indecies.reserve( indecies.size() );
        for( zp_int i = 0, imax = indecies.size(); i < imax; ++i )
        {
            m_resource.indecies.pushBack( index[ i ] );
        }

        // build weights
        const zp_float* weight = (const zp_float*)weights.asData();
        m_resource.weights.reserve( weights.size() );
        for( zp_int i = 0, imax = weights.size(); i < imax; ++i )
        {
            m_resource.weights.pushBack( weight[ i ] );
        }
    }

    return ok;
}
void zpSkeletonResource::unload()
{
    m_resource.indecies.clear();
    m_resource.weights.clear();
    m_resource.boneNames.clear();
    m_resource.bones.clear();
}


zp_bool zpSkeletonContentManager::createResource( zpSkeletonResource* res, const zp_char* filename )
{
    return res->load( filename );
}
void zpSkeletonContentManager::destroyResource( zpSkeletonResource* res )
{
    res->unload();
}
