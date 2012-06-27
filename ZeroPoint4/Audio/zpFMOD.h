#pragma once
#ifndef ZP_FMOD_H
#define ZP_FMOD_H

#if ZP_DEBUG
	#if ZP_WIN_32
		#pragma comment( lib, "fmodexL_vc.lib" )
	#else
		#pragma comment( lib, "fmodexL64_vc.lib" )
	#endif
#else
	#if ZP_WIN_32
		#pragma comment( lib, "fmodex_vc.lib" )
	#else
		#pragma comment( lib, "fmodex64_vc.lib" )
	#endif
#endif

namespace FMOD {
	class System;
};

class zpFMOD {
public:
	~zpFMOD();

	static zp_bool createInstance();
	static void destroyInstance();

	static FMOD::System* getInstance();

private:
	zpFMOD();

	static FMOD::System* s_instance;

};

#endif