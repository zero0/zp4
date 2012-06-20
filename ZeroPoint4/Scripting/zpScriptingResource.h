#pragma once
#ifndef ZP_SCRIPTING_RESOURCE_H
#define ZP_SCRIPTING_RESOURCE_H

class zpScriptingResource : public zpResource {
public:
	zpScriptingResource();
	virtual ~zpScriptingResource();

	zp_bool load();
	void unload();

	void* getScriptObjectType() const;
	void* getMethod( const zpString& functionName );

private:
	void* m_scriptObjectType;

	zpHashMap<zpString, void*> m_cachedMethods;
};

#endif