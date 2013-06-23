#pragma once
#ifndef ZP_WORLD_H
#define ZP_WORLD_H

class zpWorldPooledContent;

class zpWorld
{
public:
	zpWorld( const zp_char* filename );
	~zpWorld();

	void addObject( zpObject* go );
	void removeObject( zpObject* go );
	void removeObjectAtIndex( zp_uint index );
	zpObject* getObject( zp_uint index ) const;
	zp_uint getNumObjects() const;

	void create();
	void destroy();

	const zpString& getName() const;

	void setApplication( zpApplication* application );
	zpApplication* getApplication() const;

private:
	zpArrayList<zpObject*> m_objects;
	zpApplication* m_application;

	zpString m_name;
};

class zpWorldPooledContent : public zpContentPool< zpWorld, 8 >
{
public:
	zpWorldPooledContent();
	~zpWorldPooledContent();

private:
};

#endif
