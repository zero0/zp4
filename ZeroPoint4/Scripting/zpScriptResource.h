#pragma once
#ifndef ZP_SCRIPT_RESOURCE_H
#define ZP_SCRIPT_RESOURCE_H

class zpScriptResource : public zpResource {
public:
	zpScriptResource();
	virtual ~zpScriptResource();

	zp_bool load();
	void unload();

	void* getScriptObjectType() const;
	void* getMethod( const zpString& functionName );

private:
	void* m_scriptObjectType;

	zpHashMap<zpString, void*> m_cachedMethods;
};

#endif