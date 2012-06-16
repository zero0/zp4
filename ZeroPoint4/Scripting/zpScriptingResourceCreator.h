#pragma once
#ifndef ZP_SCRIPTING_RESOURCE_CREATOR_H
#define ZP_SCRIPTING_RESOURCE_CREATOR_H

class zpScriptingResourceCreator : public zpResourceCreator {
public:
	zpScriptingResourceCreator();
	virtual ~zpScriptingResourceCreator();

	zpResource* createResource( const zpString& filename );
};

#endif