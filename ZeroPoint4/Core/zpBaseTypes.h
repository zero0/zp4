#pragma once
#ifndef ZP_BASE_TYPES_H
#define ZP_BASE_TYPES_H

#if __cplusplus
#define ZP_NULL	nullptr
#else
#define ZP_NULL	0
#endif

typedef unsigned __int8 zp_ubyte;
typedef unsigned __int16 zp_ushort;
typedef unsigned __int32 zp_uint;
typedef unsigned __int64 zp_ulong;

typedef signed __int8 zp_byte;
typedef signed __int16 zp_short;
typedef signed __int32 zp_int;
typedef signed __int64 zp_long;

typedef float zp_float;
typedef double zp_double;

typedef bool zp_bool;

#if UNICODE
typedef wchar_t zp_char;
#else
typedef char zp_char;
#endif

#if ZP_USE_SIMD
#include "zpBaseTypesSimd.inl"
#else
#include "zpBaseTypesFpu.inl"
#endif

#endif