#ifndef ZP_CAMERA_COMPONENT_H
#define ZP_CAMERA_COMPONENT_H

class zpCameraComponent : public zpComponent
{
public:
	zpCameraComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpCameraComponent();

	zpCamera* getCamera() { return m_camera; }

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zpCamera* m_camera;
};


class zpCameraComponentPool : public zpContentPool< zpCameraComponent, 8 >
{
public:
	zpCameraComponentPool();
	virtual ~zpCameraComponentPool();

	void update();
	void simulate();
};

#endif
