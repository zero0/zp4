#pragma once
#ifndef ZP_BLEND_STATE_H
#define ZP_BLEND_STATE_H

class zpBlendStateImpl;

class zpBlendState
{
public:
	zpBlendState();
	~zpBlendState();

	zpBlendStateImpl* getBlendStateImpl() const;

private:
	zpBlendStateImpl* m_blendState;

	friend class zpRenderingEngine;
};

#endif