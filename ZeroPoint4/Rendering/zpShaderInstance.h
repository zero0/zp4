#pragma once
#ifndef ZP_SHADER_INSTANCE_H
#define ZP_SHADER_INSTANCE_H

template<>
class zpResourceInstance<zpShaderResource> {
public:
	zpResourceInstance();
	zpResourceInstance( zpShaderResource* resource );
	~zpResourceInstance();

	void operator=( const zpResourceInstance<zpShaderResource>& instance );
	void operator=( zpResourceInstance<zpShaderResource>&& instance );

	operator zp_bool() const;

	zpShaderResource* getShaderResource() const;

private:
	zpShaderResource* m_resource;
};

#endif