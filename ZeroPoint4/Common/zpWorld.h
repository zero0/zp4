#pragma once
#ifndef ZP_WORLD_H
#define ZP_WORLD_H

class zpWorld : public zpMessageReceiver, public zpSerializable {
public:
	zpWorld();
	virtual ~zpWorld();

	void addGameObject( zpGameObject* go );
	void removeGameObject( zp_uint index );
	zpGameObject* getGameObject( zp_uint index ) const;
	zp_uint getNumGameObjects() const;

	void update();
	void simulate();

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

private:
	zp_bool m_isEnabled;
	zp_bool m_isCreated;

	zpArrayList<zpGameObject*> m_gameObjects;
	zpGame* m_game;

	zpString m_name;
};

#endif