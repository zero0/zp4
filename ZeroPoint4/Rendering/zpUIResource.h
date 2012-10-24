#pragma once
#ifndef ZP_UI_RESOURCE_H
#define ZP_UI_RESOURCE_H

class zpUIResource : public zpResource {
public:
	zp_bool load() { return true; }
	void unload() {}
};

ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpUIResource )
ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif