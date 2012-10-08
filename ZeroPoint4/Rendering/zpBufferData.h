#pragma once
#ifndef ZP_BUFFER_DATA_H
#define ZP_BUFFER_DATA_H

struct zpWorldBufferData {
	zpMatrix4f world;
};

struct zpCameraBufferData {
	zpMatrix4f viewProjection;
	zpMatrix4f invViewProjection;
};

#endif