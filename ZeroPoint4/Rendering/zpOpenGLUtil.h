#pragma once
#ifndef ZP_OPENGL_UTIL_H
#define ZP_OPENGL_UTIL_H

ZP_FORCE_INLINE zp_uint __zpToGL( zpBufferType t ) {
	switch( t ) {
	case ZP_BUFFER_TYPE_VERTEX:	return GL_ARRAY_BUFFER;
	case ZP_BUFFER_TYPE_INDEX:	return GL_ELEMENT_ARRAY_BUFFER;
	default:					return GL_ARRAY_BUFFER;
	}
}

ZP_FORCE_INLINE zp_uint __zpToGL( zpBufferBindType t ) {
	switch( t ) {
	case ZP_BUFFER_BIND_DEFAULT:	return GL_STREAM_DRAW;
	case ZP_BUFFER_BIND_DYNAMIC:	return GL_DYNAMIC_DRAW;
	case ZP_BUFFER_BIND_IMMUTABLE:	return GL_STATIC_DRAW;
	default:						return GL_STREAM_DRAW;
	}
}

ZP_FORCE_INLINE zp_uint __zpToGL( zpMapType t ) {
	switch( t ) {
	case ZP_MAP_TYPE_READ:			return GL_READ_ONLY;
	case ZP_MAP_TYPE_WRITE:			return GL_WRITE_ONLY;
	case ZP_MAP_TYPE_READ_WRITE:	return GL_READ_WRITE;
	case ZP_MAP_TYPE_WRITE_DISCARD:	return GL_WRITE_ONLY;
	default:						return GL_WRITE_ONLY;
	}
}

ZP_FORCE_INLINE zp_uint __zpToGL( zpTopology t ) {
	switch( t ) {
	case ZP_TOPOLOGY_POINT_LIST: return GL_POINTS;

	case ZP_TOPOLOGY_LINE_LIST: return GL_LINES;
	case ZP_TOPOLOGY_LINE_STRIP: return GL_LINE_STRIP;

	case ZP_TOPOLOGY_TRIANGLE_LIST: return GL_TRIANGLES;
	case ZP_TOPOLOGY_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;

	default: return GL_TRIANGLES;
	}
}

#endif