#include "zpCore.h"
#include <memory.h>

zpMessage::zpMessage() : m_type( ZP_MESSAGE_TYPE_NULL ), m_sender( ZP_NULL ), m_data( 0 ), m_dataSize( 0 ) {}
zpMessage::zpMessage( zpMessageType type, zpMessageReceiver* sender ) : m_type( type ), m_sender( sender ), m_data( 0 ), m_dataSize( 0 ) {}
zpMessage::zpMessage( zpMessage&& message ) : m_type( message.m_type), m_sender( message.m_sender ), m_data( message.m_data ), m_dataSize( message.m_dataSize ) {
	message.m_type = 0;
	message.m_data = 0;
	message.m_dataSize = 0;
	message.m_sender = ZP_NULL;
}
zpMessage::~zpMessage() {
	if( m_data ) {
		zpMessageDataCache::getInstance()->removeMessageData( m_data, m_dataSize );
	}
};

zpMessageType zpMessage::getMessageType() const {
	return m_type;
}
zpMessageReceiver* zpMessage::getSender() const {
	return m_sender;
}
zp_uint zpMessage::getMessageSize() const {
	return m_dataSize;
}

zpMessageDataCache::zpMessageDataCache() {}
zpMessageDataCache::~zpMessageDataCache() {}

zpMessageDataCache zpMessageDataCache::s_instance;
zpMessageDataCache* zpMessageDataCache::getInstance() {
	return &s_instance;
}

void zpMessageDataCache::removeMessageData( zpMessageDataHandle handle, zp_uint size ) {
	if( size > sizeof( zpMessageDataHandle ) ) {
		free( (void*)handle );
	}
}
zpMessageDataHandle zpMessageDataCache::allocateMessageData( void* data, zp_uint size ) {
	if( size <= sizeof( zpMessageDataHandle ) ) {
		return (zpMessageDataHandle)*( zp_ptr*)data;
	} else {
		void* d = malloc( size );
		memcpy_s( d, size, data, size );
		return (zpMessageDataHandle)d;
	}
}
void* zpMessageDataCache::getMessageData( zpMessageDataHandle& handle, zp_uint size ) {
	if( size <= sizeof( zpMessageDataHandle ) ){ 
		return &handle;
	} else {
		return (void*)handle;
	}
}