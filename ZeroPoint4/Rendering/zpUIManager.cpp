#include "zpRendering.h"
#include "Awesomium\awesomium_capi.h"

zpUIManager::zpUIManager() {}
zpUIManager::~zpUIManager() {}

void zpUIManager::serialize( zpSerializedOutput* out ) {
    out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

    out->endBlock();
}
void zpUIManager::deserialize( zpSerializedInput* in ) {
    in->readBlock( ZP_SERIALIZE_TYPE_THIS );

    in->endBlock();
}

void zpUIManager::receiveMessage( const zpMessage& message ) {}

void zpUIManager::onCreate() {
    /*
    zpContentManager* content = this->getGame()->getGameManagerOfType<zpContentManager>();
    
    zpString root;
    content->getRootDirectoryForExtension( "html", root );

    awe_webcore_initialize_default();

    
    awe_webcore_initialize(
    false,
    true,
    false,
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    ll,
    false,
    awe_string_empty(),
    true,
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    awe_string_empty(),
    true,
    0,
    false,
    false,
    customCSS
    );
    */
}
void zpUIManager::onDestroy() {}

void zpUIManager::onUpdate() {
    //awe_webcore_update();
}

void zpUIManager::onEnabled() {}
void zpUIManager::onDisabled() {}
