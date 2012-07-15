#pragma once
#ifndef ZP_DELEGATE_H
#define ZP_DELEGATE_H

#if 0
template<typename R> class zpDelegate;

template<typename R> class zpDelegateFunctionFactory;
template<typename R, typename T> class zpDelegateMemberFactory;
template<typename R, typename T> class zpDelegateConstMemberFactory;

template<typename R>
class zpDelegate<R ()> {
public:
	zpDelegate() : m_function( ZP_NULL ), m_object( ZP_NULL ) {}
	zpDelegate( const zpDelegate& other ) : m_function( other.m_function ), m_object( other.m_object ) {}
	zpDelegate( zpDelegate&& other ) : m_function( other.m_function ), m_object( other.m_object ) {
		other.m_function = ZP_NULL;
		other.m_object = ZP_NULL;
	}
	~zpDelegate() {}

	void operator=( const zpDelegate& other ) {
		m_function = other.m_function;
		m_object = other.m_object;
	}
	void operator=( zpDelegate&& other ) {
		m_function = other.m_function;
		m_object = other.m_object;

		other.m_function = ZP_NULL;
		other.m_object = ZP_NULL;
	}

	ZP_FORCE_INLINE R operator()() {
		return (*m_function)( m_object );
	}

	operator zp_bool() const { return m_function == ZP_NULL ? false : m_object != ZP_NULL; }
	zp_bool operator!() const { return !( operator zp_bool() ); }

private:
	typedef R (*FuncType)( void* obj );

	zpDelegate( FuncType func, void* obj ) : m_function( func ), m_object( obj ) {}

	FuncType m_function;
	void* m_object;

	template<typename FR>
	friend class zpDelegateFunctionFactory;

	template<typename FR, typename FT>
	friend class zpDelegateMemberFactory;

	template<typename FR, typename FT>
	friend class zpDelegateConstMemberFactory;
};

template<typename R>
class zpDelegateFunctionFactory {
private:
	template<R (*Func)()>
	ZP_FORCE_INLINE static R Wrapper( void* ) {
		return (*Func)();
	}

public:
	template<R (*Func)()>
	ZP_FORCE_INLINE static zpDelegate<R ()> Bind() {
		return zpDelegate<R ()>( &zpDelegateFunctionFactory::Wrapper<Func>, ZP_NULL );
	}
};

template<typename R, typename T>
class zpDelegateMemberFactory {
private:
	template<R (T::*Func)()>
	ZP_FORCE_INLINE static R Wrapper( void* obj ) {
		T* o = static_cast<T*>( obj );
		return (o->*Func)();
	}

public:
	template<R (T::*Func)()>
	ZP_FORCE_INLINE static zpDelegate<R ()> Bind( T* obj ) {
		return zpDelegate<R ()>( &zpDelegateMemberFactory::Wrapper<Func>, static_cast<void*>( obj ) );
	}
};

template<typename R, typename T>
class zpDelegateConstMemberFactory {
private:
	template<R (T::*Func)() const>
	ZP_FORCE_INLINE static R Wrapper( void* obj ) {
		const T* o = static_cast<const T*>( obj );
		return (o->*Func)();
	}

public:
	template<R (T::*Func)() const>
	ZP_FORCE_INLINE static zpDelegate<R ()> Bind( T* obj ) {
		return zpDelegate<R ()>( &zpDelegateConstMemberFactory::Wrapper<Func>, static_cast<void*>( obj ) );
	}
};

template<typename R>
ZP_FORCE_INLINE zpDelegateFunctionFactory<R> zpDelegateGetFactory( R (*)() ) {
	return zpDelegateFunctionFactory<R>();
}

template<typename R, typename T>
ZP_FORCE_INLINE zpDelegateMemberFactory<R, T> zpDelegateGetFactory( R (T::*)() ) {
	return zpDelegateMemberFactory<R, T>();
}

template<typename R, typename T>
ZP_FORCE_INLINE zpDelegateConstMemberFactory<R, T> zpDelegateGetFactory( R (T::*)() const ) {
	return zpDelegateConstMemberFactory<R, T>();
}
#endif

template<typename R> class zpDelegate;

// 0 parameters

#define DELEGATE_ARG_COUNT					0
#define DELEGATE_COMMA
#define DELEGATE_TEMPLATE_PARAMS
#define DELEGATE_CLASS_TEMPLATE_ARGS
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#define DELEGATE_OPERATOR_ARGS
#define DELEGATE_OPERATOR_PARAMS
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS


// 1 parameter
#define DELEGATE_ARG_COUNT					1
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1
#define DELEGATE_OPERATOR_ARGS				P1 a1
#define DELEGATE_OPERATOR_PARAMS			a1
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS

// 2 parameters
#define DELEGATE_ARG_COUNT					2
#define DELEGATE_COMMA						,
#define DELEGATE_TEMPLATE_PARAMS			typename P1, typename P2
#define DELEGATE_CLASS_TEMPLATE_ARGS		P1, P2
#define DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	typename FP1, typename FP2
#define DELEGATE_OPERATOR_ARGS				P1 a1, P2 a2
#define DELEGATE_OPERATOR_PARAMS			a1, a2
#include "zpDelegate.inl"
#undef DELEGATE_ARG_COUNT
#undef DELEGATE_COMMA
#undef DELEGATE_TEMPLATE_PARAMS
#undef DELEGATE_CLASS_TEMPLATE_ARGS
#undef DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS
#undef DELEGATE_OPERATOR_ARGS
#undef DELEGATE_OPERATOR_PARAMS


#define zpCreateFunctionDelegate( func )			( zpDelegateGetFactory( func ).Bind<func>() )
#define zpCreateMemberDelegate( memFunc, objPtr	)	( zpDelegateGetFactory( memFunc ).Bind<memFunc>( objPtr ) )

#endif