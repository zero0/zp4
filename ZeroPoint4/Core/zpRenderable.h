#pragma once
#ifndef ZP_RENDERABLE_H
#define ZP_RENDERABLE_H

ZP_PURE_INTERFACE zpRenderable {
public:
    virtual void render() = 0;
};

#endif