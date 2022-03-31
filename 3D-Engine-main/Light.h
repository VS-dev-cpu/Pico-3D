#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "EngineMath.h"

enum lightType
{
    point,
    direction,
    spot
};

typedef struct light
{
    lightType type = point;

    color3f color;
    float strength = 0.5f;
    float distance = 50.0f;

    vec3d position, direction, spread;
} light;

void Light_Setup(light *lit, lightType type, vec3d pos)
{
    lit->type = type;
    lit->position = pos;
    lit->spread.x = 5;
    lit->spread.y = 5;
    lit->spread.z = 5;
}

void Light_Setup(light *lit, lightType type, float x, float y, float z)
{
    lit->type = type;
    lit->position.x = x;
    lit->position.y = y;
    lit->position.z = z;
    lit->spread.x = 5;
    lit->spread.y = 5;
    lit->spread.z = 5;
}

#endif