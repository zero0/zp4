#ifndef ZP_CAMERA_COMPONENT_H
#define ZP_CAMERA_COMPONENT_H

class zpCameraComponent : public zpComponent
{
public:
	zpCameraComponent();
	~zpCameraComponent();

private:
	zpCamera m_camera;
};

#endif
