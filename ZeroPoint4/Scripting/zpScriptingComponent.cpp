#include "zpScripting.h"
#include "sqrat.h"

zpScriptingComponent::zpScriptingComponent() {}
zpScriptingComponent::~zpScriptingComponent() {}

void zpScriptingComponent::receiveMessage( const zpMessage& message ) {}

void zpScriptingComponent::serialize( zpSerializedOutput* out ) {}
void zpScriptingComponent::deserialize( zpSerializedInput* in ) {}

void zpScriptingComponent::onCreate() {}
void zpScriptingComponent::onDestroy() {}

void zpScriptingComponent::onUpdate() {}

void zpScriptingComponent::onEnabled() {}
void zpScriptingComponent::onDisabled() {}