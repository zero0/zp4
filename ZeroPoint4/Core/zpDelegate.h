#pragma once
#ifndef ZP_DELEGATE_H
#define ZP_DELEGATE_H

template<typename T> class zpDelegate;
template<typename T> class zpDelegateEvent;

// 0 parameters
#define DELEGATE_ARG_COUNT					0
#define DELEGATE_COMMA
#define DELEGATE_TEMPLATE_PARAMS
#define DELEGATE_CLASS_TEMPLATE_ARGS
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#define DELEGATE_OPERATOR_ARGS
#define DELEGATE_OPERATOR_PARAMS
#define DELEGATE_OPERATOR_REF_PARAMS
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

// 1 parameter
#define DELEGATE_ARG_COUNT					1
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1
#define DELEGATE_OPERATOR_ARGS				P1 a1
#define DELEGATE_OPERATOR_PARAMS			a1
#define DELEGATE_OPERATOR_REF_PARAMS		&a1
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

// 2 parameters
#define DELEGATE_ARG_COUNT					2
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1, typename P2
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1, P2
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1, typename FP2
#define DELEGATE_OPERATOR_ARGS				P1 a1, P2 a2
#define DELEGATE_OPERATOR_PARAMS			a1, a2
#define DELEGATE_OPERATOR_REF_PARAMS		&a1, &a2
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

// 3 parameters
#define DELEGATE_ARG_COUNT					3
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1, typename P2, typename P3
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1, P2, P3
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1, typename FP2, typename FP3
#define DELEGATE_OPERATOR_ARGS				P1 a1, P2 a2, P3 a3
#define DELEGATE_OPERATOR_PARAMS			a1, a2, a3
#define DELEGATE_OPERATOR_REF_PARAMS		&a1, &a2, &a3
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

// 4 parameters
#define DELEGATE_ARG_COUNT					4
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1, typename P2, typename P3, typename P4
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1, P2, P3, P4
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1, typename FP2, typename FP3, typename FP4
#define DELEGATE_OPERATOR_ARGS				P1 a1, P2 a2, P3 a3, P4 a4
#define DELEGATE_OPERATOR_PARAMS			a1, a2, a3, a4
#define DELEGATE_OPERATOR_REF_PARAMS		&a1, &a2, &a3, &a4
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

// 5 parameters
#define DELEGATE_ARG_COUNT					5
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1, typename P2, typename P3, typename P4, typename P5
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1, P2, P3, P4, P5
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1, typename FP2, typename FP3, typename FP4, typename FP5
#define DELEGATE_OPERATOR_ARGS				P1 a1, P2 a2, P3 a3, P4 a4, P5 a5
#define DELEGATE_OPERATOR_PARAMS			a1, a2, a3, a4, a5
#define DELEGATE_OPERATOR_REF_PARAMS		&a1, &a2, &a3, &a4, &a5
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

// 6 parameters
#define DELEGATE_ARG_COUNT					6
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1, typename P2, typename P3, typename P4, typename P5, typename P6
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1, P2, P3, P4, P5, P6
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6
#define DELEGATE_OPERATOR_ARGS				P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6
#define DELEGATE_OPERATOR_PARAMS			a1, a2, a3, a4, a5, a6
#define DELEGATE_OPERATOR_REF_PARAMS		&a1, &a2, &a3, &a4, &a5, &a6
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS
#undef DELEGATE_OPERATOR_REF_PARAMS

#define zpCreateFunctionDelegate( func )			( zpDelegateGetFactory( func ).Bind<func>() )
#define zpCreateMemberDelegate( memFunc, objPtr	)	( zpDelegateGetFactory( memFunc ).Bind<memFunc>( objPtr ) )

#endif