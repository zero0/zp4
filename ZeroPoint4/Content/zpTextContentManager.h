#pragma once
#ifndef ZP_TEXT_CONTENT_MANAGER_H
#define ZP_TEXT_CONTENT_MANAGER_H

class zpTextResource : public zpResource< zpBison >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpTextContentManager;
};

class zpTextResourceInstance : public zpResourceInstance< zpTextResource >
{};

class zpTextContentManager : public zpContentManager< zpTextResource, zpTextResourceInstance, zpTextContentManager, 16 >
{
private:
	zp_bool createResource( zpTextResource* res, const zp_char* filename );
	void destroyResource( zpTextResource* res );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
