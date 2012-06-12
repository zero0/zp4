#pragma once
#ifndef ZP_RESOURCE_CREATOR_H
#define ZP_RESOURCE_CREATOR_H

ZP_ABSTRACT_CLASS zpResourceCreator : public zpSerializable {
public:
	zpResourceCreator();
	virtual ~zpResourceCreator();

	virtual zpResource* createResource( const zpString& filename ) = 0;

	void setRootDirectory( const zpString& rootDirectory );
	const zpString& getRootDirectory() const;

	virtual void serialize( zpSerializedOutput* out );
	virtual void deserialize( zpSerializedInput* in );

private:
	zpString m_rootDirectory;
};

#endif