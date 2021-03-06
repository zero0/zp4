#pragma once
#ifndef ZP_MESSAGE_H
#define ZP_MESSAGE_H

typedef zp_uint zpMessageType;
typedef zp_ptr zpMessageDataHandle;
#define ZP_MESSAGE_TYPE_NULL    (zpMessageType)-1

class zpMessageDataCache;

#undef MT_DEF
#undef MT_SYS

#define MT_DEF( m )    m,
#define MT_SYS( m ) SYS_##m,

struct zpMessageTypes
{
    enum Types
    {
#include "zpMessageTypes.inc"

#ifdef ZP_MESSAGE_EXTERNAL_TYPES
#undef MT_SYS
#define MT_SYS( m ) ,
#include ZP_MESSAGE_EXTERNAL_TYPES
#endif
    };
};

#undef MT_DEF
#undef MT_SYS

class zpMessage
{
    ZP_NON_COPYABLE( zpMessage );
public:
    zpMessage();
    zpMessage( zpMessageType type, zpMessageReceiver* sender = ZP_NULL );
    zpMessage( zpMessage&& message );
    ~zpMessage();

    zpMessageType getMessageType() const;
    zpMessageReceiver* getSender() const;
    zp_uint getMessageSize() const;
    
    template< typename MessageData >
    zpMessage( zpMessageType type, const MessageData& data, zpMessageReceiver* sender = ZP_NULL )
        : m_type( type )
        , m_sender( sender )
        , m_data( zpMessageDataCache::getInstance()->storeMessageData( data ) )
        , m_dataSize( sizeof( MessageData ) )
    {}
    template< typename MessageData >
    const MessageData& getMessageData() const
    {
        ZP_ASSERT( m_dataSize == sizeof( MessageData ), "zpMessage: Inconsistent data size. Size: %d Given: %d", m_dataSize, sizeof( MessageData ) );
        return zpMessageDataCache::getInstance()->getMessageData< MessageData >( m_data, m_dataSize );
    }

private:
    zpMessageType m_type;
    zpMessageReceiver* m_sender;
    zpMessageDataHandle m_data;
    zp_uint m_dataSize;
};

class zpMessageDataCache
{
public:
    ~zpMessageDataCache();

    static zpMessageDataCache* getInstance();

    template< typename MessageData >
    zpMessageDataHandle storeMessageData( const MessageData& data )
    {
        return allocateMessageData( (void*)&data, sizeof( MessageData ) );
    }

    void removeMessageData( zpMessageDataHandle handle, zp_uint size );

    template< typename MessageData >
    const MessageData& getMessageData( zpMessageDataHandle handle, zp_uint size )
    {
        return *(const MessageData*)getMessageData( handle, size );
    }

private:
    static zpMessageDataCache s_instance;

    zpMessageDataCache();

    zpMessageDataHandle allocateMessageData( void* data, zp_uint size );
    
    void* getMessageData( zpMessageDataHandle& handle, zp_uint size );
};

class zpMessageTypeSystem
{
public:
    ~zpMessageTypeSystem();

    static zpMessageTypeSystem* getInstance();

    zpMessageType getMessageType( const zpString& msg ) const;
    const zpString& getMessageName( zpMessageType msg ) const;

private:
    zpMessageTypeSystem();

    void createMessages();

    static zpMessageTypeSystem s_instance;

    zpArrayList< zpString > m_names;
    zpArrayList< zpMessageType > m_types;
};

#endif