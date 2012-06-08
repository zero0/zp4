#pragma once
#ifndef ZP_GAME_H
#define ZP_GAME_H

class zpGame {
public:
	zpGame();
	~zpGame();

	void addWorld( zpWorld* world, zp_bool andCreate = false );
	void removeWorld( zpWorld* world );
	zpWorld* getCurrentWorld() const;

	void setNextWorld( const zpString& worldName, zp_bool asynchCreateNextWorld = false );
	
	void addGameManager( zpGameManager* manager );
	void removeGameManager( zpGameManager* manager );

	void setRenderable( zpRenderable* renderable );
	zpRenderable* getRenderable() const;

	void process();

private:
	zpWorld* m_currentWorld;
	zpWorld* m_nextWorld;
	zpRenderable* m_renderable;

	zp_bool m_asynchCreateNextWorld;

	zpIntrusiveList<zpWorld> m_worlds;
	zpIntrusiveList<zpGameManager> m_managers;
};

#endif