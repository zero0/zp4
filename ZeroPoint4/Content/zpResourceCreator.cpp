#include "zpContent.h"

zpResourceCreator::zpResourceCreator() {}
zpResourceCreator::~zpResourceCreator() {}

void zpResourceCreator::setRootDirectory( const zpString& rootDirectory ) {
	m_rootDirectory = rootDirectory;
}
const zpString& zpResourceCreator::getRootDirectory() const {
	return m_rootDirectory;
}

void zpResourceCreator::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeString( m_rootDirectory, "@root" );

	out->endBlock();
}
void zpResourceCreator::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readString( &m_rootDirectory, "@root" );

	in->endBlock();
}