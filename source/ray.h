#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

#define arrayCount(array) (sizeof(array) / sizeof(array[0]))
#define square(a) (a * a)

struct Material
{
    float shininess;
    glm::fvec3 diffuseColor;
    glm::fvec3 emitColor;
};

struct Plane
{
    glm::fvec3 normal;
    float distance;
    unsigned materialIndex;
};

struct Sphere
{
    glm::fvec3 position;
    float radius;
    unsigned materialIndex;
};

struct World
{
    Material* materials;
    unsigned materialsCount;

    Plane* planes;
    unsigned planesCount;

    Sphere* spheres;
    unsigned spheresCount;
};

float randomZeroToOne();

float randomMinusOneToOne();

unsigned colorToUnsigned(glm::fvec3 color);

glm::fvec3 rayCast(const World* world, glm::fvec3 rayOrigin, glm::fvec3 rayDirection, unsigned depth);







#endif
