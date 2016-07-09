#pragma once
#ifndef ZP_BUFFER_DATA_H
#define ZP_BUFFER_DATA_H

struct zpDrawCallBufferData
{
    zpMatrix4f world;
};

struct zpFrameBufferData
{
    zp_float deltaTime;
    zp_float actualDeltaTime;
    zp_float fixedDeltaTime;
    zp_float timeFromStart;
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
