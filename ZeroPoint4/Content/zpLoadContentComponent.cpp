#include "zpContent.h"

zpLoadContentComponent::zpLoadContentComponent()
	: m_manager( ZP_NULL )
	, m_isUnloadWhenDestroyed( true )
	, m_isWorldDisabledWhenLoading( true )
{}
zpLoadContentComponent::~zpLoadContentComponent() {
	destroy();
}

void zpLoadContentComponent::receiveMessage( const zpMessage& message ) {}

void zpLoadContentComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeBoolean( m_isUnloadWhenDestroyed, "@unload-when-destroyed" );
	out->writeBoolean( m_isWorldDisabledWhenLoading, "@disable-world-when-loading" );

	m_immediateResources.foreach( [ out ]( const zpString& alias, const zpString& filename ){
		out->writeBlock( "Content" );

		out->writeBoolean( true, "@immediate" );
		out->writeString( filename, "@file" );
		out->writeString( alias, "@alias" );

		out->endBlock();
	} );
	m_bufferedResources.foreach( [ out ]( const zpString& alias, const zpString& filename ){
		out->writeBlock( "Content" );

		out->writeBoolean( false, "@immediate" );
		out->writeString( filename, "@file" );
		out->writeString( alias, "@alias" );

		out->endBlock();
	} );

	out->endBlock();
}
void zpLoadContentComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readBoolean( &m_isUnloadWhenDestroyed, "@unload-when-destroyed" );
	in->readBoolean( &m_isWorldDisabledWhenLoading, "@disable-world-when-loading" );

	zpString filename, alias;
	zp_bool immediateLoad = false;
	in->readEachBlock( "Content", [ this, &filename, &alias, &immediateLoad ]( zpSerializedInput* in ) {
		in->readBoolean( &immediateLoad, "@immediate" );
		in->readString( &filename, "@file" );
		in->readString( &alias, "@alias" );

		if( immediateLoad ) {
			m_immediateResources.put( alias, filename );
		} else {
			m_bufferedResources.put( alias, filename );
		}
	} );
		
	in->endBlock();
}

void zpLoadContentComponent::onCreate() {
	m_manager = getGameManagerOfType<zpContentManager>();
	
	// if the world should be disabled while loading, disable it now
	if( m_isWorldDisabledWhenLoading ) {
		getWorld()->setEnabled( false );
	}
	
	// preload the buffered resources
	m_bufferedResources.foreach( [ this ]( const zpString& alias, const zpString& filename ) {
		m_manager->loadResource( filename, alias, false );
	} );

	// setup the callbacks
	m_manager->onResourceLoaded() += zpCreateMemberDelegate( &zpLoadContentComponent::onResourceLoaded, this );
	m_manager->onAllResourcesLoaded() += zpCreateMemberDelegate( &zpLoadContentComponent::onResourcesFinishedLoading, this );

	// load the immediate resources that are required now
	m_immediateResources.foreach( [ this ]( const zpString& alias, const zpString& filename ) {
		m_manager->loadResource( filename, alias, true );
	} );
}
void zpLoadContentComponent::onDestroy() {
	if( m_isUnloadWhenDestroyed ) {
		m_immediateResources.foreach( [ this ]( const zpString& alias, const zpString& value ) {
			m_manager->unloadResource( alias );
		} );
		m_bufferedResources.foreach( [ this ]( const zpString& alias, const zpString& value ) {
			m_manager->unloadResource( alias );
		} );
	}

	m_manager = ZP_NULL;

	m_immediateResources.clear();
	m_bufferedResources.clear();
}

void zpLoadContentComponent::onUpdate() {}

void zpLoadContentComponent::onEnabled() {}
void zpLoadContentComponent::onDisabled() {}

void zpLoadContentComponent::onResourceLoaded( const zpString& filename, zp_bool loaded, zp_uint numLeft ) {
	sendMessageToSiblingComponents( zpMessage( zpMessageTypes::RESOURCE_LOADED, this ) );
}
void zpLoadContentComponent::onResourcesFinishedLoading() {
	// if the world was disabled during loading, enable it
	if( m_isWorldDisabledWhenLoading ) {
		getWorld()->setEnabled( true );
	}

	sendMessageToSiblingComponents( zpMessage( zpMessageTypes::RESOURCES_FINISHED_LOADING, this ) );

	m_manager->onResourceLoaded() -= zpCreateMemberDelegate( &zpLoadContentComponent::onResourceLoaded, this );
	m_manager->onAllResourcesLoaded() -= zpCreateMemberDelegate( &zpLoadContentComponent::onResourcesFinishedLoading, this );
}
