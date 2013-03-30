#pragma once
#ifndef ZP_VERTEX_H
#define ZP_VERTEX_H

struct zpVertexPositionColor
{
	zpVector4f position;
	zpColor4f color;
};

struct zpVertexPositionUV
{
	zpVector4f position;
	zpVector2f uv0;
};

struct zpVertexPositionUV2
{
	zpVector4f position;
	zpVector2f uv0;
	zpVector2f uv1;
};

struct zpVertexPositionNormalUV
{
	zpVector4f position;
	zpVector4f normal;
	zpVector2f uv0;
};

#endif