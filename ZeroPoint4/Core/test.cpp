#include "zpCore.h"

int main() {
	//zpGameObject go;
	zpVector4 vv( 4, 5, 6 );
	//vv.add( zpVector4( 1, 0, 3, 0 ) );
	//vv.mul( zpVector4( 2, 4, 6, 8 ) );
	zp_float dot = vv.dot3( zpVector4( 1, 2, 3 ) );
	return 0;
}