#ifndef ZP_SCRIPT_COMPONENT_H
#define ZP_SCRIPT_COMPONENT_H

class zpScriptComponent : public zpComponent
{
public:
	zpScriptComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpScriptComponent();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zpArrayList< zpScriptResourceInstance > m_scripts;
};

class zpScriptComponentPool : public zpContentPool< zpScriptComponent, 128 >
{
public:
	zpScriptComponentPool();
	virtual ~zpScriptComponentPool();

	void update();
	void simulate();
};

#endif
