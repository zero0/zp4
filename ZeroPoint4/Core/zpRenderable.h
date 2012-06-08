#pragma once
#ifndef ZP_RENDERABLE_H
#define ZP_RENDERABLE_H

ZP_PURE_INTERFACE zpRenderable {
	virtual void render() = 0;
};

#endif