#pragma once
#ifndef ZP_CAMERA_H
#define ZP_CAMERA_H

class zpCamera : public zpComponent, public zpRenderable {
public:
	zpCamera();
	~zpCamera();

	void render();

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

};

#endif