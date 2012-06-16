#pragma once
#ifndef ZP_SCRIPTING_COMPONENT_H
#define ZP_SCRIPTING_COMPONENT_H

class zpScriptingComponent : public zpComponent {
public:
	zpScriptingComponent();
	virtual ~zpScriptingComponent();
	
	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpString m_scriptAlias;
	zpScriptingInstance* m_scriptInstance;
	void* m_scriptObject;
};

#endif