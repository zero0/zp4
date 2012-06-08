#pragma once
#ifndef ZP_GAME_OBJECT_H
#define ZP_GAME_OBJECT_H

class zpGameObject : 
	public zpIntrusiveListNode<zpGameObject>, 
	public zpReferencedObject, 
	public zpMessageReceiver,
	public zpSerializable
{
public:
	/*
	void* operator new( zp_uint size );
	void* operator new[]( zp_uint size );
	void operator delete( void* ptr );
	*/
	zpGameObject();
	virtual ~zpGameObject();

	void setParentGameObject( zpGameObject* go );
	zpGameObject* getParentGameObject() const;

	void addChildGameObject( zpGameObject* go );
	void removeChildGameObject( zpGameObject* go );

	void addComponent( zpComponent* goc );
	void removeComponent( zpComponent* goc );

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	void setWorld( zpWorld* world );
	zpWorld* getWorld() const;

	void update();

	void create();
	void destroy();

	const zpIntrusiveList<zpGameObject>* getChildGameObjects() const;
	const zpIntrusiveList<zpComponent>* getComponents() const;

	const zpGameObject* getChildGameObjectByName( const zpString& name ) const;
	zpGameObject* getChildGameObjectByName( const zpString& name );

	const zpComponent* getComponentByName( const zpString& name ) const;
	zpComponent* getComponentByName( const zpString& name );

	template<typename T>
	const T* getComponentOfType() const { return (const T*)getComponent_T( &typeid( T ) ); }
	template<typename T>
	T* getComponentOfType() { return (T*)getComponent_T( &typeid( T ) ); }

	const zpString& getName() const;
	void setName( const zpString& name );

	const zpMatrix4& getTransform() const;
	zpMatrix4 getComputedTransform() const;
	void setTransform( const zpMatrix4& transform );

	void receiveMessage( const zpMessage& message );
	void sendMessageToComponents( const zpMessage& message );
	void sendMessageToChildGameObjects( const zpMessage& message );
	void sendMessageToParentGameObject( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

private:
	zpComponent* getComponent_T( const void* type );

	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpGameObject* m_parentGameObject;
	zpWorld* m_world;

	zpIntrusiveList<zpGameObject> m_children;
	zpIntrusiveList<zpComponent> m_components;

	zpMatrix4 m_transform;
	zpString m_name;
};

#endif