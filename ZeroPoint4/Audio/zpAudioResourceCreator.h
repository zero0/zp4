#pragma once

class zpAudioResourceCreator : public zpResourceCreator {
public:
	zpAudioResourceCreator();
	virtual ~zpAudioResourceCreator();

	zpResource* createResource( const zpString& filename );
};

