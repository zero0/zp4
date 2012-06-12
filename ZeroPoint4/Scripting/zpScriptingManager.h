#pragma once
#ifndef ZP_SCRIPTING_MANAGER_H
#define ZP_SCRIPTING_MANAGER_H

#if ZP_DEBUG
#pragma comment( lib, "squirrel_d.lib" )
#pragma comment( lib, "sqstdlib_d.lib" )
#else
#pragma comment( lib, "squirrel.lib" )
#pragma comment( lib, "sqstdlib.lib" )
#endif

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
	zp_ptr m_vm;
};

#endif