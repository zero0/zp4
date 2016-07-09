#pragma once
#ifndef ZP_THREAD_H
#define ZP_THREAD_H

typedef zp_dword (zpThreadProc)( zp_handle param );

zp_handle zp_create_thread( zpThreadProc func, zp_handle param );

class zpThread
{
    ZP_NON_COPYABLE( zpThread );
public:
    zpThread();
    ~zpThread();

    void start();
    void stop();

private:
    zp_handle m_threadHandle;
};

#endif
