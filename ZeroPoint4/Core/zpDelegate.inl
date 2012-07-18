
//DELEGATE_COMMA						,
//DELEGATE_TEMPLATE_PARAMS				typename P1, typename P2,...
//DELEGATE_CLASS_TEMPLATE_ARGS			P1, P2,...
//DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS	FP1, FP2,...
//DELEGATE_OPERATOR_ARGS				P1 p1, P2 p2,..
//DELEGATE_OPERATOR_PARAMS				p1, p2, 

#define APPEND_MACRO( x, y ) APPEND_DO( x, y )
#define APPEND_DO( x, y ) APPEND_DO2( x, y )
#define APPEND_DO2( x, y ) x ## y

#define zpDelegateFunctionFactoryClass		APPEND_MACRO( zpDelegateFunctionFactory, DELEGATE_ARG_COUNT )
#define zpDelegateMemberFactoryClass		APPEND_MACRO( zpDelegateMemberFactory, DELEGATE_ARG_COUNT )
#define zpDelegateConstMemberFactoryClass	APPEND_MACRO( zpDelegateConstMemberFactory, DELEGATE_ARG_COUNT )

template<typename R DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS> class zpDelegateFunctionFactoryClass;
template<typename R, typename T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS> class zpDelegateMemberFactoryClass;
template<typename R, typename T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS> class zpDelegateConstMemberFactoryClass;

template<typename R DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
class zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )> {
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

	ZP_FORCE_INLINE R operator()( DELEGATE_OPERATOR_ARGS ) {
		return (*m_function)( m_object DELEGATE_COMMA DELEGATE_OPERATOR_PARAMS );
	}

	operator zp_bool() const { return m_function == ZP_NULL ? false : m_object != ZP_NULL; }
	zp_bool operator!() const { return !( operator zp_bool() ); }

	zp_bool operator==( const zpDelegate& other ) { return m_function == ZP_NULL && m_object == ZP_NULL; }
	zp_bool operator!=( const zpDelegate& other ) { return !( operator==( other ) ); }

private:
	typedef R (*FuncType)( void* DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS );

	zpDelegate( FuncType func, void* obj ) : m_function( func ), m_object( obj ) {}

	FuncType m_function;
	void* m_object;

	template<typename FR DELEGATE_COMMA DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS>
	friend class zpDelegateFunctionFactoryClass;

	template<typename FR, typename FT DELEGATE_COMMA DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS>
	friend class zpDelegateMemberFactoryClass;

	template<typename FR, typename FT DELEGATE_COMMA DELEGATE_FRIEND_CLASS_TEMPLATE_ARGS>
	friend class zpDelegateConstMemberFactoryClass;
};

template<typename R DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
class zpDelegateFunctionFactoryClass {
private:
	template<R (*Func)( DELEGATE_CLASS_TEMPLATE_ARGS )>
	ZP_FORCE_INLINE static R Wrapper( void* DELEGATE_COMMA DELEGATE_OPERATOR_ARGS ) {
		return (*Func)( DELEGATE_OPERATOR_PARAMS );
	}

public:
	template<R (*Func)( DELEGATE_CLASS_TEMPLATE_ARGS )>
	ZP_FORCE_INLINE static zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )> Bind() {
		return zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )>( &zpDelegateFunctionFactoryClass::Wrapper<Func DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS>, ZP_NULL );
	}
};

template<typename R, typename T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
class zpDelegateMemberFactoryClass {
private:
	template<R (T::*Func)( DELEGATE_CLASS_TEMPLATE_ARGS )>
	ZP_FORCE_INLINE static R Wrapper( void* obj DELEGATE_COMMA DELEGATE_OPERATOR_ARGS ) {
		T* o = static_cast<T*>( obj );
		return (o->*Func)( DELEGATE_OPERATOR_PARAMS );
	}

public:
	template<R (T::*Func)( DELEGATE_CLASS_TEMPLATE_ARGS )>
	ZP_FORCE_INLINE static zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )> Bind( T* obj DELEGATE_COMMA DELEGATE_OPERATOR_ARGS ) {
		return zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )>( &zpDelegateMemberFactoryClass::Wrapper<Func DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS>, static_cast<void*>( obj ) );
	}
};

template<typename R, typename T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
class zpDelegateConstMemberFactoryClass {
private:
	template<R (T::*Func)( DELEGATE_CLASS_TEMPLATE_ARGS ) const>
	ZP_FORCE_INLINE static R Wrapper( void* obj DELEGATE_COMMA DELEGATE_OPERATOR_ARGS ) {
		const T* o = static_cast<const T*>( obj );
		return (o->*Func)( DELEGATE_OPERATOR_PARAMS );
	}

public:
	template<R (T::*Func)( DELEGATE_CLASS_TEMPLATE_ARGS ) const>
	ZP_FORCE_INLINE static zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )> Bind( T* obj ) {
		return zpDelegate<R ( DELEGATE_CLASS_TEMPLATE_ARGS )>( &zpDelegateConstMemberFactoryClass::Wrapper<Func DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS>, static_cast<void*>( obj ) );
	}
};


template<typename R DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
ZP_FORCE_INLINE zpDelegateFunctionFactoryClass<R DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS> zpDelegateGetFactory( R (*)( DELEGATE_CLASS_TEMPLATE_ARGS ) ) {
	return zpDelegateFunctionFactoryClass<R DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>();
};

template<typename R, typename T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
ZP_FORCE_INLINE zpDelegateMemberFactoryClass<R, T DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS> zpDelegateGetFactory( R (T::*)( DELEGATE_CLASS_TEMPLATE_ARGS ) ) {
	return zpDelegateMemberFactoryClass<R, T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>();
};

template<typename R, typename T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>
ZP_FORCE_INLINE zpDelegateConstMemberFactoryClass<R, T DELEGATE_COMMA DELEGATE_CLASS_TEMPLATE_ARGS> zpDelegateGetFactory( R (T::*)( DELEGATE_CLASS_TEMPLATE_ARGS ) const ) {
	return zpDelegateConstMemberFactoryClass<R, T DELEGATE_COMMA DELEGATE_TEMPLATE_PARAMS>();
};


#undef zpDelegateFunctionFactoryClass
#undef zpDelegateMemberFactoryClass
#undef zpDelegateFunctionFactoryClass

#undef APPEND_MACRO
#undef APPEND_DO
#undef APPEND_DO2
