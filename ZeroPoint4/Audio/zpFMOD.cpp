#include "zpAudio.h"
#include "zpFMOD.h"

#include "fmod.hpp"

zpFMOD::zpFMOD() {}
zpFMOD::~zpFMOD() {}

FMOD::System* zpFMOD::s_instance = ZP_NULL;
zp_bool zpFMOD::createInstance() {
	if( s_instance ) return true;
	FMOD_RESULT r;
	r = FMOD::System_Create( &s_instance );

	return r == FMOD_OK;
}
void zpFMOD::destroyInstance() {
	if( s_instance ) {
		s_instance->release();
		s_instance = ZP_NULL;
	}
}

FMOD::System* zpFMOD::getInstance() {
	return s_instance;
}
