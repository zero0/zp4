#pragma once
#ifndef ZP_WORLD_H
#define ZP_WORLD_H

class zpWorld : public zpIntrusiveListNode<zpWorld>, public zpReferencedObject {
public:
	zpWorld();
	virtual ~zpWorld();

	void setRootGameObject( zpGameObject* root );
	zpGameObject* getRootGameObject() const;

	void update();

	void create();
	void destroy();

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	const zpString& getName() const;
	void setName( const zpString& name );

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpGameObject* m_root;

	zpString m_name;
};

#endif