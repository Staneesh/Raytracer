#include "model.h"
#include <stdlib.h>
#include <stdio.h>

//NOTE(Stanisz13): this is passing the pointer by reference to get it changed.
// without it, the pointer would be passed by value and thus the argument will be
// only changed locally.
//Success iff 1, failure iff 0.
unsigned readEntireFile(char* &buffer, const char* path)
{

    FILE* file = fopen(path, "rb");

    if (file == 0)
    {
        printf("Error opening file: %s.\n", path);
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    unsigned long long size = ftell(file);
    buffer = (char*)malloc(sizeof(char) * size + 1);

    rewind(file);
    unsigned bytesRead = fread(buffer, sizeof(char), size, file);
    
    if (sizeof(char) * size != bytesRead)
    {
        printf("Error reading file: %s.\n", path);
        fclose(file);
        return 0;
    }

    fclose(file);
    buffer[size] = 0;

    return 1;
}

Mesh loadMesh(const char* objPath, const char* mtlPath)
{
    Mesh result = {};

    char* modelBuffer;
    char* mtlBuffer;
    
    //TODO(Stanisz13): do i have to go through the file 2 times? (reading + processing)
    unsigned modelReadRes = readEntireFile(modelBuffer, objPath);
    unsigned mtlReadRes = readEntireFile(mtlBuffer, mtlPath);

    //NOTE(Stanisz13): Failure reading either of the files.
    if (modelReadRes == 0 || mtlReadRes == 0)
    {
        return result;
    }

    printf("BUFFER:\n %s\n\n", modelBuffer);
    
    
    
    
    free(modelBuffer);
    free(mtlBuffer);

    return result;
}

void loadMesh(const char* objPath, const char* mtlPath, Mesh* intoHere)
{

}
