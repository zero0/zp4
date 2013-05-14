#pragma once
#ifndef ZP_RESOURCE_H
#define ZP_RESOURCE_H

ZP_ABSTRACT_CLASS zpResource
{
public:
	zpResource();
	virtual ~zpResource();

	virtual zp_bool load() = 0;
	virtual void unload() = 0;

	zpContentManager* getContentManager() const;
	const zpString& getFilename() const;
	zp_bool isLoaded() const;

private:
	void setIsLoaded( zp_bool loaded );
	void setContentManager( zpContentManager* manager );
	void setFilename( const zpString& filename );

	zp_bool m_isLoaded;
	zpContentManager* m_manager;
	zpString m_filename;
	
	friend class zpContentManager;
};

/*
template<typename R>
class zpResourceInstance {
public:
	zpResourceInstance()
		: m_resource( ZP_NULL )
	{}
	zpResourceInstance( R* resource )
		: m_resource( resource )
	{}
	zpResourceInstance( const zpResourceInstance<R>& instance )
		: m_resource( instance.m_resource )
	{}
	zpResourceInstance( zpResourceInstance<R>&& instance )
		: m_resource( instance.m_resource )
	{
		instance.m_resource = ZP_NULL;
	}
	~zpResourceInstance() {
		if( m_resource ) {
			m_resource->getContentManager()->destroyInstanceOfResource( *this );
		}
		m_resource = ZP_NULL;
	}

	void operator=( const zpResourceInstance<R>& instance )	{
		m_resource = instance.m_resource;
	}
	void operator=( zpResourceInstance<R>&& instance )	{
		m_resource = instance.m_resource;
		instance.m_resource = ZP_NULL;
	}

	operator zp_bool() const {
		return m_resource && m_resource->isLoaded();
	}

	R* getResource() const {
		return m_resource;
	}

private:
	R* m_resource;

	friend class zpContentManager;
};
*/

#define ZP_RESOURCE_INSTANCE_TEMPLATE_START( T )	\
template<>	\
class zpResourceInstance<T> {	\
public:	\
	zpResourceInstance() : m_resource( ZP_NULL ) {}\
	zpResourceInstance( T* resource ) : m_resource( resource ) {}\
	zpResourceInstance( const zpResourceInstance<T>& instance )	: m_resource( instance.m_resource ) {}\
	zpResourceInstance( zpResourceInstance<T>&& instance ) : m_resource( instance.m_resource ) { instance.m_resource = ZP_NULL; }\
	~zpResourceInstance() { if( m_resource ) { m_resource->getContentManager()->destroyInstanceOfResource( *this ); }; m_resource = ZP_NULL; }\
	void operator=( const zpResourceInstance<T>& instance )	{ m_resource = instance.m_resource; }\
	void operator=( zpResourceInstance<T>&& instance ) { m_resource = instance.m_resource; instance.m_resource = ZP_NULL; }\
	operator zp_bool() const { return m_resource && m_resource->isLoaded(); }\
	T* getResource() const { return m_resource;	}\
private:	\
	T* m_resource;	\
	//friend class zpContentManager;

#define ZP_RESOURCE_INSTANCE_TEMPLATE_START_COPY( T, Copy )	\
	template<>	\
class zpResourceInstance<T> {	\
public:	\
	zpResourceInstance() : m_resource( ZP_NULL ) {}\
	zpResourceInstance( T* resource ) : m_resource( resource ) {}\
	zpResourceInstance( const zpResourceInstance<T>& instance )	: m_resource( instance.m_resource) { Copy( instance ); }\
	zpResourceInstance( zpResourceInstance<T>&& instance ) : m_resource( instance.m_resource ) { Copy( instance ); instance.m_resource = ZP_NULL; }\
	~zpResourceInstance() { m_resource = ZP_NULL; }\
	void operator=( const zpResourceInstance<T>& instance )	{ m_resource = instance.m_resource; Copy( instance ); }\
	void operator=( zpResourceInstance<T>&& instance ) { m_resource = instance.m_resource; Copy( instance ); instance.m_resource = ZP_NULL; }\
	operator zp_bool() const { return m_resource && m_resource->isLoaded(); }\
	T* getResource() const { return m_resource;	}\
private:	\
	T* m_resource;	\
	friend class zpContentManager;

#define ZP_RESOURCE_INSTANCE_TEMPLATE_START_KDC( T, Construct, Deconstruct, Copy )	\
	template<>	\
class zpResourceInstance<T> {	\
public:	\
	zpResourceInstance() : m_resource( ZP_NULL ) { Construct(); }\
	zpResourceInstance( T* resource ) : m_resource( resource ) { Construct(); }\
	zpResourceInstance( const zpResourceInstance<T>& instance )	: m_resource( instance.m_resource ) { Copy( instance ); }\
	zpResourceInstance( zpResourceInstance<T>&& instance ) : m_resource( instance.m_resource ) { Copy( instance ); instance.m_resource = ZP_NULL; }\
	~zpResourceInstance() { Deconstruct(); m_resource = ZP_NULL; }\
	void operator=( const zpResourceInstance<T>& instance )	{ m_resource = instance.m_resource; Copy( instance ); }\
	void operator=( zpResourceInstance<T>&& instance ) { m_resource = instance.m_resource; Copy( instance ); instance.m_resource = ZP_NULL; }\
	operator zp_bool() const { return m_resource && m_resource->isLoaded(); }\
	T* getResource() const { return m_resource;	}\
private:	\
	T* m_resource;	\
	friend class zpContentManager;

#define ZP_RESOURCE_INSTANCE_TEMPLATE_END	};

#endif