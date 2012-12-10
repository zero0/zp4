#pragma once
#ifndef ZP_GAME_OBJECT_RESOURCE_H
#define ZP_GAME_OBJECT_RESOURCE_H

class zpGameObjectResource : public zpResource {
public:
	zpGameObjectResource();
	virtual ~zpGameObjectResource();

	zp_bool load();
	void unload();

	void addInstance( zpGameObject* go );
	void removeInstance( zpGameObject* go );

	zpXmlNode* getRoot();
	
private:
	zpXmlNode m_root;
	zpArrayList<zpGameObject*> m_instances;
};

template<>
class zpResourceInstance<zpGameObjectResource> {
public:
	zpResourceInstance()
		: m_resource( ZP_NULL )
	{}
	zpResourceInstance( zpGameObjectResource* resource )
		: m_resource( resource )
	{}
	zpResourceInstance( const zpResourceInstance<zpGameObjectResource>& instance )
		: m_resource( instance.m_resource )
	{}
	zpResourceInstance( zpResourceInstance<zpGameObjectResource>&& instance )
		: m_resource( instance.m_resource )
	{ instance.m_resource = ZP_NULL; }
	~zpResourceInstance() { m_resource = ZP_NULL; }

	void operator=( const zpResourceInstance<zpGameObjectResource>& instance )
	{ m_resource = instance.m_resource; }
	void operator=( zpResourceInstance<zpGameObjectResource>&& instance )
	{ m_resource = instance.m_resource; instance.m_resource = ZP_NULL; }

	operator zp_bool() const { return m_resource && m_resource->isLoaded(); }
	zpGameObjectResource* getResource() const { return m_resource; }

private:
	zpGameObjectResource* m_resource;
	friend class zpContentManager;
};

#if 0
ZP_RESOURCE_INSTANCE_TEMPLATE_START_KDC( zpGameObjectResource, onConstruct, onDeconstruct, onCopy )
public:
	zpGameObject* getGameObject() { return m_gameObject; }

private:
	void onConstruct() {
		m_gameObject = new zpGameObject;
		m_resource->addInstance( m_gameObject );
		if( m_resource->isLoaded() ) {
			zpXmlSerializedInput in( m_resource->getRoot(), zpProperties() );
			m_gameObject->deserialize( &in );
			m_gameObject->create();
		}
	}
	void onDeconstruct() {
		m_resource->removeInstance( m_gameObject );
		m_gameObject->destroy();
		ZP_SAFE_DELETE( m_gameObject );
	}
	void onCopy( const zpResourceInstance<zpGameObjectResource>& instance ) {
		if( m_gameObject ) {
			m_gameObject->destroy();
			ZP_SAFE_DELETE( m_gameObject );
		}

		m_gameObject = new zpGameObject;
		m_resource->addInstance( m_gameObject );
		if( m_resource->isLoaded() ) {
			zpXmlSerializedInput in( m_resource->getRoot(), zpProperties() );
			m_gameObject->deserialize( &in );
			m_gameObject->create();
		}
	}

	zpGameObject* m_gameObject;

ZP_RESOURCE_INSTANCE_TEMPLATE_END
#endif

#endif