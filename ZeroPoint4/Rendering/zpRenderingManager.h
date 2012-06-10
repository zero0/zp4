#pragma once
#ifndef ZP_RENDERING_MANAGER_H
#define ZP_RENDERING_MANAGER_H

class zpRenderingManager : public zpGameManager {
public:
	zpRenderingManager();
	virtual ~zpRenderingManager();

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	zpRenderingEngine* getRenderingEngine() const;

	void setRenderingEngineType( zpRenderingEngineType type );
	zpRenderingEngineType getRenderingEngineType() const;

	void receiveMessage( const zpMessage& message );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpRenderingEngine* m_engine;

	zpRenderingEngineType m_type;
};

#endif