#include "zpCore.h"

zpGameObject::zpGameObject() :
	m_parent( ZP_NULL ),
	m_components( ZP_NULL )
{
	m_children.setParent( this );
	//m_node.m_parent = this;
}
zpGameObject::~zpGameObject() {}

void zpGameObject::addChildGameObject( zpGameObject* go ) {
	m_children.prepend( go );
}
void zpGameObject::removeChildGameObject( zpGameObject go ) {
	
}

void zpGameObject::addGameObjectComponent( zpGameObjectComponent* goc ) {
	m_components.prepend( goc );
}
void zpGameObject::removeGameObjectComponent( zpGameObjectComponent* goc ) {
}

void zpGameObject::update() {
}
