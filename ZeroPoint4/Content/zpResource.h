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
	zp_bool isLoaded() const;

private:
	void setIsLoaded( zp_bool loaded );
	void setContentManager( zpContentManager* manager );
	void setFilename( const zpString& filename );

	zp_bool m_isLoaded;
	zpContentManager* m_manager;
	zpString m_filename;
	
	friend class zpContentManager;
};

#endif