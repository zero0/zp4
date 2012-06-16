#pragma once
#ifndef ZP_SCRIPTING_RESOURCE_H
#define ZP_SCRIPTING_RESOURCE_H

class zpScriptingResource : public zpResource {
public:
	zpScriptingResource();
	virtual ~zpScriptingResource();

	zp_bool load();
	void unload();

private:
	void* m_script;
};

#endif