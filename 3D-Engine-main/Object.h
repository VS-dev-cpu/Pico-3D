#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "EngineMath.h"

typedef struct object
{
    mesh meshData;
    mesh translated;
    vec3d position;
    rot3d rotation;
} object;

void Object_CalculateNormals(object *obj)
{
    // Calculate Normals
    for (int i = 0; i < obj->meshData.tris.size(); i++)
    {
        obj->meshData.tris[i].normal = Triangle_CalculateNormal(obj->meshData.tris[i]);
    }
}

void Object_Update(object *obj, bool bRecalculateNormals)
{
    obj->translated = obj->meshData;

    mat4x4 matRotX = Matrix_MakeRotationX(obj->rotation.pitch);
    mat4x4 matRotY = Matrix_MakeRotationY(obj->rotation.yaw);
    mat4x4 matRotZ = Matrix_MakeRotationZ(obj->rotation.roll);
    mat4x4 rotation = Matrix_MakeIdentity();
    rotation = Matrix_MultiplyMatrix(rotation, matRotX);
    rotation = Matrix_MultiplyMatrix(rotation, matRotY);
    rotation = Matrix_MultiplyMatrix(rotation, matRotZ);

    for (int t = 0; t < obj->translated.tris.size(); t++)
    {
        for (int i = 0; i < 3; i++)
        {
            obj->translated.tris[t].p[i] = Matrix_MultiplyVector(matRotX, obj->translated.tris[t].p[i]);
            obj->translated.tris[t].p[i] = Matrix_MultiplyVector(matRotY, obj->translated.tris[t].p[i]);
            obj->translated.tris[t].p[i] = Matrix_MultiplyVector(matRotZ, obj->translated.tris[t].p[i]);

            obj->translated.tris[t].p[i].x += obj->position.x;
            obj->translated.tris[t].p[i].y += obj->position.y;
            obj->translated.tris[t].p[i].z += obj->position.z;
        }
    }

    if (bRecalculateNormals)
        calculateNormals(obj);
}

#endif