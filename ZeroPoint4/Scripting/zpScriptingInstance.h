#pragma once
#ifndef ZP_SCRIPTING_INSTANCE_H
#define ZP_SCRIPTING_INSTANCE_H

class zpScriptingInstance {
public:
	zpScriptingInstance( zpScriptingResource* resource );
	~zpScriptingInstance();

	void callFunction( const zpString& functionName );

private:
	zpScriptingInstance();
	
	zpScriptingResource* m_resource;

	void* m_object;
};

#endif