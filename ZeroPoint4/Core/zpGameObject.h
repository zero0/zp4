#pragma once
#ifndef ZP_GAME_OBJECT_H
#define ZP_GAME_OBJECT_H

class zpGameObject : public zpLinkNode<zpGameObject> {
public:
	zpGameObject();
	~zpGameObject();

	void addChildGameObject( zpGameObject* go );
	void removeChildGameObject( zpGameObject go );

	void addGameObjectComponent( zpGameObjectComponent* goc );
	void removeGameObjectComponent( zpGameObjectComponent* goc );

	void update();

	const zpLink<zpGameObject>* getChildren() const { return &m_children; }
	const zpLink<zpGameObjectComponent>* getComponents() const { return &m_components; }

private:
	zpGameObject* m_parent;

	zpLink<zpGameObject> m_children;
	zpLink<zpGameObjectComponent> m_components;

	zpVector4 m_vector;
};

#endif