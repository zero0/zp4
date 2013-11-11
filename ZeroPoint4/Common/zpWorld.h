#pragma once
#ifndef ZP_WORLD_H
#define ZP_WORLD_H

class zpWorld;
class zpWorldContentManager;
class zpWorldResource;

enum zpWorldFlag : zp_uint
{
	ZP_WORLD_FLAG_CREATED,
	ZP_WORLD_FLAG_SHOULD_CREATE,
	ZP_WORLD_FLAG_SHOULD_DESTROY,
	ZP_WORLD_FLAG_STEP_CREATE,
	ZP_WORLD_FLAG_DESTROY_AFTER_LOAD,

	zpWorldFlag_Count,
};

class zpWorldResource : public zpResource< zpBison >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpWorldContentManager;
};

class zpWorldResourceInstance : public zpResourceInstance< zpWorldResource >
{};


class zpWorld
{
	friend class zpWorldContentManager;

public:
	zpWorld( zpApplication* application, const zpWorldResourceInstance& res );
	~zpWorld();

	zp_bool isFlagSet( zpWorldFlag flag ) const;
	void setFlag( zpWorldFlag flag );
	void unsetFlag( zpWorldFlag flag );

	zp_bool createStep();
	void destroy();

	const zpString& getName() const;
	zpApplication* getApplication() const;

	void additivlyLoad( const zpWorldResourceInstance& world );

private:
	zpString m_name;
	zpFlag32 m_flags;
	zpApplication* m_application;

	zp_uint m_numObjectsLoaded;
	zp_uint m_totalObjects;

	zpWorldResourceInstance m_world;
};

class zpWorldContentManager : public zpContentManager< zpWorldResource, zpWorldResourceInstance, zpWorldContentManager, 4 >, private zpContentPool< zpWorld, 4 >
{
public:
	zpWorld* createWorld( zpApplication* application, const zp_char* filename );

	void update();

private:
	zp_bool createResource( zpWorldResource* res, const zp_char* filename );
	void destroyResource( zpWorldResource* res );
	void initializeInstance( zpWorldResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
