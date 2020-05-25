#include "ray.h"
#include "image.h"
#include "model.h"
#include <cstdio>

//TODO(Stanisz13): Multiple rays per pixel should sample slightly
// different light directions? Blue noise sampling.
//NOTE(Stanisz13): i implemented jittered sampling, so that's enough blue-noisy for now.

//TODO(Stanisz13): replace rand in raycast

//TODO(Stanisz13): better light reflection using correct hemisphere sampling

int main()
{
    const unsigned windowWidth = 1280, windowHeight = 720;
    const unsigned raysPerPixel = 9; //NOTE(Stanisz13): must be a perfect square, jittered sampling.
    const unsigned raycastingDepth = 32;


    Mesh cube = loadMesh("meshes/cube.obj", "meshes/cube.mtl");
    
    
    unsigned* pixels = (unsigned*)malloc(sizeof(unsigned) * windowHeight * windowWidth);

    Material materials[9] = {};
    float ambientIntensity = 1.0f;
    materials[0].emitColor = glm::fvec3(0.5, 0.5, 0.5) * ambientIntensity;

    materials[1].diffuseColor = {1.0f, 1.0f, 1.0f};
    materials[1].shininess = 0.7f;
    
    materials[2].diffuseColor = {1.0f, square(192.0f / 255.0f), square(203.0f / 255.0f)};

    materials[3].emitColor = glm::fvec3(1.0f, 0.0f, 0.0f) * 1.0f;

    materials[4].diffuseColor = {0.8f, 0.8f, 0.32f};
    materials[4].shininess = 0.7f;

    materials[5].diffuseColor = {0.4f, 0.8f, 0.9f};
    materials[5].shininess = 0.85f;

    materials[6].diffuseColor = {1.0f, 1.0f, 1.0f};
    materials[6].shininess = 1.0f;

    materials[7].emitColor = glm::fvec3(1.0f, 1.0f, 1.0f) * 1.0f;
    materials[7].shininess = 0.0f;

    materials[8].diffuseColor = {0.0f, 0.8f, 0.3f};
    materials[8].shininess = 1.0f;
    
    Plane planes[1] = {};
    planes[0].normal = {0, 1, 0};
    planes[0].distance = -1.0f;
    planes[0].materialIndex = 1;

    Sphere spheres[6] = {};
    spheres[0].position = {0, 0, -5};
    spheres[0].radius = 1.0f;
    spheres[0].materialIndex = 2;

    spheres[1].position = {3, -1, -10};
    spheres[1].radius = 1.0f;
    spheres[1].materialIndex = 3;

    spheres[2].position = {-2, 1, -8};
    spheres[2].radius = 1.0f;
    spheres[2].materialIndex = 4;

    spheres[3].position = {1, 1, -7};
    spheres[3].radius = 1.0f;
    spheres[3].materialIndex = 5;

    spheres[4].position = {3, 2, -9};
    spheres[4].radius = 1.0f;
    spheres[4].materialIndex = 6;

    spheres[5].position = {-1.4, 0.6, -4};
    spheres[5].radius = 0.3;
    spheres[5].materialIndex = 7;
    
    World world;
    world.materialsCount = arrayCount(materials);
    world.materials = materials;
    world.planesCount = arrayCount(planes);
    world.planes = planes;
    world.spheresCount = arrayCount(spheres);
    world.spheres = spheres;
    
    glm::fvec3 cameraPosition = {};
    //TODO(Stanisz13): why does increasing the distance zooms onto the image?
    float filmDistance = 1.0f;
    float filmWidth = 1.0;
    float filmHeight = 1.0;

    if (windowWidth > windowHeight)
    {
        filmHeight = filmWidth * ((float)windowHeight / windowWidth);
    }
    else if (windowHeight > windowWidth)
    {
        filmWidth = filmHeight * ((float)windowWidth / windowHeight);
    }

    float halfFilmWidth = 0.5 * filmWidth;
    float halfFilmHeight = 0.5 * filmHeight;

    unsigned raysPerDimension = sqrt(raysPerPixel);

    float contrib = 1.0f / raysPerPixel;
    
    float pixelWidth = filmWidth / (windowWidth);
    float pixelHeight = filmHeight / (windowHeight);

    float subPixelWidth = pixelWidth / raysPerDimension;
    float subPixelHeight = pixelHeight / raysPerDimension;

    unsigned* pixel = pixels;
    for (unsigned y = 0; y < windowHeight; ++y)
    {
        if (y % 100 == 0)
        {
            printf("Raycasting progress: %f%% \n", (float)y * 100.0f / windowHeight);
        }

        float filmY = -1.0 + 2.0 * ((float)y / windowHeight);
        
        for (unsigned x = 0; x < windowWidth; ++x)
        {
            float filmX = -1.0 + 2.0 * ((float)x / windowWidth);
            
            glm::fvec3 rayDirection = glm::normalize(glm::fvec3(filmX * halfFilmWidth,
                                                                filmY * halfFilmHeight,
                                                                -filmDistance));

            float xLowerLeftCornerOfThePixel = rayDirection.x - 0.5f * pixelWidth;
            float yLowerLeftCornerOfThePixel = rayDirection.y - 0.5f * pixelHeight;

            glm::fvec3 pixelColor = {};

            for (unsigned yRayIndex = 0; yRayIndex < raysPerDimension; ++yRayIndex)
            {
                for (unsigned xRayIndex = 0; xRayIndex < raysPerDimension; ++xRayIndex)
                {
                    
                    float xOffsetRayDirection = randomZeroToOne() * (subPixelWidth) +
                        (xRayIndex % raysPerDimension) * subPixelWidth;
                    float yOffsetRayDirection = randomZeroToOne() * (subPixelHeight) +
                        (yRayIndex % raysPerDimension) * subPixelHeight;
#if 1
                    rayDirection.x = xLowerLeftCornerOfThePixel + xOffsetRayDirection;
                    rayDirection.y = yLowerLeftCornerOfThePixel + yOffsetRayDirection;
#endif       

                    pixelColor += contrib * rayCast(&world, cameraPosition, rayDirection, raycastingDepth);
                }    
            }

#if 1
            //TODO(Stanisz13): is it a good gamma correct? 
            pixelColor.x = sqrt(pixelColor.x);
            pixelColor.y = sqrt(pixelColor.y);
            pixelColor.z = sqrt(pixelColor.z);
#endif
            *pixel = colorToUnsigned(pixelColor);
            ++pixel;
        }
    }

    writeImage(windowWidth, windowHeight, pixels, "newBeauty.bmp");


    free(pixels);

    
    return 0;
}
