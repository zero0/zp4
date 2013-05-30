#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

enum zpObjectFlag : zp_byte
{
	ZP_OBJECT_FLAG_ENABLED,
	ZP_OBJECT_FLAG_CREATED,
	ZP_OBJECT_FLAG_SHOULD_DESTROY,

	zpObjectFlag_Count,
};

class zpGameObject : public zpMessageReceiver
{
public:
	zpGameObject();
	~zpGameObject();

	zpAllComponents* getComponents();

	void setFlag( zpObjectFlag flag );
	void unsetFlag( zpObjectFlag flag );
	zp_bool isFlagSet( zpObjectFlag flag ) const;

	void create();
	void destroy();

	const zpString& getName() const;
	void setName( const zpString& name );

	const zpMatrix4f& getTransform() const;
	void setTransform( const zpMatrix4f& transform );

	void receiveMessage( const zpMessage& message );
	void sendMessageToComponents( const zpMessage& message );
	//void sendMessageToChildGameObjects( const zpMessage& message );
	//void sendMessageToParentGameObject( const zpMessage& message );

private:
	zpMatrix4f m_transform;
	zpString m_name;
	zpFlag8 m_flags;
	zpAllComponents m_components;
};

#endif