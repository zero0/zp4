#ifndef OBJ_MESH_COMPILER_H
#define OBJ_MESH_COMPILER_H

class BaseMeshCompiler;

class ObjMessCompiler : public BaseMeshCompiler
{
public:
    virtual ~ObjMessCompiler();

protected:
    zp_bool compileMesh();
};

#endif
