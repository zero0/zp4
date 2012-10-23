#pragma once
#ifndef ZP_VERTEX_H
#define ZP_VERTEX_H

struct zpVertexPositionColor {
	zpVector4f position;
	zpColor4f color;
};

struct zpVertexPositionlTexture {
	zpVector4f position;
	zpVector2f texCoord0;
};

struct zpVertexPositionNormalTexture {
	zpVector4f position;
	zpVector4f normal;
	zpVector2f texCoord0;
};

#endif