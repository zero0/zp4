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
	m_referenceCount( 1 ),
	m_isMarkedForAutoDelete( false ),
	m_parentGameObject( ZP_NULL ),
	m_world( ZP_NULL ),
	m_children(),
	m_components(),
	m_transform(),
	m_name()
{}
zpGameObject::~zpGameObject() {
	destroy();
}

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

void zpGameObject::addComponent( zpComponent* goc ) {
	if( goc ) {
		m_components.pushBack( goc );
		goc->setParentGameObject( this );
		goc->addReference();
	}
}
void zpGameObject::removeComponent( zpComponent* goc ) {
	if( goc ) {
		m_components.remove( goc );
		goc->setParentGameObject( ZP_NULL );
		goc->removeReference();
	}
}

void zpGameObject::setEnabled( zp_bool enabled ) {
	if( m_isEnabled == enabled ) return;
	m_isEnabled = enabled;

	m_components.foreach( [ enabled ]( zpComponent* goc ) {
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
		m_components.foreach( []( zpComponent* goc ) {
			goc->update();
		} );

		m_children.foreach( []( zpGameObject* go ) {
			go->update();
		} );
	}
}

void zpGameObject::create() {
	if( m_isCreated ) return;
	m_isCreated = true;

	m_components.foreach( []( zpComponent* goc ) {
		goc->create();
	} );
	
	m_children.foreach( []( zpGameObject* go ) {
		go->create();
	} );
}
void zpGameObject::destroy() {
	if( !m_isCreated ) return;
	m_isCreated = false;

	m_components.foreach( []( zpComponent* goc ) {
		goc->destroy();
	} );

	m_children.foreach( []( zpGameObject* go ) {
		go->destroy();
	} );
}

const zpIntrusiveList<zpGameObject>* zpGameObject::getChildGameObjects() const {
	return &m_children;
}
const zpIntrusiveList<zpComponent>* zpGameObject::getComponents() const {
	return &m_components;
}

const zpString& zpGameObject::getName() const {
	return m_name;
}
void zpGameObject::setName( const zpString& name ) {
	m_name = name;
}

const zpMatrix4f& zpGameObject::getTransform() const {
	return m_transform;
}
zpMatrix4f zpGameObject::getComputedTransform() const {
	return m_parentGameObject ? m_transform * m_parentGameObject->getComputedTransform() : m_transform;
}
void zpGameObject::setTransform( const zpMatrix4f& transform ) {
	m_transform = transform;
}

zpComponent* zpGameObject::getComponent_T( const void* type ) {
	const std::type_info& info = *(const std::type_info*)type;
	
	zpComponent* component = m_components.findFirstIf( [ &info ]( const zpComponent* component ) {
		return typeid( component ) == info;
	});

	return component;
}

void zpGameObject::receiveMessage( const zpMessage& message ) {
	sendMessageToComponents( message );
	sendMessageToChildGameObjects( message );
}
void zpGameObject::sendMessageToComponents( const zpMessage& message ) {
	m_components.foreach( [ &message ]( zpComponent* goc ) {
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

void zpGameObject::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeString( m_name, "@name" );

	out->writeBlock( "Components" );
	{
		out->writeBlock( "Component" );
		m_components.foreach( [ &out ]( zpComponent* comp ) {
			comp->serialize( out );
		} );
		out->endBlock();
	}
	out->endBlock();

	out->writeBlock( "Children" );
	{
		out->writeBlock( "Child" );
		m_children.foreach( [ &out ]( zpGameObject* go ) {
			go->serialize( out );
		} );
		out->endBlock();
	}
	out->endBlock();

	out->endBlock();
}
void zpGameObject::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	setName( in->readString( "@name" ) );

	if( in->readBlock( "Components" ) )
	{
		in->readEach( "Component", [ this, &in ](){
			zpComponent* comp = in->readSerializableType<zpComponent>();
			if( comp ) addComponent( comp );
		} );
	}
	in->endBlock();

	if( in->readBlock( "Children" ) )
	{
		in->readEach( "Child", [ this, &in ](){
			zpGameObject* go = in->readSerializableType<zpGameObject>();
			if( go ) addChildGameObject( go );
		} );
	}
	in->endBlock();

	in->endBlock();
}

void zpGameObject::addReference() const {
	++m_referenceCount;
}
zp_bool zpGameObject::removeReference() const {
	--m_referenceCount;
	if( m_referenceCount == 0 ) {
		if( m_isMarkedForAutoDelete ) delete this;
		return true;
	}
	return false;
}

zp_uint zpGameObject::getReferenceCount() const {
	return m_referenceCount;
}

void zpGameObject::markForAutoDelete( zp_bool marked ) const {
	m_isMarkedForAutoDelete = marked;
}
zp_bool zpGameObject::isMarkedForAutoDelete() const {
	return m_isMarkedForAutoDelete;
}