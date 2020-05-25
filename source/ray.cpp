#include "ray.h"

#include <iostream>

//NOTE(Stanisz): color components in 0 - 1 range
unsigned colorToUnsigned(glm::fvec3 color)
{
    //TODO(Stanisz): check if works on windows.
    color = 255.0f * color;
    unsigned res = 0;
    res |= ((unsigned char)255 << 24);
    res |= ((unsigned char)color.x << 16);
    res |= ((unsigned char)color.y << 8);
    res |= ((unsigned char)color.z << 0);
    
    return res;
}

float rayIntersectsPlane(const glm::fvec3& rayOrigin, const glm::fvec3& rayDirection,
                         const glm::fvec3& planeNormal, float planeDistance)
{
    float denom = glm::dot(planeNormal, rayDirection);

    //NOTE(Stanisz): the denom is 0, we did not hit the plane at all.
    if ((denom > -FLT_EPSILON) && (denom < FLT_EPSILON))
    {
        return FLT_MAX;
    }

    float result = ((planeDistance - glm::dot(planeNormal, rayOrigin)) /
                    denom);

    return result;
}

glm::fvec3 hadamard(const glm::fvec3& a, const glm::fvec3& b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}


float rayIntersectsSphere(const glm::fvec3& rayOrigin, const glm::fvec3& rayDirection,
                          const glm::fvec3& spherePosition, float radius)
{
    float result = FLT_MAX;

    glm::fvec3 sphereRelativeOrigin = rayOrigin - spherePosition;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0 * glm::dot(rayDirection, sphereRelativeOrigin);
    float c = glm::dot(sphereRelativeOrigin, sphereRelativeOrigin) - radius*radius;

    float denom = 2.0 * a;

    float delta = b * b - 4.0 * a * c;
    if (delta > FLT_EPSILON)
    {
        float rootDelta = sqrt(delta);
        float t1 = (-b -rootDelta) / denom;
        float t2 = (-b +rootDelta) / denom;

        t1 = glm::max(t1, (float)0.0);
        t2 = glm::max(t2, (float)0.0);
        
        result = glm::min(t1, t2);
    }
    
    return result;
}

float lerp(float a, float b, float t)
{
    return ((1.0f - t) * a + b);
}


glm::fvec3 lerpVec3(const glm::fvec3& a, const glm::vec3& b, const float t)
{
    return {lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t)};
}

float randomZeroToOne()
{
    return (float)rand() / RAND_MAX;
}

float randomMinusOneToOne()
{
    float r = 2.0f * randomZeroToOne() -1.0f;
    
    return r;
}

glm::fvec3 rayCast(const World* world, glm::fvec3 rayOrigin, glm::fvec3 rayDirection,
                   unsigned depth)
{
    float minHitDistance = 0.001f;

    glm::fvec3 resultingColor = {};
    glm::fvec3 attenuation(1);

    for (unsigned d = 0; d < depth; ++d)
    {
        float closestHitDistance = FLT_MAX;
        unsigned closestHitMaterial = 0;
        glm::fvec3 closestHitNormal;
        glm::fvec3 closestHitPoint;
        
        for (unsigned i = 0; i < world->spheresCount; ++i)
        {
            const Sphere cur = world->spheres[i];

            float hitDistance = rayIntersectsSphere(rayOrigin, rayDirection, cur.position, cur.radius); 
            if (hitDistance < FLT_MAX && hitDistance > minHitDistance)
            {
                if (hitDistance < closestHitDistance)
                {
                    closestHitDistance = hitDistance;
                    closestHitMaterial = cur.materialIndex;

                    closestHitPoint = rayOrigin + hitDistance * rayDirection;
                    closestHitNormal = glm::normalize(closestHitPoint - cur.position);
                }
            }
        }

        for (unsigned i = 0; i < world->planesCount; ++i)
        {
            const Plane cur = world->planes[i];

            float hitDistance = rayIntersectsPlane(rayOrigin, rayDirection, cur.normal, cur.distance); 
            if (hitDistance < FLT_MAX && hitDistance > minHitDistance)
            {
                if (hitDistance < closestHitDistance)
                {
                    closestHitDistance = hitDistance;
                    closestHitMaterial = cur.materialIndex;

                    closestHitNormal = cur.normal;
                    closestHitPoint = rayOrigin + hitDistance * rayDirection;
                }
            }
        }

        const Material hitMaterial = world->materials[closestHitMaterial];
        resultingColor += hadamard(attenuation, hitMaterial.emitColor);

        if (closestHitMaterial != 0)
        {
            float cosRayDirNormal =  glm::dot(-rayDirection, closestHitNormal);
            if (cosRayDirNormal < 0)
            {
                cosRayDirNormal = 0; 
            }

            attenuation = hadamard(attenuation, hitMaterial.diffuseColor * cosRayDirNormal);
           
            glm::fvec3 pureBounce = rayDirection + 2.0f * cosRayDirNormal * closestHitNormal;                                                                         
            glm::fvec3 randomBounce(randomMinusOneToOne(), randomMinusOneToOne(), randomMinusOneToOne());

            randomBounce += closestHitNormal;
            randomBounce = glm::normalize(randomBounce);

            rayOrigin = closestHitPoint;
            rayDirection = glm::normalize(lerpVec3(randomBounce, pureBounce, hitMaterial.shininess));
        }
        else
        {
            return resultingColor;
        }                                  
    }
        
    return resultingColor;
}

