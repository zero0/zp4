#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

enum zpObjectFlag : zp_ulong
{
	ZP_OBJECT_FLAG_ENABLED =		( 0 << 1 ),
	ZP_OBJECT_FLAG_CREATED =		( 1 << 1 ),
	ZP_OBJECT_FLAG_SHOULD_DESTROY =	( 2 << 1 ),

	ZP_OBJECT_FLAG_USER0 =			( 3 << 1 ),

	zpObjectFlag_Count,
};

class zpObjectPooledContent;

class zpObject
{
public:
	zpObject( const zp_char* filename );
	~zpObject();

	zpAllComponents* getComponents();

	void setFlag( zpObjectFlag flag );
	void unsetFlag( zpObjectFlag flag );
	zp_bool isFlagSet( zpObjectFlag flag ) const;

	const zpString& getName() const;
	void setName( const zpString& name );

	const zpMatrix4f& getTransform() const;
	void setTransform( const zpMatrix4f& transform );

	zpWorld* getWorld() const;
	void setWorld( zpWorld* world );

	zpApplication* getApplication() const;
	void setApplication( zpApplication* application );

private:
	zpMatrix4f m_transform;
	zpString m_name;
	zpFlag64 m_flags;
	zpAllComponents m_components;

	zpWorld* m_world;
	zpApplication* m_application;
};

class zpObjectPooledContent : public zpContentPool< zpObject, 256 >
{
public:
	zpObjectPooledContent();
	~zpObjectPooledContent();

	void update();

private:
};

#endif