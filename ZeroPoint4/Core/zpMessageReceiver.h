#pragma once
#ifndef ZP_MESSAGE_RECEIVER_H
#define ZP_MESSAGE_RECEIVER_H

ZP_PURE_INTERFACE zpMessageReceiver {
public:
    virtual void receiveMessage( const zpMessage& message ) = 0;
};

#endif