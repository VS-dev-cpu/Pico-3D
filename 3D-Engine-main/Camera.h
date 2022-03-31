#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "EngineMath.h"

typedef struct camera
{
    vec3d position;
    rot3d rotation;

    vec3d vLookDir;
    vec3d vUp;
    mat4x4 matView;
} camera;

void Camera_Update(camera *cam)
{
    vec3d vUp;
    vUp.y = -1;

    vec3d vTarget;
    vTarget.z = 1;
    mat4x4 matCameraRotX = Matrix_MakeRotationX(cam->rotation.pitch);
    mat4x4 matCameraRotY = Matrix_MakeRotationY(cam->rotation.yaw);
    mat4x4 matCameraRotZ = Matrix_MakeRotationZ(cam->rotation.roll);
    cam->vLookDir = Matrix_MultiplyVector(matCameraRotY, vTarget);
    vTarget = Vector_Add(cam->position, cam->vLookDir);

    mat4x4 matCamera = Matrix_PointAt(cam->position, vTarget, vUp);
    cam->matView = Matrix_QuickInverse(matCamera);
    cam->matView = Matrix_MultiplyMatrix(cam->matView, matCameraRotX);
    cam->matView = Matrix_MultiplyMatrix(cam->matView, matCameraRotZ);
}

void Camera_Move(camera *cam, float x, float y, float z)
{
    Camera_MoveX(cam, x);
    Camera_MoveY(cam, y);
    Camera_MoveZ(cam, z);
}

void Camera_Move(camera *cam, vec3d pos)
{
    Camera_Move(cam, pos.x, pos.y, pos.z);
}

void Camera_MoveX(camera *cam, float x) // Right
{
    vec3d vUp;
    vUp.y = -1;
    vec3d vForward = Vector_Mul(cam->vLookDir, x);
    vec3d vRight = Vector_CrossProduct(vForward, vUp);
    cam->position = Vector_Sub(cam->position, vRight);
}

void Camera_MoveY(camera *cam, float y) // Up
{
    cam->position.y += y;
}

void Camera_MoveZ(camera *cam, float z) // Forward
{
    vec3d vForward = Vector_Mul(cam->vLookDir, z);
    cam->position = Vector_Add(cam->position, vForward);
}

#endif