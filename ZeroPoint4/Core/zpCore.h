#pragma once
#ifndef ZP_CORE_H
#define ZP_CORE_H

#if defined(DEBUG) || defined(_DEBUG)
#define ZP_DEBUG	1
#endif

#if ZP_DEBUG
#define ZP_ON_DEBUG( code )			do { code } while( 0 )
#define ZP_ON_DEBUG_MSG( msg, ... )	zp_printfln( msg, __VA_ARGS__ )
#define ZP_ASSERT( test, msg, ... )	do { if( !(test) ) { zp_printfcln( ZP_CC( CC_WHITE, CC_RED ), msg, __VA_ARGS__ ); } } while( 0 )
#else
#define ZP_ON_DEBUG( code )			(void)0
#define ZP_ON_DEBUG_MSG( msg, ... )	(void)0
#define ZP_ASSERT( test, msg, ... ) (void)0
#endif

#define ZP_USE_SIMD		0

#define ZP_INLINE		inline
#define ZP_FORCE_INLINE	__forceinline

#define ZP_SAFE_DELETE( p )			{ if( (p) ) { delete (p); (p) = ZP_NULL; } }
#define ZP_SAFE_DELETE_ARRAY( a )	{ if( (a) ) { delete[] (a); (a) = ZP_NULL; } }
#define ZP_SAFE_RELEASE( r )		{ if( (r) ) { (r)->Release(); (r) = ZP_NULL; } }

#include "zpBaseTypes.h"
#include "zpMath.h"

class zpConsole;
class zpTime;
class zpGameObject;
class zpGameObjectComponent;

#include "zpString.h"

#include "zpTime.h"
#include "zpConsole.h"

#include "zpIntrusiveList.h"
#include "zpFlag.h"

#include "zpGameObject.h"
#include "zpGameObjectComponent.h"

void zp_printf( const char* text, ... );
void zp_printfln( const char* text, ... );

#endif