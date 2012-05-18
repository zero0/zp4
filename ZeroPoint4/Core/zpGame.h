#pragma once
#ifndef ZP_GAME_H
#define ZP_GAME_H

class zpGame {
public:
	zpGame();
	~zpGame();

	void addWorld( zpWorld* world );
	void removeWorld( zpWorld* world );
	zpWorld* getCurrentWorld() const;

	void addGameManager( zpGameManager* manager );
	void removeGameManager( zpGameManager* manager );

	void update();

private:
	zpWorld* m_currentWorld;
	zpWorld* m_nextWorld;

	zpIntrusiveList<zpWorld> m_worlds;
	zpIntrusiveList<zpGameManager> m_managers;
};

#endif