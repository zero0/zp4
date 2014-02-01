#pragma once
#ifndef ZP_CAMERA_STATE_H
#define ZP_CAMERA_STATE_H

ZP_ABSTRACT_CLASS zpCameraState
{
public:
	virtual ~zpCameraState() {}

	virtual void onEnter( zpCamera* camera ) {};
	virtual void onLeave( zpCamera* camera ) {};

	virtual zp_bool onUpdate( zpApplication* app, zpCamera* camera ) = 0;
};

#endif
