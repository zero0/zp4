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

//#ifdef _WIN32
//#define ZP_USE_SIMD			0
//#elif defined(_WIN64)
#define ZP_USE_SIMD				0
//#endif

#define ZP_USE_ALIGNMENT		1
#if ZP_USE_ALIGNMENT
#define ZP_MALLOC_ALIGNMENT		16
#endif

#define ZP_USE_ASSERTIONS		1
#define ZP_USE_MEMORY_SYSTEM	1

#define ZP_LOG_ENABLED			1

#if ZP_DEBUG
#define ZP_ON_DEBUG( code )							do { code ; } while( 0 )
#define ZP_ON_DEBUG_MSG( msg, ... )					zp_printfln( msg, __VA_ARGS__ )
#else
#define ZP_ON_DEBUG( code )							(void)0
#define ZP_ON_DEBUG_MSG( msg, ... )					(void)0
#endif

#if ZP_USE_ASSERTIONS
#define ZP_ASSERT( test, msg, ... )					zp_assert( (test), __FILE__, __LINE__, msg, __VA_ARGS__ )
#else
#define ZP_ASSERT( test, msg, ... )					(void)0
#endif

#define ZP_STDCALL		__stdcall
#define ZP_FASTCALL		__fastcall

#define ZP_INLINE		inline
#define ZP_FORCE_INLINE	__forceinline
#define ZP_NO_VTABLE	__declspec( novtable )
#define ZP_ALIGN(x)		__declspec( align( x ) )
#define ZP_ALIGN16		ZP_ALIGN( 16 )

#define ZP_PURE_INTERFACE	class ZP_NO_VTABLE
#define ZP_ABSTRACT_CLASS	class ZP_NO_VTABLE

#define ZP_SAFE_DELETE( p )			{ if( (p) ) { delete (p); (p) = ZP_NULL; } }
#define ZP_SAFE_DELETE_ARRAY( a )	{ if( (a) ) { delete[] (a); (a) = ZP_NULL; } }
#define ZP_SAFE_RELEASE( r )		{ if( (r) ) { (r)->Release(); (r) = ZP_NULL; } }
#define ZP_SAFE_REMOVE_REF( r )		{ if( (r) ) { (r)->removeReference(); (r) = ZP_NULL; } }
#define ZP_SAFE_FREE( p )			{ if( (p) ) { zp_free( (p) ); (p) = ZP_NULL; } }

#define ZP_ARRAY_LENGTH( a )		( sizeof( (a) ) / sizeof( (a)[0] ) )

#define ZP_REGISTER_SERIALIZABLES( pack )	pack##RegisterSerializables()

#define ZP_USE_COLOR_CONSOLE	1
#define ZP_USE_FAST_MATH		0
#define ZP_USE_SAFE_FUNCTIONS	1

#include "zpBaseTypes.h"

#if ZP_USE_ASSERTIONS
void zp_assert( zp_bool test, const zp_char* file, zp_int line, const zp_char* msg, ... );
#endif

void zp_printf( const zp_char* text, ... );
void zp_printfln( const zp_char* text, ... );
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

template<zp_uint Size>
zp_char* zp_strcpy( zp_char (&destString)[Size], const zp_char* srcString ) { return zp_strcpy( destString, Size, srcString ); }
zp_char* zp_strcpy( zp_char* destString, zp_uint numElements, const zp_char* srcString );
zp_uint zp_strlen( const zp_char* srcString );
zp_int zp_strcmp( const zp_char* str1, const zp_char* str2 );
zp_char* zp_strstr( zp_char* str, const zp_char* subStr );
const zp_char* zp_strstr(  const zp_char* str, const zp_char* subStr );

zp_int zp_atoi( const zp_char* str );
zp_float zp_atof( const zp_char* str );

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
class zpRect;

class zpConsole;
class zpWindow;

class zpTime;

class zpMessage;
ZP_PURE_INTERFACE zpMessageReceiver;

template<typename T, zp_uint Count> class zpPooledObject;
class zpProperties;
class zpMemorySystem;
ZP_PURE_INTERFACE zpReferencedObject;

class zpGame;
class zpWorld;
ZP_ABSTRACT_CLASS zpGameManager;
class zpGameObject;
ZP_ABSTRACT_CLASS zpComponent;

#include "zpLog.h"
#include "zpMath.h"
#include "zpTween.h"

#include "zpDelegate.h"

#include "zpRenderable.h"
#include "zpSerializable.h"
#include "zpRegisterSerializable.h"

#include "zpString.h"
#include "zpStringBuffer.h"

#include "zpFlag.h"
#include "zpLinkedList.h"
#include "zpIntrusiveList.h"
#include "zpPair.h"
#include "zpArray.h"
#include "zpArrayList.h"
#include "zpHashMap.h"
#include "zpMap.h"
#include "zpFile.h"

#include "zpPooledObject.h"
#include "zpProperties.h"
#include "zpMemorySystem.h"
#include "zpReferencedObject.h"
#include "zpReferencedPointer.h"

#include "zpRandom.h"
#include "zpColor4f.h"
#include "zpRect.h"

#include "zpConsole.h"
#include "zpWindow.h"

#include "zpTime.h"

#include "zpMessage.h"
#include "zpMessageReceiver.h"

#include "zpAllComponents.h"
#include "zpAllGameManagers.h"

#include "zpGame.h"
#include "zpWorld.h"
#include "zpGameManager.h"
#include "zpGameObject.h"
#include "zpComponent.h"

template<typename T, typename A>
ZP_FORCE_INLINE T& zp_as( A& a )
{
	return (T)a;
}

template<typename T>
void zp_zero_memory( T* ptr )
{
	zp_memset( ptr, 0, sizeof( T ) );
}
template<typename T, zp_uint Size>
void zp_zero_memory_array( T (&arr)[Size] )
{
	zp_memset( arr, 0, Size * sizeof( T ) );
}

template<typename T>
ZP_FORCE_INLINE T&& zp_move( T& v )
{
	return (T&&)v;
}

template<typename T>
void zp_move_swap( T& a, T& b ) {
	T temp = (T&&)a;
	a = (T&&)b;
	b = (T&&)temp;
}

template<typename T>
void zp_qsort( T* arr, zp_uint l, zp_uint r ) {
	if( r <= l ) return;
	zp_uint i = l - 1, j = r;
	zp_uint p - l - 1, q = r;
	zp_uint k;

	while( true ) {
		while( arr[ ++i ] < a[ r ] );
		while( arr[ r ] < arr[ --j ] ) if( j == l ) break;
		if( i >= j ) break;

		zp_move_swap( arr[ i ], arr[ j ] );

		if( a[ i ] == a[ r ] ) zp_move_swap( a[ ++p ], a[ i ] );
		if( a[ j ] == a[ r ] ) zp_move_swap( a[ --q ], a[ j ] );
	}

	zp_move_swap( a[ i ], a[ r ] );
	j = i - 1;
	i = i + 1;
	
	for( k = l; k <= p; k++ ) zp_move_swap( a[ k ], a[ j-- ] );
	for( k = r - 1; k >= q; k-- ) zp_move_swap( a[ k ], a[ i++ ] );

	zp_qsort( a, l, j );
	zp_qsort( a, i, r );
}

template<typename T, zp_uint Size>
void zp_qsort( T (&arr)[Size] ) {
	zp_qsort( arr, 0, Size - 1 );
}

template<typename T>
void zp_qsort( T* arr, zp_uint count ) {
	zp_qsort( arr, 0, count - 1 );
}

void zpCoreRegisterSerializables();

#endif