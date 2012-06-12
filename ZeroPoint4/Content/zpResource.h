#pragma once
#ifndef ZP_RESOURCE_H
#define ZP_RESOURCE_H

ZP_ABSTRACT_CLASS zpResource {
public:
	zpResource();
	virtual ~zpResource();

	virtual zp_bool load() = 0;
	virtual void unload() = 0;

	zpContentManager* getContentManager() const;
	const zpString& getFilename() const;

private:
	void setContentManager( zpContentManager* manager );
	void setFilename( const zpString& filename );

	zpContentManager* m_manager;
	zpString m_filename;

	friend class zpContentManager;
};

#endif