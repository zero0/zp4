#include "zpCore.h"
#include <typeinfo>

const zp_char* __zp_serialize_type( const void* type ) {
	return &(*(const type_info*)type).name()[ 6 ];
}