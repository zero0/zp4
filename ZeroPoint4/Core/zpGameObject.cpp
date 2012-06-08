#include "zpCore.h"
#include <typeinfo>

/*
void* zpGameObject::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
void* zpGameObject::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
void zpGameObject::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/
zpGameObject::zpGameObject() :
	m_isEnabled( true ),
	m_isCreated( false ),
	m_parentGameObject( ZP_NULL ),
	m_world( ZP_NULL ),
	m_children(),
	m_components(),
	m_transform(),
	m_name()
{}
zpGameObject::~zpGameObject() {}

void zpGameObject::setParentGameObject( zpGameObject* go ) {
	if( go == this ) return;
	if( go ) go->addReference();
	if( m_parentGameObject ) m_parentGameObject->removeReference();
	m_parentGameObject = go;
	
	setWorld( m_parentGameObject ? m_parentGameObject->getWorld() : ZP_NULL );
}
zpGameObject* zpGameObject::getParentGameObject() const {
	return m_parentGameObject;
}

void zpGameObject::addChildGameObject( zpGameObject* go ) {
	if( go && go != this ) {
		m_children.pushBack( go );
		go->setParentGameObject( this );
		go->addReference();
	}
}
void zpGameObject::removeChildGameObject( zpGameObject* go ) {
	if( go && go != this ) {
		m_children.remove( go );
		go->setParentGameObject( ZP_NULL );
		go->removeReference();
	}
}

void zpGameObject::addGameObjectComponent( zpGameObjectComponent* goc ) {
	if( goc ) {
		m_components.pushBack( goc );
		goc->setParentGameObject( this );
		goc->addReference();
	}
}
void zpGameObject::removeGameObjectComponent( zpGameObjectComponent* goc ) {
	if( goc ) {
		m_components.remove( goc );
		goc->setParentGameObject( ZP_NULL );
		goc->removeReference();
	}
}

void zpGameObject::setEnabled( zp_bool enabled ) {
	if( m_isEnabled == enabled ) return;
	m_isEnabled = enabled;

	m_components.foreach( [ enabled ]( zpGameObjectComponent* goc ) {
		goc->setEnabled( enabled );
	} );

	m_children.foreach( [ enabled ]( zpGameObject* goc ) {
		goc->setEnabled( enabled );
	});
}
zp_bool zpGameObject::isEnabled() const {
	return m_isEnabled;
}

void zpGameObject::setWorld( zpWorld* world ) {
	if( world ) world->addReference();
	if( m_world ) m_world->removeReference();
	m_world = world;
}
zpWorld* zpGameObject::getWorld() const {
	return m_world;
}

void zpGameObject::update() {
	if( m_isEnabled && m_isCreated ) {
		m_components.foreach( []( zpGameObjectComponent* goc ) {
			goc->onUpdate();
		} );

		m_children.foreach( []( zpGameObject* go ) {
			go->update();
		} );
	}
}

void zpGameObject::create() {
	if( m_isCreated ) return;
	m_isCreated = true;

	m_components.foreach( []( zpGameObjectComponent* goc ) {
		goc->onCreate();
	} );
	
	m_children.foreach( []( zpGameObject* go ) {
		go->create();
	} );
}
void zpGameObject::destroy() {
	if( !m_isCreated ) return;
	m_isCreated = false;

	m_components.foreach( []( zpGameObjectComponent* goc ) {
		goc->onDestroy();
	} );

	m_children.foreach( []( zpGameObject* go ) {
		go->destroy();
	} );
}

const zpIntrusiveList<zpGameObject>* zpGameObject::getChildGameObjects() const {
	return &m_children;
}
const zpIntrusiveList<zpGameObjectComponent>* zpGameObject::getGameObjectComponents() const {
	return &m_components;
}

const zpString& zpGameObject::getName() const {
	return m_name;
}
void zpGameObject::setName( const zpString& name ) {
	m_name = name;
}

const zpMatrix4& zpGameObject::getTransform() const {
	return m_transform;
}
zpMatrix4 zpGameObject::getComputedTransform() const {
	return m_parentGameObject ? m_transform * m_parentGameObject->getComputedTransform() : m_transform;
}
void zpGameObject::setTransform( const zpMatrix4& transform ) {
	m_transform = transform;
}

zpGameObjectComponent* zpGameObject::getGameObjectComponent_T( const void* type ) {
	const std::type_info& info = *(const std::type_info*)type;
	
	zpGameObjectComponent* component = m_components.findFirstIf( [ &info ]( const zpGameObjectComponent* component ) {
		return typeid( component ) == info;
	});

	return component;
}

void zpGameObject::receiveMessage( const zpMessage& message ) {
	sendMessageToGameObjectComponents( message );
	sendMessageToChildGameObjects( message );
}
void zpGameObject::sendMessageToGameObjectComponents( const zpMessage& message ) {
	m_components.foreach( [ &message ]( zpGameObjectComponent* goc ) {
		goc->receiveMessage( message );
	} );
}
void zpGameObject::sendMessageToChildGameObjects( const zpMessage& message ) {
	m_children.foreach( [ &message ]( zpGameObject* go ) {
		go->receiveMessage( message );
	} );
}
void zpGameObject::sendMessageToParentGameObject( const zpMessage& message ) {
	if( m_parentGameObject ) m_parentGameObject->receiveMessage( message );
}

void zpGameObject::serialize( zpSerializedOutput* out ) {}
void zpGameObject::deserialize( zpSerializedInput* in ) {}