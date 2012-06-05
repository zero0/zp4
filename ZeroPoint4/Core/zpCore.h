#pragma once
#ifndef ZP_CORE_H
#define ZP_CORE_H

#if defined(DEBUG) || defined(_DEBUG)
#define ZP_DEBUG	1
#endif

#if _WIN32
#define ZP_WIN_32
#elif _WIN64
#define ZP_WIN_64
#endif

//#ifdef _WIN32
//#define ZP_USE_SIMD		0
//#elif defined(_WIN64)
#define ZP_USE_SIMD			0
//#endif

#if ZP_DEBUG
#define ZP_ON_DEBUG( code )							do { code ; } while( 0 )
#define ZP_ON_DEBUG_MSG( msg, ... )					zp_printfln( msg, __VA_ARGS__ )
#define ZP_ASSERT( test, msg, ... )					do { if( !( test ) ) { zp_printfln( msg, __VA_ARGS__ ); } } while( 0 )
#define ZP_ASSERT_RETURN( test, msg, ... )			do { if( !( test ) ) { zp_printfln( msg, __VA_ARGS__ ); return; } } while( 0 ) 
#define ZP_ASSERT_RETURN_( test, ret, msg, ... )	do { if( !( test ) ) { zp_printfln( msg, __VA_ARGS__ ); return (ret); } } while( 0 ) 
#else
#define ZP_ON_DEBUG( code )							(void)0
#define ZP_ON_DEBUG_MSG( msg, ... )					(void)0
#define ZP_ASSERT( test, msg, ... )					(void)0
#define ZP_ASSERT_RETURN( test, msg, ... )			(void)0
#define ZP_ASSERT_RETURN_( test, ret, msg, ... )	(void)0
#endif

#define ZP_STDCALL		__stdcall
#define ZP_FASTCALL		__fastcall

#define ZP_INLINE		inline
#define ZP_FORCE_INLINE	__forceinline
#define ZP_NO_VTABLE	__declspec( novtable )
#define ZP_ALIGN(x)		__declspec( align( x ) )
#define ZP_ALIGN16		ZP_ALIGN( 16 )

#define ZP_PURE_INTERFACE	struct ZP_NO_VTABLE
#define ZP_ABSTRACT_CLASS	class ZP_NO_VTABLE

#define ZP_SAFE_DELETE( p )			{ if( (p) ) { delete (p); (p) = ZP_NULL; } }
#define ZP_SAFE_DELETE_ARRAY( a )	{ if( (a) ) { delete[] (a); (a) = ZP_NULL; } }
#define ZP_SAFE_RELEASE( r )		{ if( (r) ) { (r)->Release(); (r) = ZP_NULL; } }
#define ZP_SAFE_REMOVE_REF( r )		{ if( (r) ) { (r)->removeReference(); (r) = ZP_NULL; } }
#define ZP_SAFE_FREE( p )			{ if( (p) ) { free( (p) ); (p) = ZP_NULL; } }

#include "zpBaseTypes.h"
#include "zpMath.h"

class zpString;
template<typename T> class zpFlag;
template<typename T> class zpLinkedList;
template<typename T> class zpIntrusiveList;
template<typename F, typename S> class zpPair;
template<typename T> class zpArray;
template<typename K, typename V> class zpMap;
template<typename F, typename S> class zpHashMap;

class zpConsole;
class zpWindow;

class zpTime;

class zpMessage;
ZP_PURE_INTERFACE zpMessageReceiver;

class zpMemorySystem;
class zpReferencedObject;

class zpGame;
class zpWorld;
class zpGameManager;
class zpGameObject;
class zpGameObjectComponent;

#include "zpString.h"

#include "zpFlag.h"
#include "zpLinkedList.h"
#include "zpIntrusiveList.h"
#include "zpPair.h"
#include "zpArray.h"
#include "zpHashMap.h"
#include "zpMap.h"

#include "zpMemorySystem.h"
#include "zpReferencedObject.h"
#include "zpReferencedPointer.h"

#include "zpConsole.h"
#include "zpWindow.h"

#include "zpTime.h"

#include "zpMessage.h"
#include "zpMessageReceiver.h"

#include "zpGame.h"
#include "zpWorld.h"
#include "zpGameManager.h"
#include "zpGameObject.h"
#include "zpGameObjectComponent.h"

void zp_printf( const char* text, ... );
void zp_printfln( const char* text, ... );

#endif