#pragma once
#ifndef ZP_SERIALIZED_FACTORY_H
#define ZP_SERIALIZED_FACTORY_H

enum zpSerializedType {
	ZP_SERIALIZED_TYPE_XML,
	ZP_SERIALIZED_TYPE_JSON,
	ZP_SERIALIZED_TYPE_BINARY,
};

class zpSerializedFactory {
public:
	~zpSerializedFactory();

	static zpSerializedFactory* getInstance();

	zpSerializedInput* createSerializedInputFromFile( const zpString& filename );
	zpSerializedOutput* createSerializedOutputForFile( const zpString& filename );

private:
	zpSerializedFactory();

};

#endif