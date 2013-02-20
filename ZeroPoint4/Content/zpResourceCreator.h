#pragma once
#ifndef ZP_RESOURCE_CREATOR_H
#define ZP_RESOURCE_CREATOR_H

ZP_PURE_INTERFACE zpResourceCreator
{
public:
	virtual zpResource* createResource( const zpString& filename ) = 0;
	virtual void destroyResource( zpResource* res ) = 0;
};

#endif