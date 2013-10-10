#pragma once
#ifndef ZP_BUFFER_DATA_H
#define ZP_BUFFER_DATA_H

struct zpWorldBufferData
{
	zpMatrix4f world;
};

struct zpCameraBufferData
{
	zpMatrix4f viewProjection;
	zpMatrix4f invViewProjection;

	zpVector4f up;
	zpVector4f lookTo;
	zpVector4f position;

	zp_float zNear;
	zp_float zFar;
	zp_float fovy;
	zp_float aspectRatio;
};

#endif