#pragma once
#ifndef ZP_LOAD_CONTENT_COMPONENT_H
#define ZP_LOAD_CONTENT_COMPONENT_H

class zpLoadContentComponent : public zpComponent {
public:
	zpLoadContentComponent();
	virtual ~zpLoadContentComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

	void onResourceLoaded( const zpString& filename, zp_bool loaded, zp_uint numLeft );
	void onResourcesFinishedLoading();

private:
	zpContentManager* m_manager;

	zp_bool m_isDestroyedWhenFinished;
	zp_bool m_isWorldDisabledWhenLoading;

	zpHashMap<zpString, zpString> m_immediateResources;
	zpHashMap<zpString, zpString> m_bufferedResources;
};

#endif