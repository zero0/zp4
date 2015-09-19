#pragma once
#ifndef ZP_RAY_H
#define ZP_RAY_H

class zpRay {
public:
	zpRay();
	zpRay( zpVector4fParamF origin, zpVector4fParamF direction );
	zpRay( const zpRay& ray );
	zpRay( zpRay&& ray );
	~zpRay();

	void operator=( const zpRay& ray );
	void operator=( zpRay&& ray );

	zpVector4f getOrigin() const;
	void setOrigin( zpVector4fParamF origin );

	zpVector4f getDirection() const;
	void setDirection( zpVector4fParamF direction );

	zpVector4f getPointAt( zpScalarParamF t ) const;
	zp_bool isPointOnRay( zpVector4fParamF point ) const;

private:
	zpVector4f m_origin;
	zpVector4f m_direction;
};

#endif