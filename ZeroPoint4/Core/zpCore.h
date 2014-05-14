#pragma once
#ifndef ZP_CORE_H
#define ZP_CORE_H

#if defined(DEBUG) || defined(_DEBUG)
#define ZP_DEBUG				1
#endif

#if _WIN32
#define ZP_WIN_32				1
#elif _WIN64
#define ZP_WIN_64				1
#endif

#define ZP_USE_COLOR_CONSOLE	0
#define ZP_USE_FAST_MATH		0
#define ZP_USE_SAFE_FUNCTIONS	1
#define ZP_USE_CONSOLE_PRINT	1
#define ZP_USE_DEBUG_PRINT		1
#define ZP_USE_PRINT			1
#define ZP_USE_PROFILER			1
#define ZP_USE_ALIGNMENT		0
#define ZP_USE_LOGGING			1
#define ZP_USE_ASSERTIONS		1
#define ZP_USE_MEMORY_SYSTEM	1
#define ZP_USE_HOT_RELOAD		1

//#ifdef _WIN32
//#define ZP_USE_SIMD			0
//#elif defined(_WIN64)
#define ZP_USE_SIMD				0
//#endif


#if ZP_USE_ALIGNMENT
#define ZP_MALLOC_ALIGNMENT		16
#endif

#define ZP_FORECE_32BIT			0x7FFFFFFF

#if ZP_DEBUG
#define ZP_ON_DEBUG( code )			do { code ; } while( 0 )
#else
#define ZP_ON_DEBUG( code )			(void)0
#endif

#if ZP_USE_ASSERTIONS
#define ZP_ASSERT( test, msg, ... )			if( !(test) ) { zp_assert( __FILE__, __LINE__, msg, __VA_ARGS__ ); }
#define ZP_ASSERT_WARN( test, msg, ... )	if( !(test) ) { zp_assert_warning( __FILE__, __LINE__, msg, __VA_ARGS__ ); }
#else
#define ZP_ASSERT( test, msg, ... )			(void)0
#define ZP_ASSERT_WARN( test, msg, ... )	(void)0
#endif

#define ZP_STDCALL					__stdcall
#define ZP_FASTCALL					__fastcall

#define ZP_INLINE					inline
#define ZP_FORCE_INLINE				__forceinline
#define ZP_NO_VTABLE				__declspec( novtable )
#define ZP_ALIGN(x)					__declspec( align( x ) )
#define ZP_ALIGN16					ZP_ALIGN( 16 )

#define ZP_PURE_INTERFACE			class ZP_NO_VTABLE
#define ZP_ABSTRACT_CLASS			class ZP_NO_VTABLE

#define ZP_UNUSED( v )				(void)v

#define ZP_SAFE_DELETE( p )			{ if( (p) ) { delete (p); (p) = ZP_NULL; } }
#define ZP_SAFE_DELETE_ARRAY( a )	{ if( (a) ) { delete[] (a); (a) = ZP_NULL; } }
#define ZP_SAFE_RELEASE( r )		{ if( (r) ) { (r)->Release(); (r) = ZP_NULL; } }
#define ZP_SAFE_REMOVE_REF( r )		{ if( (r) ) { (r)->removeReference(); (r) = ZP_NULL; } }
#define ZP_SAFE_FREE( p )			{ if( (p) ) { zp_free( (p) ); (p) = ZP_NULL; } }

#define ZP_ARRAY_LENGTH( a )		( sizeof( (a) ) / sizeof( (a)[0] ) )

#define ZP_NON_COPYABLE( t )		private: t( const t& ){} t& operator=( const t& ){}

#define ZP_MEMORY_KB( s )			(zp_int)( (s) * 1024 )
#define ZP_MEMORY_MB( s )			(zp_int)( ZP_MEMORY_KB(s) * 1024 )

#define ZP_MAKE_UINT( a, b, c, d )					(zp_uint)( ( (a) & 0xFF ) << 24 | ( (b) & 0xFF ) << 16 | ( (c) & 0xFF ) << 8 | ( (d) & 0xFF ) )
#define ZP_MAKE_ULONG( a, b, c, d, e, f, g, h )		(zp_ulong)( ZP_MAKE_UINT( a, b, c, d ) ) << 32 | (zp_ulong)( ZP_MAKE_UINT( e, f, g, h ) )
#define ZP_MAKE_CHAR4( a, b, c, d )					{ (a), (b), (c), (d) }
#define ZP_MAKE_FOURCC( a, b, c, d )				(zp_uint)( ( (a) & 0xFF ) << 0 | ( (b) & 0xFF ) << 8 | ( (c) & 0xFF ) << 16 | ( (d) & 0xFF ) << 24 )

#define ZP_REGISTER_SERIALIZABLES( pack )	pack##RegisterSerializables()

#include "zpBaseTypes.h"

#if ZP_USE_MEMORY_SYSTEM
void* operator new( zp_uint size );
void* operator new[]( zp_uint size );

void operator delete( void* ptr );
void operator delete[]( void* ptr );
#endif


#if ZP_USE_ASSERTIONS
void zp_assert( const zp_char* file, zp_int line, const zp_char* msg, ... );
void zp_assert_warning( const zp_char* file, zp_int line, const zp_char* msg, ... );
#endif

#if ZP_USE_PRINT
void zp_printf( const zp_char* text, ... );
void zp_printfln( const zp_char* text, ... );
#else
#define zp_printf( text, ... ) (void)0
#define zp_printfln( text, ... ) (void)0
#endif

void zp_snprintf( zp_char* dest, zp_uint destSize, zp_uint maxCount, const zp_char* format, ... );

void* zp_malloc( zp_uint size );
void* zp_calloc( zp_uint num, zp_uint size );
void* zp_realloc( void* ptr, zp_uint size );

void* zp_aligned_malloc( zp_uint size, zp_uint alignment );
void* zp_aligned_calloc( zp_uint size, zp_uint count, zp_uint alignment );
void* zp_aligned_realloc( void* ptr, zp_uint size, zp_uint alignment );
zp_uint zp_aligned_memsize( void* ptr, zp_uint alignment );

void zp_free( void* ptr );
void zp_aligned_free( void* ptr );

void* zp_memcpy( void* dest, zp_uint destSize, const void* src, zp_uint size );
void* zp_memmove( void* dest, zp_uint destSize, const void* src, zp_uint size );
void* zp_memset( void* dest, zp_int value, zp_uint size );
zp_int zp_memcmp( const void* ptr1, const void* ptr2, zp_uint size );

template<zp_uint Size>
zp_char* zp_strcpy( zp_char (&destString)[Size], const zp_char* srcString ) { return zp_strcpy( destString, Size, srcString ); }
zp_char* zp_strcpy( zp_char* destString, zp_uint numElements, const zp_char* srcString );
zp_char* zp_strncpy( zp_char* destString, zp_uint numElements, const zp_char* srcString, zp_uint maxCount );
zp_uint zp_strlen( const zp_char* srcString );
zp_int zp_strcmp( const zp_char* str1, const zp_char* str2 );
zp_char* zp_strstr( zp_char* str, const zp_char* subStr );
const zp_char* zp_strstr(  const zp_char* str, const zp_char* subStr );

zp_int zp_atoi( const zp_char* str );
zp_float zp_atof( const zp_char* str );
zp_float zp_atof( const zp_char* str, zp_char** end );
zp_long zp_atol( const zp_char* str );
zp_long zp_atol( const zp_char* str, zp_char** end, zp_int base );

zp_int zp_rand();
zp_float zp_randf();
void zp_srand( zp_uint seed );

zp_char zp_to_lower( zp_char ch );
zp_char zp_to_upper( zp_char ch );

zp_bool zp_is_ctrl( zp_char ch );
zp_bool zp_is_whitespace( zp_char ch );
zp_bool zp_is_upper( zp_char ch );
zp_bool zp_is_lower( zp_char ch );
zp_bool zp_is_alpha( zp_char ch );
zp_bool zp_is_digit( zp_char ch );
zp_bool zp_is_xdigit( zp_char ch );
zp_bool zp_is_alpha_numeric( zp_char ch );
zp_bool zp_is_punctuation( zp_char ch );
zp_bool zp_is_graphic( zp_char ch );
zp_bool zp_is_print( zp_char ch );

zp_uint zp_near_pow2( zp_uint number );

zp_int zp_round_up( zp_int number, zp_int multiple );
zp_int zp_round_up_pow2( zp_int number, zp_int multiple );

void zp_sleep( zp_uint milliseconds );

template<typename T>
ZP_FORCE_INLINE zp_hash zp_fnv1_32( const T& d, zp_hash h = 0 )
{
	return zp_fnv1_32_data( (const void*)&d, sizeof( T ), h );
}
zp_hash zp_fnv1_32_data( const void* d, zp_uint l, zp_hash h );
zp_hash zp_fnv1_32_string( const zp_char* c, zp_hash h );

template<typename T>
ZP_FORCE_INLINE T&& zp_move( T& v )
{
	return (T&&)v;
}

template<typename T>
ZP_FORCE_INLINE void zp_swap( T& a, T& b )
{
	T temp( a );
	a = b;
	b = temp;
}
template<typename T>
ZP_FORCE_INLINE void zp_move_swap( T& a, T& b )
{
	T temp( zp_move( a ) );
	a = zp_move( b );
	b = zp_move( temp );
}

class zpStackTrace;

class zpLog;
class zpLogOutput;

template<typename T> class zpTween;

ZP_PURE_INTERFACE zpRenderable;
ZP_PURE_INTERFACE zpSerializable;
ZP_PURE_INTERFACE zpSerializedOutput;
ZP_PURE_INTERFACE zpSerializedInput;
class zpRegisterSerializable;

class zpString;
class zpStringBuffer;
class zpDataBuffer;
template<typename T> class zpFlag;
template<typename T> class zpLinkedList;
template<typename T> class zpIntrusiveList;
template<typename F, typename S> class zpPair;
template<typename T, zp_uint Size> class zpArray;
template<typename T> class zpArrayList;
template<typename K, typename V> class zpMap;
template<typename F, typename S> class zpHashMap;
class zpFile;

class zpRandom;
class zpColor4f;

class zpConsole;
class zpWindow;

class zpTime;
class zpProfiler;

class zpMessage;
ZP_PURE_INTERFACE zpMessageReceiver;

template<typename T, zp_uint Count> class zpPooledObject;
class zpProperties;
class zpMemorySystem;
ZP_PURE_INTERFACE zpReferencedObject;
class zpJson;
class zpXmlParser;

#include "zpStackTrace.h"

#include "zpMath.h"
#include "zpTween.h"

#include "zpDelegate.h"

#include "zpRenderable.h"
#include "zpSerializable.h"
#include "zpRegisterSerializable.h"

#include "zpString.h"
#include "zpStringBuffer.h"
#include "zpDataBuffer.h"

#include "zpFlag.h"
#include "zpLinkedList.h"
#include "zpIntrusiveList.h"
#include "zpPair.h"
#include "zpArray.h"
#include "zpArrayList.h"
#include "zpHashMap.h"
#include "zpMap.h"
#include "zpFile.h"

#include "zpProperties.h"
#include "zpMemorySystem.h"
#include "zpReferencedObject.h"
#include "zpReferencedPointer.h"
#include "zpJson.h"
#include "zpXML.h"
#include "zpBison.h"

#include "zpRandom.h"
#include "zpColor4f.h"
#include "zpRect.h"

#include "zpConsole.h"
#include "zpWindow.h"
#include "zpLog.h"

#include "zpTime.h"
#include "zpProfiler.h"

#include "zpMessage.h"
#include "zpMessageReceiver.h"



template<typename T>
void zp_zero_memory( T* ptr )
{
	zp_memset( ptr, 0, sizeof( T ) );
}
template<typename T>
void zp_zero_memory_array( T* ptr, zp_uint count )
{
	zp_memset( ptr, 0, count * sizeof( T ) );
}
template<typename T, zp_uint Size>
void zp_zero_memory_array( T (&arr)[Size] )
{
	zp_memset( arr, 0, Size * sizeof( T ) );
}

template<typename T, typename LessFunc>
void zp_qsort( T* arr, zp_int l, zp_int r, LessFunc cmp )
{
	zp_int i = l, j = r;
	zp_int p = ( l + r ) / 2;

	while( i <= j )
	{
		while( cmp( arr[ i ], arr[ p ] ) )
		{
			i++;
		}
		while( cmp( arr[ p ], arr[ j ] ) )
		{
			j--;
		}
		if( i <= j )
		{
			zp_move_swap( arr[ i ], arr[ j ] );
			i++;
			j--;
		}
	}

	if( l < j )
	{
		zp_qsort( arr, l, j, cmp );
	}
	if( i < r )
	{
		zp_qsort( arr, i, r, cmp );
	}

#if 0
	if( r <= l ) return;
	zp_int i = l - 1, j = r;
	zp_int p = l - 1, q = r;
	zp_int k;

	while( true ) {
		while( cmp( arr[ ++i ], arr[ r ] ) )
		{}
		while( cmp( arr[ r ], arr[ --j ] ) )
		{
			if( j == l ) break;
		}
		if( i >= j ) break;

		zp_move_swap( arr[ i ], arr[ j ] );

		if( arr[ i ] == arr[ r ] ) zp_move_swap( arr[ ++p ], arr[ i ] );
		if( arr[ j ] == arr[ r ] ) zp_move_swap( arr[ --q ], arr[ j ] );
	}

	zp_move_swap( arr[ i ], arr[ r ] );
	j = i - 1;
	i = i + 1;
	
	for( k = l; k <= p; k++ )
	{
		zp_move_swap( arr[ k ], arr[ j-- ] );
	}
	for( k = r - 1; k >= q; k-- )
	{
		zp_move_swap( arr[ k ], arr[ i++ ] );
	}

	zp_qsort( arr, l, j, cmp );
	zp_qsort( arr, i, r, cmp );
#endif
}

template<typename T, zp_uint Size, typename LessFunc>
void zp_qsort( T (&arr)[Size], LessFunc cmp )
{
	zp_qsort( arr, 0, Size - 1, cmp );
}

template<typename T, typename LessFunc>
void zp_qsort( T* arr, zp_uint count, LessFunc cmp )
{
	zp_qsort( arr, 0, count - 1, cmp );
}

zp_bool zp_base64_encode( const void* data, zp_uint length, zpStringBuffer& outEncode );
zp_bool zp_base64_decode( const zp_char* data, zp_uint length, zpDataBuffer& outDecode );

void zpCoreRegisterSerializables();

#endif