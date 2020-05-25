#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>


//TODO(Stanisz13): Can this be optimised further? is #define Triangle glm::fvec3 verts[3] better?
// Struct for one field looks kind of stupid...
struct Triangle
{
    glm::fvec3 vertices[3];
};

struct Mesh
{
    unsigned long long numberOfTriangles;
    Triangle* triangles;
};

Mesh loadMesh(const char* objPath, const char* mtlPath);

void loadMesh(const char* objPath, const char* mtlPath, Mesh* intoHere);

#endif
