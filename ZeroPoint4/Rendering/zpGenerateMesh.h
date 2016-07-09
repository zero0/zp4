#ifndef ZP_GENERATE_MESH_H
#define ZP_GENERATE_MESH_H

struct zpMesh;

struct zpPlaneOps
{
    zp_float width;
    zp_float height;
    zp_uint widthSegments;
    zp_uint heightSegments;
    zpVector4f up;
    zpRectf uvRect;
    zp_float rotationAngle;
    zp_bool isTopLeftCenter;

    zpPlaneOps();
};

class zpGenerateMesh
{
    ZP_NON_COPYABLE( zpGenerateMesh );
public:
    static void Plane( zpArrayList< zpVector4f >& verts, zpArrayList< zpVector4f >& normals, zpArrayList< zpVector2f >& uvs, zpArrayList< zp_short >& triangles, const zpPlaneOps& ops );

    static void Rebuild( zpRenderingEngine* engine, zpMesh* mesh, zpArrayList< zpVector4f >& verts, zpArrayList< zpVector4f >& normals, zpArrayList< zpVector2f >& uvs, zpArrayList< zp_short >& triangles );
private:
    zpGenerateMesh() {}

};

#endif