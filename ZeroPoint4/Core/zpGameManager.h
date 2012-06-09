#pragma once
#ifndef ZP_GAME_MANAGER_H
#define ZP_GAME_MANAGER_H

class zpGameManager : 
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

protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;
};

#endif