#pragma once
#ifndef ZP_UI_MANAGER_H
#define ZP_UI_MANAGER_H

class zpUIManager {
public:
    zpUIManager();
    virtual ~zpUIManager();

    void serialize( zpSerializedOutput* out );
    void deserialize( zpSerializedInput* in );
    
    void receiveMessage( const zpMessage& message );

protected:
    void onCreate();
    void onDestroy();

    void onUpdate();

    void onEnabled();
    void onDisabled();

private:

};

#endif