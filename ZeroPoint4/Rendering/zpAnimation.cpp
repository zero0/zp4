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

			const zpBison::Value& keyFrames = value[ "KeyFrames" ];
			clip.boneNames.reserve( keyFrames.size() );
			clip.keyFrames.reserve( keyFrames.size() );

			keyFrames.foreachObject( [ &clip ]( const zpBison::Value& boneName, const zpBison::Value& keyFrameList ) {
				clip.boneNames.pushBackEmpty() = boneName.asCString();

			} );
		} );
	}

	return ok;
}
void zpAnimationResource::unload()
{

}



zp_bool zpAnimationContentManager::createResource( zpAnimationResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpAnimationContentManager::destroyResource( zpAnimationResource* res )
{
	res->unload();
}
