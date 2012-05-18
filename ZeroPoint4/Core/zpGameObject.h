#pragma once
#ifndef ZP_GAME_OBJECT_H
#define ZP_GAME_OBJECT_H

class zpGameObject : public zpIntrusiveListNode<zpGameObject>, public zpReferencedObject {
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

	void addGameObjectComponent( zpGameObjectComponent* goc );
	void removeGameObjectComponent( zpGameObjectComponent* goc );

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	void setWorld( zpWorld* world );
	zpWorld* getWorld() const;

	void update();

	void create();
	void destroy();

	const zpIntrusiveList<zpGameObject>* getChildren() const { return &m_children; }
	const zpIntrusiveList<zpGameObjectComponent>* getComponents() const { return &m_components; }

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpGameObject* m_parent;
	zpWorld* m_world;

	zpIntrusiveList<zpGameObject> m_children;
	zpIntrusiveList<zpGameObjectComponent> m_components;

	zpMatrix4 m_transform;
	
};

#endif