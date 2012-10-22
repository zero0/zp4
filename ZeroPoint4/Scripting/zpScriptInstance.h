#pragma once
#ifndef ZP_SCRIPT_INSTANCE_H
#define ZP_SCRIPT_INSTANCE_H

ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpScriptResource )
public:
	void* getMethod( const zpString& methodName );
	void* getScriptObject() const;

private:
	void createObject();
	void destroyObject();

	void* m_scriptObject;
ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif