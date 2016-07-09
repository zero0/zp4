#pragma once
#ifndef ZP_BOUNDING_AABB_H
#define ZP_BOUNDING_AABB_H

class zpBoundingSphere;

class zpBoundingAABB {
public:
    zpBoundingAABB();
    zpBoundingAABB( zp_float width, zp_float height, zp_float depth );
    zpBoundingAABB( const zpBoundingAABB& box );
    zpBoundingAABB( zpBoundingAABB&& box );
    ~zpBoundingAABB();

    void reset();

    zpVector4f getMin() const;
    zpVector4f getMax() const;
    void setMin( zpVector4fParamF min );
    void setMax( zpVector4fParamF max );
    void setMinMax( zpVector4fParamF min, zpVector4fParamF max );

    zpVector4f getCenter() const;
    zpVector4f getExtents() const;

    void setCenter(  zpVector4fParamF center );
    void setExtents( zpVector4fParamF extents );

    void generateBoundingSphere( zpBoundingSphere& sphere ) const;

    zpScalar getWidth() const;
    zpScalar getHeight() const;
    zpScalar getDepth() const;
    zpVector4f getSize() const;

    void translate( zpVector4fParamF translate );
    void scaleUniform( zpScalarParamF scale );
    void scale( zpVector4fParamF scale );
    void padUniform( zpScalarParamF padding );
    void pad( zpVector4fParamF padding );

    void add( zp_float x, zp_float y, zp_float z );
    void add( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z );
    void add( zpVector4fParamF point );
    void add( const zpBoundingAABB& box );
    void add( const zpBoundingSphere& sphere );

private:
    zpVector4f m_center;
    zpVector4f m_extent;
};

#endif
