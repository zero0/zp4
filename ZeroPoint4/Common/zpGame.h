#pragma once
#ifndef ZP_GAME_H
#define ZP_GAME_H

class zpGame : public zpSerializable, public zpMessageReceiver {
public:
	zpGame();
	~zpGame();

	void create();
	void destroy();

	void setNextWorld( zpWorld* world );
	zpWorld* getCurrentWorld() const;
	zpWorld* getNextWorld() const;

	zpAllGameManagers* getGameManagers();

	void setRenderable( zpRenderable* renderable );
	zpRenderable* getRenderable() const;

	void process();

	void setWindow( zpWindow* window );
	zpWindow* getWindow() const;

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

private:
	zpTime* m_timer;

	zpWorld* m_currentWorld;
	zpWorld* m_nextWorld;
	zpRenderable* m_renderable;
	zpWindow* m_window;

	zp_long m_lastTime;
	zp_long m_simulateHz;
	zp_int m_renderMsHz;

	zpAllGameManagers m_managers;
};

#endif