#pragma once
#ifndef ZP_SCRIPTING_INSTANCE_H
#define ZP_SCRIPTING_INSTANCE_H

class zpScriptingInstance {
public:
	zpScriptingInstance( zpScriptingResource* resource );
	~zpScriptingInstance();

	void* getMethod( const zpString& methodName );
	void* getScriptObject() const;

private:
	zpScriptingInstance();
	
	void createObject();
	void destroyObject();

	zpScriptingResource* m_resource;

	void* m_scriptObject;
};

#endif