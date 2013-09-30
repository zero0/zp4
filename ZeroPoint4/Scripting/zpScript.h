#ifndef ZP_SCRIPT_H
#define ZP_SCRIPT_H

class zpScriptResource;
class zpScriptResourceInstance;
class zpScriptContentManager;

class zpScriptResource : public zpResource< zp_handle >
{
public:
	zp_handle getMethod( const zp_char* methodName ) const;

private:
	zp_bool load( const zp_char* filename );
	void unload();

	mutable zpHashMap< zpString, zp_handle > m_cachedMethods;

	friend class zpScriptContentManager;
};

class zpScriptResourceInstance : public zpResourceInstance< zpScriptResource >
{
public:
	zpScriptResourceInstance();
	virtual ~zpScriptResourceInstance();

	void callMethod( const zp_char* methodName );

private:
	void initialized();
	void destroyed();

	zp_handle m_scriptObject;

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

class zpScriptContentManager : public zpContentManager< zpScriptResource, zpScriptResourceInstance, zpScriptContentManager, 128 >
{
public:
	zpScriptContentManager();
	virtual ~zpScriptContentManager();

private:
	zp_bool createResource( zpScriptResource* res, const zp_char* filename );
	void destroyResource( zpScriptResource* res );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
