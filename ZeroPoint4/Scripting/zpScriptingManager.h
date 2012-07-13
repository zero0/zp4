#pragma once
#ifndef ZP_SCRIPTING_MANAGER_H
#define ZP_SCRIPTING_MANAGER_H

struct asSMessageInfo;
class asIScriptContext;

class zpScriptingManager : public zpGameManager {
public:
	zpScriptingManager();
	virtual ~zpScriptingManager();
	
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
	void messageCallback( const asSMessageInfo& msg );
	
	zp_uint m_numContexts;
	zpArrayList<asIScriptContext*> m_contexts;
};

#endif