#pragma once
#ifndef ZP_GAME_H
#define ZP_GAME_H

class zpGame : public zpSerializable, public zpMessageReceiver {
public:
	zpGame();
	~zpGame();

	void create();
	void destroy();

	void addWorld( zpWorld* world, zp_bool andCreate = false );
	void removeWorld( zpWorld* world );
	zpWorld* getCurrentWorld() const;

	void setNextWorld( const zpString& worldName, zp_bool asynchCreateNextWorld = false );
	
	void addGameManager( zpGameManager* manager );
	void removeGameManager( zpGameManager* manager );
	template<typename T>
	T* getGameManagerOfType() const {
		return (T*)getGameManager_T( &typeid( T ) );
	}

	void setRenderable( zpRenderable* renderable );
	zpRenderable* getRenderable() const;

	void process();

	void setWindow( zpWindow* window );
	zpWindow* getWindow() const;

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

private:
	zpGameManager* getGameManager_T( const void* type ) const;

	zpTime* m_timer;

	zpWorld* m_currentWorld;
	zpWorld* m_nextWorld;
	zpRenderable* m_renderable;
	zpWindow* m_window;

	zp_long m_lastTime;
	zp_long m_simulateHz;
	zp_int m_renderMsHz;

	zpIntrusiveList<zpWorld> m_worlds;
	zpIntrusiveList<zpGameManager> m_managers;
};

#endif