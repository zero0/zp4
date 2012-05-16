#pragma once
#ifndef ZP_GAME_OBJECT_H
#define ZP_GAME_OBJECT_H

class zpGameObject : public zpIntrusiveListNode<zpGameObject> {
public:
	/*
	void* operator new( zp_uint size );
	void* operator new[]( zp_uint size );
	void operator delete( void* ptr );
	*/
	zpGameObject();
	~zpGameObject();

	void addChildGameObject( zpGameObject* go );
	void removeChildGameObject( zpGameObject* go );

	void addGameObjectComponent( zpGameObjectComponent* goc );
	void removeGameObjectComponent( zpGameObjectComponent* goc );

	void update();

	const zpIntrusiveList<zpGameObject>* getChildren() const { return &m_children; }
	const zpIntrusiveList<zpGameObjectComponent>* getComponents() const { return &m_components; }

private:
	zpGameObject* m_parent;

	zpIntrusiveList<zpGameObject> m_children;
	zpIntrusiveList<zpGameObjectComponent> m_components;

	zpMatrix4 m_transform;
	
};

#endif