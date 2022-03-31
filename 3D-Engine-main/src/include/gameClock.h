#pragma once

#include "include.h"

struct gameClock
{
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;

    float start = 0;
    float current = 0;

    void update()
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = ((float)((NOW - LAST) * 1000 / (float)SDL_GetPerformanceFrequency())) / 1000;

        current = SDL_GetTicks() - start;
    }

    void resetCounter()
    {
        start = SDL_GetTicks();
    }
};