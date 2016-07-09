#pragma once
#ifndef ZP_BOUNDING_SPHERE_H
#define ZP_BOUNDING_SPHERE_H

class zpBoundingAABB;

class zpBoundingSphere {
public:
    zpBoundingSphere();
    zpBoundingSphere( zpVector4fParamF center, zpScalarParamF radius );
    zpBoundingSphere( const zpBoundingSphere& sphere );
    zpBoundingSphere( zpBoundingSphere&& sphere );
    ~zpBoundingSphere();

    void operator=( const zpBoundingSphere& sphere );
    void operator=( zpBoundingSphere&& sphere );

    zpVector4f getCenter() const;
    void setCenter( zpVector4fParamF center );
    
    zpScalar getRadius() const;
    void setRadius( zpScalarParamF radius );

    void generateBoundingAABB( zpBoundingAABB& box ) const;

    void translate( zpVector4fParamF translate );
    void scale( zpScalarParamF scale );
    void pad( zpScalarParamF padding );
    
    void add( zp_float x, zp_float y, zp_float z );
    void add( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z );
    void add( zpVector4fParamF point );
    void add( const zpBoundingAABB& box );
    void add( const zpBoundingSphere& sphere );

private:
    zpVector4f m_center;
    zpScalar m_radius;
};

#endif