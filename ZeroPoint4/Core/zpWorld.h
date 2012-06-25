#pragma once
#ifndef ZP_WORLD_H
#define ZP_WORLD_H

class zpWorld : 
	public zpIntrusiveListNode<zpWorld>, 
	public zpReferencedObject, 
	public zpMessageReceiver,
	public zpSerializable
{
public:
	zpWorld();
	virtual ~zpWorld();

	void setRootGameObject( zpGameObject* root );
	zpGameObject* getRootGameObject() const;

	void update();

	void create();
	void destroy();
	
	zp_bool isCreated() const;

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	const zpString& getName() const;
	void setName( const zpString& name );

	void setGame( zpGame* game );
	zpGame* getGame() const;

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpGameObject* m_root;
	zpGame* m_game;

	struct {
		mutable zp_uint m_referenceCount : 31;
		mutable zp_bool m_isMarkedForAutoDelete : 1;
	};

	zpString m_name;
};

#endif