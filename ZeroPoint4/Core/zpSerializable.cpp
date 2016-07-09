#include "zpCore.h"
#include <typeinfo>

const zp_char* __zp_serialize_type( const void* type ) {
    return &(*(const type_info*)type).name()[ 6 ];
}

void zpSerializableObject<zpVector2i>::serializeFrom( zpSerializedOutput* out, const zpVector2i& in) {
    out->writeInt( in.getX(), "@x" );
    out->writeInt( in.getY(), "@y" );
}
void zpSerializableObject<zpVector2i>::deserializeTo( zpSerializedInput* in, zpVector2i& out ) {
    zp_int x = 0, y = 0;

    in->readInt( &x, "@x" );
    in->readInt( &y, "@y" );

    out = zpVector2i( x, y );
}

void zpSerializableObject<zpVector4f>::serializeFrom( zpSerializedOutput* out, const zpVector4f& in) {
    out->writeFloat( zpMath::AsFloat( zpMath::Vector4GetX( in ) ), "@x" );
    out->writeFloat( zpMath::AsFloat( zpMath::Vector4GetX( in ) ), "@y" );
    out->writeFloat( zpMath::AsFloat( zpMath::Vector4GetX( in ) ), "@z" );
    out->writeFloat( zpMath::AsFloat( zpMath::Vector4GetX( in ) ), "@w" );
}
void zpSerializableObject<zpVector4f>::deserializeTo( zpSerializedInput* in, zpVector4f& out ) {
    zp_float x = 0.f, y = 0.f, z = 0.f, w = 0.f;

    in->readFloat( &x, "@x" );
    in->readFloat( &y, "@y" );
    in->readFloat( &z, "@z" );
    in->readFloat( &w, "@w" );

    out = zpMath::Vector4( x, y, z, w );
}

void zpSerializableObject<zpMatrix4f>::serializeFrom( zpSerializedOutput* out, const zpMatrix4f& in ) {
    //if( in.isIdentity() ) {
    //    out->writeBlock( "Identity" );
    //    out->endBlock();
    //} else {
    //    zpVector4f pos;
    //    in.getPosition( pos );
    //
    //    out->writeBlock( "Position" );
    //    zpSerializableObject<zpVector4f>::serializeFrom( out, pos );
    //    out->endBlock();
    //}
}
void zpSerializableObject<zpMatrix4f>::deserializeTo( zpSerializedInput* in, zpMatrix4f& out ) {
    //zp_bool empty = true;
    //
    //if( in->readBlock( "Position" ) ) {
    //    empty = false;
    //    zpVector4f pos;
    //    zpSerializableObject<zpVector4f>::deserializeTo( in, pos );
    //    out.setPosition( pos );
    //    in->endBlock();
    //}
    //
    //if( empty ) {
    //    out = zpMatrix4f::identity;
    //}
}

void zpSerializableObject<zpColor4f>::serializeFrom( zpSerializedOutput* out, const zpColor4f& in) {
    out->writeFloat( in.getRed(), "@r" );
    out->writeFloat( in.getGreen(), "@g" );
    out->writeFloat( in.getBlue(), "@b" );
    out->writeFloat( in.getAlpha(), "@a" );
}
void zpSerializableObject<zpColor4f>::deserializeTo( zpSerializedInput* in, zpColor4f& out ) {
    zp_float r = 0.f, g = 0.f, b = 0.f, a = 0.f;

    in->readFloat( &r, "@r" );
    in->readFloat( &g, "@g" );
    in->readFloat( &b, "@b" );
    in->readFloat( &a, "@a" );

    out = zpColor4f( r, g, b, a );
}
