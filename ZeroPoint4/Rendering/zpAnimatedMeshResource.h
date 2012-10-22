#pragma once
#ifndef ZP_ANIMATED_MESH_RESOURCE_H
#define ZP_ANIMATED_MESH_RESOURCE_H

class zpAnimatedMeshResource : public zpResource {
public:
	zpAnimatedMeshResource();
	virtual ~zpAnimatedMeshResource();

	zp_bool load();
	void unload();

private:

};

#endif