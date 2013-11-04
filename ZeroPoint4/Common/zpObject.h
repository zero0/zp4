#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

class zpObject;
class zpObjectContentManager;
class zpObjectResource;

enum zpObjectFlag : zp_ulong
{
	ZP_OBJECT_FLAG_ENABLED =		( 1 << 0 ),
	ZP_OBJECT_FLAG_CREATED =		( 1 << 1 ),
	ZP_OBJECT_FLAG_SHOULD_DESTROY =	( 1 << 2 ),

	ZP_OBJECT_FLAG_USER0 =			( 1 << 3 ),

	zpObjectFlag_Count,
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

	zpWorld* getWorld() const;
	void setWorld( zpWorld* world );

	zpApplication* getApplication() const;
	void setApplication( zpApplication* application );

	void update();

private:
	void loadObject();
	void unloadObject();

	zpMatrix4f m_transform;
	zpString m_name;
	zpFlag64 m_flags;
	zp_long m_lastLoadTime;
	zpAllComponents m_components;

	zpWorld* m_world;
	zpApplication* m_application;

	zpObjectResourceInstance m_object;
};

class zpObjectContentManager : public zpContentManager< zpObjectResource, zpObjectResourceInstance, zpObjectContentManager, 128 >, private zpContentPool< zpObject, 256 >
{
public:
	zpObject* createObject( zpApplication* application, const zp_char* filename );

	void update();

private:
	zp_bool createResource( zpObjectResource* res, const zp_char* filename );
	void destroyResource( zpObjectResource* res );
	void initializeInstance( zpObjectResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};




#endif