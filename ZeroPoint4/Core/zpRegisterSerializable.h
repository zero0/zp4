#pragma once
#ifndef ZP_REGISTER_SERIALIZABLE_H
#define ZP_REGISTER_SERIALIZABLE_H

class zpRegisterSerializable {
public:
	~zpRegisterSerializable();

	typedef zpSerializable* (*CreateSerializableFunction)();

	template<typename T>
	static void registerSerializable() {
		registerSerializable( ZP_SERIALIZE_TYPE( T ), zpRegisterSerializable::template defaultCreateSerializable<T> );
	}

	template<typename T>
	static void registerSerializable( CreateSerializableFunction createFunc ) {
		registerSerializable( ZP_SERIALIZE_TYPE( T ), createFunc );
	}

	template<typename T>
	static void registerSerializable( const zpString& name ) {
		registerSerializable( name, zpRegisterSerializable::template defaultCreateSerializable<T> );
	}

	static void registerSerializable( const zpString& name, CreateSerializableFunction createFunc );

	template<typename T>
	static T* createSerializable( const zpString& name ) {
		return (T*)createSerializable( name );
	};

	static zpSerializable* createSerializable( const zpString& name );

private:
	zpRegisterSerializable();
	zpRegisterSerializable( const zpRegisterSerializable& );
	void operator=( const zpRegisterSerializable& );

	template<typename T>
	static zpSerializable* defaultCreateSerializable() {
		return new T();
	}

	static zpHashMap<zpString, CreateSerializableFunction> s_creators;
};

#endif