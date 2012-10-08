#include "zpCore.h"
#include <typeinfo>

const zp_char* __zp_serialize_type( const void* type ) {
	return &(*(const type_info*)type).name()[ 6 ];
}

void zpSerializableObject<zpVector4f>::serializeAs( zpSerializedOutput* out, zpVector4f& in) {
	out->writeFloat( zp_real_to_float( in.getX() ), "@x" );
	out->writeFloat( zp_real_to_float( in.getY() ), "@y" );
	out->writeFloat( zp_real_to_float( in.getZ() ), "@z" );
	out->writeFloat( zp_real_to_float( in.getW() ), "@w" );
}
void zpSerializableObject<zpVector4f>::deserializeAs( zpSerializedInput* in, zpVector4f& out ) {
	zp_float x = 0.f, y = 0.f, z = 0.f, w = 0.f;

	in->readFloat( &x, "@x" );
	in->readFloat( &y, "@y" );
	in->readFloat( &z, "@z" );
	in->readFloat( &w, "@w" );

	out = zpVector4f( x, y, z, w );
}

void zpSerializableObject<zpMatrix4f>::serializeAs( zpSerializedOutput* out, zpMatrix4f& in ) {
	if( in.isIdentity() ) {
		out->writeBlock( "Identity" );
		out->endBlock();
	} else {
		zpVector4f pos;
		in.getPosition( pos );

		out->writeBlock( "Position" );
		zpSerializableObject<zpVector4f>::serializeAs( out, pos );
		out->endBlock();
	}
}
void zpSerializableObject<zpMatrix4f>::deserializeAs( zpSerializedInput* in, zpMatrix4f& out ) {
	if( in->readBlock( "Identity" ) ) {
		out = zpMatrix4f::identity;
	}
	in->endBlock();

	if( in->readBlock( "Position" ) ) {
		zpVector4f pos;
		zpSerializableObject<zpVector4f>::deserializeAs( in, pos );
		out.setPosition( pos );
	}
	in->endBlock();
}
