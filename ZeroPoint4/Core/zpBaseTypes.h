#pragma once
#ifndef ZP_BASE_TYPES_H
#define ZP_BASE_TYPES_H

#if __cplusplus
#define ZP_NULL	nullptr
#else
#define ZP_NULL	0
#endif

typedef unsigned __int16 zp_ushort;
typedef unsigned __int32 zp_uint;
typedef unsigned __int64 zp_ulong;

typedef signed __int8 zp_sbyte;
typedef signed __int16 zp_short;
typedef signed __int32 zp_int;
typedef signed __int64 zp_long;

typedef float zp_float;
typedef double zp_double;

typedef bool zp_bool;
typedef char zp_char;

#if defined(_WIN32) 
typedef zp_uint zp_ptr;
typedef zp_ulong zp_lptr;
#elif defined(_WIN64)
typedef zp_ulong zp_ptr;
typedef zp_ulong zp_lptr;
#endif

typedef void* zp_handle;

typedef unsigned char zp_byte;
typedef zp_ushort zp_word;
typedef zp_uint zp_dword;
typedef zp_ulong zp_qword;

typedef zp_uint zp_hash;

#endif