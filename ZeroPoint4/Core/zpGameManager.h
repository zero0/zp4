#pragma once
#ifndef ZP_GAME_MANAGER_H
#define ZP_GAME_MANAGER_H

ZP_ABSTRACT_CLASS zpGameManager : 
	public zpIntrusiveListNode<zpGameManager>, 
	public zpReferencedObject, 
	public zpMessageReceiver,
	public zpSerializable
{
public:
	zpGameManager();
	virtual ~zpGameManager();

	void create();
	void destroy();

	void update();

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	void setGame( zpGame* game );
	zpGame* getGame() const;

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpGame* m_game;

	struct {
		mutable zp_uint m_referenceCount : 31;
		mutable zp_bool m_isMarkedForAutoDelete : 1;
	};
};

#endif