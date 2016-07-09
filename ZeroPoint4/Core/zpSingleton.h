#pragma once
#ifndef ZP_SINGLETON_H
#define ZP_SINGLETON_H

template<typename T>
class zpSingleton {
public:
    static T& getInstance() {
        static T obj;
        return obj;
    }
};

#endif