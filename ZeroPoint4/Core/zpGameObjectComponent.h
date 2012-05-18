#pragma once
#ifndef ZP_GAME_OBJECT_COMPONENT_H
#define ZP_GAME_OBJECT_COMPONENT_H

class zpGameObjectComponent : public zpIntrusiveListNode<zpGameObjectComponent>, public zpReferencedObject {
public:
	zpGameObjectComponent();
	virtual ~zpGameObjectComponent();

	void setParentGameObject( zpGameObject* parent );
	zpGameObject* getParentGameObject() const;

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zp_bool m_isEnabled;

	zpGameObject* m_parentGameObject;
};

#endif