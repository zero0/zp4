#include "zpRendering.h"

zp_bool zpAnimationResource::load( const zp_char* filename )
{
    zp_bool ok = false;

    zpBison animationData;
    ok = animationData.readFromFile( zpString( filename ) );
    if( ok )
    {
        const zpBison::Value& data = animationData.root();

        m_resource.clips.reserve( data.size() );
        data.foreachObject( [ this ]( const zpBison::Value& key, const zpBison::Value& value ) {
            zpAnimationClip& clip = m_resource.clips.pushBackEmpty();

            clip.animationName = key.asCString();
            clip.frameRate = value[ "FrameRate" ].asFloat();
            clip.maxFrames = value[ "MaxFrames" ].asInt();

            const zpBison::Value& keyFrames = value[ "KeyFrames" ];
            clip.keyFrames.reserve( keyFrames.size() );
            keyFrames.foreachArray( [ &clip ]( const zpBison::Value& k ) {
                const zp_float* data = (const zp_float*)k.asData();
                zp_uint count = k.size() / ( sizeof( zpVector4f ) + sizeof( zpQuaternion4f ) + sizeof( zpVector4f ) );
                zpArrayList< zpMatrix4f >& frames = clip.keyFrames.pushBackEmpty();
                ZP_ALIGN16 zp_float buff[4];

                for( zp_uint i = 0; i < count; ++i )
                {
                    zp_memcpy( buff, sizeof( zpVector4f ), data, sizeof( zpVector4f ) );
                    zpVector4f animPos = zpMath::Vector4Load4( buff );
                    data += sizeof( zpVector4f );

                    zp_memcpy( buff, sizeof( zpQuaternion4f ), data, sizeof( zpQuaternion4f ) );
                    zpQuaternion4f animRot = zpMath::QuaternionLoad4( buff );
                    data += sizeof( zpQuaternion4f );
                    
                    zp_memcpy( buff, sizeof( zpVector4f ), data, sizeof( zpVector4f ) );
                    zpVector4f animScale = zpMath::Vector4Load4( buff );
                    data += sizeof( zpVector4f );

                    zpMatrix4f mat = zpMath::TRS( animPos, animRot, animScale );
                    frames.pushBackEmpty() = mat;
                }
            } );

            const zpBison::Value& boneNames = value[ "BoneNames" ];
            clip.boneNames.reserve( boneNames.size() );
            boneNames.foreachArray( [ &clip ]( const zpBison::Value& b ) {
                clip.boneNames.pushBackEmpty() = b.asCString();
            } );

            const zpBison::Value& numFrames = value[ "NumFrames" ];
            clip.numFrames.reserve( numFrames.size() );
            numFrames.foreachArray( [ &clip ]( const zpBison::Value& f ) {
                clip.numFrames.pushBackEmpty() = f.asInt();
            } );
            
        } );
    }

    return ok;
}
void zpAnimationResource::unload()
{
    m_resource.clips.clear();
}



zp_bool zpAnimationContentManager::createResource( zpAnimationResource* res, const zp_char* filename )
{
    return res->load( filename );
}
void zpAnimationContentManager::destroyResource( zpAnimationResource* res )
{
    res->unload();
}
