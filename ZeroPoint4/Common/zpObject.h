#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

class zpObject;
class zpObjectContentManager;
class zpObjectResource;

enum zpObjectFlag : zp_uint
{
	ZP_OBJECT_FLAG_ENABLED,
	ZP_OBJECT_FLAG_CREATED,
	ZP_OBJECT_FLAG_SHOULD_DESTROY,
	ZP_OBJECT_FLAG_DONT_DESTROY_ON_UNLOAD,

	ZP_OBJECT_FLAG_USER0,

	zpObjectFlag_Count,

	zpObjectFlagUser_Count = 31,
};

class zpObjectResource : public zpResource< zpBison >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpObjectContentManager;
};

class zpObjectResourceInstance : public zpResourceInstance< zpObjectResource >
{};


class zpObject
{
	friend class zpObjectContentManager;

public:
	zpObject( zpApplication* application, const zpObjectResourceInstance& res );
	~zpObject();

	zpAllComponents* getComponents();

	void setFlag( zpObjectFlag flag );
	void unsetFlag( zpObjectFlag flag );
	zp_bool isFlagSet( zpObjectFlag flag ) const;

	const zpString& getName() const;
	void setName( const zpString& name );

	const zpMatrix4f& getTransform() const;
	void setTransform( const zpMatrix4f& transform );

	const zpString& getTags() const;
	void clearTags();

	zp_bool hasTag( const zp_char* tag ) const;
	void addTag( const zp_char* tag );
	void removeTag( const zp_char* tag );

	zp_bool hasTag( const zpString& tag ) const;
	void addTag( const zpString& tag );
	void removeTag( const zpString& tag );

	zpApplication* getApplication() const;

	void update();

private:
	void loadObject( zp_bool isInitialLoad );
	void unloadObject();

	zpMatrix4f m_transform;
	zpString m_name;
	zpString m_tags;
	zpFlag32 m_layers;
	zpFlag32 m_flags;
	zp_long m_lastLoadTime;
	zpAllComponents m_components;

	zpApplication* m_application;

	zpObjectResourceInstance m_object;
};

class zpObjectContentManager : public zpContentManager< zpObjectResource, zpObjectResourceInstance, zpObjectContentManager, 4 >, private zpContentPool< zpObject, 8 >
{
public:
	zpObject* createObject( zpApplication* application, const zp_char* filename );

	void destroyAllObjects( zp_bool isWorldSwap );
	void update();

private:
	zp_bool createResource( zpObjectResource* res, const zp_char* filename );
	void destroyResource( zpObjectResource* res );
	void initializeInstance( zpObjectResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif