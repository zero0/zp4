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

template<typename T>
T zp_limit_min();
template<typename T>
T zp_limit_max();

template<>
ZP_FORCE_INLINE zp_byte zp_limit_max<zp_byte>() { return 0xFF; }
template<>
ZP_FORCE_INLINE zp_byte zp_limit_min<zp_byte>() { return 0; }

template<>
ZP_FORCE_INLINE zp_sbyte zp_limit_max<zp_sbyte>() { return 127; }
template<>
ZP_FORCE_INLINE zp_sbyte zp_limit_min<zp_sbyte>() { return (-zp_limit_max<zp_sbyte>() - 1); }

template<>
ZP_FORCE_INLINE zp_char zp_limit_max<zp_char>() { return 127; }
template<>
ZP_FORCE_INLINE zp_char zp_limit_min<zp_char>() { return (-zp_limit_max<zp_char>() - 1); }

template<>
ZP_FORCE_INLINE zp_short zp_limit_max<zp_short>() { return 32767; }
template<>
ZP_FORCE_INLINE zp_short zp_limit_min<zp_short>() { return (-zp_limit_max<zp_short>() - 1); }

template<>
ZP_FORCE_INLINE zp_ushort zp_limit_max<zp_ushort>() { return 0xFFFF; }
template<>
ZP_FORCE_INLINE zp_ushort zp_limit_min<zp_ushort>() { return 0; }

template<>
ZP_FORCE_INLINE zp_int zp_limit_max<zp_int>() { return 2147483647; }
template<>
ZP_FORCE_INLINE zp_int zp_limit_min<zp_int>() { return (-zp_limit_max<zp_int>() - 1); }

template<>
ZP_FORCE_INLINE zp_uint zp_limit_max<zp_uint>() { return 0xFFFFFFFF; }
template<>
ZP_FORCE_INLINE zp_uint zp_limit_min<zp_uint>() { return 0; }

template<>
ZP_FORCE_INLINE zp_long zp_limit_max<zp_long>() { return 9223372036854775807L; }
template<>
ZP_FORCE_INLINE zp_long zp_limit_min<zp_long>() { return (-zp_limit_max<zp_long>() - 1); }

template<>
ZP_FORCE_INLINE zp_ulong zp_limit_max<zp_ulong>() { return 0xFFFFFFFFFFFFFFFFL; }
template<>
ZP_FORCE_INLINE zp_ulong zp_limit_min<zp_ulong>() { return 0; }


#endif