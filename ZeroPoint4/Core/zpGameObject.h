#pragma once
#ifndef ZP_GAME_OBJECT_H
#define ZP_GAME_OBJECT_H

enum zpGameObjectFlag
{
	ZP_GAME_OBJECT_FLAG_ENABLED,
	ZP_GAME_OBJECT_FLAG_CREATED,

	ZP_GAME_OBJECT_FLAG_SHOULD_DESTROY,
};

class zpGameObject : public zpMessageReceiver, public zpSerializable
{
public:
	zpGameObject();
	virtual ~zpGameObject();

	zpAllComponents* getComponents();

	void setFlag( zpGameObjectFlag flag );
	void unsetFlag( zpGameObjectFlag flag );
	zp_bool isFlagSet( zpGameObjectFlag flag ) const;

	void setWorld( zpWorld* world );
	zpWorld* getWorld() const;

	void update();
	void simulate();

	void create();
	void destroy();

	const zpString& getName() const;
	void setName( const zpString& name );

	const zpMatrix4f& getTransform() const;
	void setTransform( const zpMatrix4f& transform );

	void receiveMessage( const zpMessage& message );
	void sendMessageToComponents( const zpMessage& message );
	void sendMessageToChildGameObjects( const zpMessage& message );
	void sendMessageToParentGameObject( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

private:
	zpFlag8 m_flags;
	zpWorld* m_world;

	zpAllComponents m_components;
	zpMatrix4f m_transform;
	
	zpString m_name;
};

#endif