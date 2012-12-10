#pragma once
#ifndef ZP_WORLD_RESOURCE_H
#define ZP_WORLD_RESOURCE_H

class zpWorldResource : public zpResource {
public:
	zpWorldResource();
	virtual ~zpWorldResource();

	zp_bool load();
	void unload();

	zpWorld* getWorld();

private:
	zpWorld m_world;
	zpXmlNode m_root;
};

ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpWorldResource )
ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif