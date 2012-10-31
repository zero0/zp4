#pragma once
#ifndef ZP_VERTEX_H
#define ZP_VERTEX_H

struct zpVertexPositionColor {
	zpVector4f position;
	zpColor4f color;
};

struct zpVertexPositionTexture {
	zpVector4f position;
	zpVector2f texCoord0;
};

struct zpVertexPositionTexture2 {
	zpVector4f position;
	zpVector2f texCoord0;
	zpVector2f texCoord1;
};

struct zpVertexPositionNormalTexture {
	zpVector4f position;
	zpVector4f normal;
	zpVector2f texCoord0;
};

#endif