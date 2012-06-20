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

	void callFunction( const zpString& functionName );

	void setScriptAlias( const zpString& alias );
	const zpString& getScriptAlias() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpScriptingInstance* m_scriptInstance;
	
	zpString m_scriptAlias;
};

#endif