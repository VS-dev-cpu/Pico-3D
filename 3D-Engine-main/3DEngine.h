#ifndef __3DENGINE_H__
#define __3DENGINE_H__

#include "st7789.h"
#include "graphics.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "EngineMath.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"

#define RAD 0.0174532925

typedef struct Engine3D
{
    uint16_t *pixels;
    int w, h;

    vector<object> objectList;
    vector<texture> textureList;
    vector<light> lightList;

    mat4x4 matProj;

    camera cam;

    float fNearCutoff = 1.0f;

    float fTheta = 0.0f;
    float fDeltaTime = 1.0f;

#ifdef USE_TEXTURE
    float *pDepthBuffer = nullptr;
    int prevDisplayRes_w;
    int prevDisplayRes_h;
#endif

} Engine3D;

bool LoadFromObjectFile(int n, string sFileName)
{
    ifstream f(sFileName);
    if (!f.is_open())
        return false;

    // Local cache of verts
    vector<vec3d> verts;
    vector<vec2d> texs;

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        strstream s;
        s << line;

        char junk;

        if (line[0] == 'v')
        {
            if (line[1] == ' ')
            {
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
            else if (line[1] == 't')
            {
                vec2d v;
                s >> junk >> junk >> v.u >> v.v;
                texs.push_back(v);
            }
            else if (line[1] == 'n')
            {
                // DOSOMETHING
            }
        }
        if (line[0] == 'f')
        {
            string str;
            str = s.str();
            vector<string> parts;
            parts.push_back(split(str, " ", 1));
            parts.push_back(split(str, " ", 2));
            parts.push_back(split(str, " ", 3));

            triangle t;
            t.p[0] = verts[isplit(parts[0], "/", 0) - 1];
            t.p[1] = verts[isplit(parts[1], "/", 0) - 1];
            t.p[2] = verts[isplit(parts[2], "/", 0) - 1];
            objectList[n].meshData.tris.push_back(t);
        }
    }

    // Calculate Normals
    objectList[n].calculateNormals();

    return true;
}

#ifdef USE_TEXTURE
bool LoadFromObjectFile(int n, string sFileName, texture tex)
{
    ifstream f(sFileName);
    if (!f.is_open())
        return false;

    // Local cache of verts
    vector<vec3d> verts;
    vector<vec2d> texs;

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        strstream ss;
        ss << line;

        char junk;

        if (line[0] == 'v')
        {
            if (line[1] == 't')
            {
                vec2d v;
                ss >> junk >> junk >> v.u >> v.v;
                texs.push_back(v);
            }
            else if (line[1] == ' ')
            {
                vec3d v;
                ss >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
        }
        if (line[0] == 'f')
        {
            string str;
            str = line;
            vector<string> parts;
            parts.push_back(split(str, " ", 1));
            parts.push_back(split(str, " ", 2));
            parts.push_back(split(str, " ", 3));

            triangle t;
            t.p[0] = verts[isplit(parts[0], "/", 0) - 1];
            t.p[1] = verts[isplit(parts[1], "/", 0) - 1];
            t.p[2] = verts[isplit(parts[2], "/", 0) - 1];

            t.t[0] = texs[isplit(parts[0], "/", 1) - 1];
            t.t[1] = texs[isplit(parts[1], "/", 1) - 1];
            t.t[2] = texs[isplit(parts[2], "/", 1) - 1];

            t.texID = textureList.size();

            objectList[n].meshData.tris.push_back(t);
        }
    }
    textureList.push_back(tex);

    // Calculate Normals
    objectList[n].calculateNormals();

    return true;
}

void textured_triangle(triangle tri)
{
    if (tri.p[1].y < tri.p[0].y)
    {
        swap(tri.p[0].y, tri.p[1].y);
        swap(tri.p[0].x, tri.p[1].x);
        swap(tri.t[0].u, tri.t[1].u);
        swap(tri.t[0].v, tri.t[1].v);
        swap(tri.t[0].w, tri.t[1].w);
    }

    if (tri.p[2].y < tri.p[0].y)
    {
        swap(tri.p[0].y, tri.p[2].y);
        swap(tri.p[0].x, tri.p[2].x);
        swap(tri.t[0].u, tri.t[2].u);
        swap(tri.t[0].v, tri.t[2].v);
        swap(tri.t[0].w, tri.t[2].w);
    }

    if (tri.p[2].y < tri.p[1].y)
    {
        swap(tri.p[1].y, tri.p[2].y);
        swap(tri.p[1].x, tri.p[2].x);
        swap(tri.t[1].u, tri.t[2].u);
        swap(tri.t[1].v, tri.t[2].v);
        swap(tri.t[1].w, tri.t[2].w);
    }

    int dy1 = tri.p[1].y - tri.p[0].y;
    int dx1 = tri.p[1].x - tri.p[0].x;
    float dv1 = tri.t[1].v - tri.t[0].v;
    float du1 = tri.t[1].u - tri.t[0].u;
    float dw1 = tri.t[1].w - tri.t[0].w;

    int dy2 = tri.p[2].y - tri.p[0].y;
    int dx2 = tri.p[2].x - tri.p[0].x;
    float dv2 = tri.t[2].v - tri.t[0].v;
    float du2 = tri.t[2].u - tri.t[0].u;
    float dw2 = tri.t[2].w - tri.t[0].w;

    float tex_u, tex_v, tex_w;

    float dax_step = 0, dbx_step = 0,
          du1_step = 0, dv1_step = 0,
          du2_step = 0, dv2_step = 0,
          dw1_step = 0, dw2_step = 0;

    if (dy1)
        dax_step = dx1 / (float)abs(dy1);
    if (dy2)
        dbx_step = dx2 / (float)abs(dy2);

    if (dy1)
        du1_step = du1 / (float)abs(dy1);
    if (dy1)
        dv1_step = dv1 / (float)abs(dy1);
    if (dy1)
        dw1_step = dw1 / (float)abs(dy1);

    if (dy2)
        du2_step = du2 / (float)abs(dy2);
    if (dy2)
        dv2_step = dv2 / (float)abs(dy2);
    if (dy2)
        dw2_step = dw2 / (float)abs(dy2);

    if (dy1)
    {
        for (int i = tri.p[0].y; i <= tri.p[1].y; i++)
        {
            int ax = tri.p[0].x + (float)(i - tri.p[0].y) * dax_step;
            int bx = tri.p[0].x + (float)(i - tri.p[0].y) * dbx_step;

            float tex_su = tri.t[0].u + (float)(i - tri.p[0].y) * du1_step;
            float tex_sv = tri.t[0].v + (float)(i - tri.p[0].y) * dv1_step;
            float tex_sw = tri.t[0].w + (float)(i - tri.p[0].y) * dw1_step;

            float tex_eu = tri.t[0].u + (float)(i - tri.p[0].y) * du2_step;
            float tex_ev = tri.t[0].v + (float)(i - tri.p[0].y) * dv2_step;
            float tex_ew = tri.t[0].w + (float)(i - tri.p[0].y) * dw2_step;

            if (ax > bx)
            {
                swap(ax, bx);
                swap(tex_su, tex_eu);
                swap(tex_sv, tex_ev);
                swap(tex_sw, tex_ew);
            }

            tex_u = tex_su;
            tex_v = tex_sv;
            tex_w = tex_sw;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                if (tex_w > pDepthBuffer[i * display_w + j])
                {
                    if (tri.texID != -1)
                    {
                        color3i rgb = textureList[tri.texID].pixel[tex_u / tex_w + tex_v / tex_w * (textureList[tri.texID].w)];

                        display.pixel[j + i * (display_w - 1)].r = rgb.r * tri.idensity.r;
                        display.pixel[j + i * (display_w - 1)].g = rgb.g * tri.idensity.g;
                        display.pixel[j + i * (display_w - 1)].b = rgb.b * tri.idensity.b;
                    }
                    else
                    {
                        display.pixel[j + i * (display_w - 1)].r = tri.color.r * tri.idensity.r;
                        display.pixel[j + i * (display_w - 1)].g = tri.color.r * tri.idensity.g;
                        display.pixel[j + i * (display_w - 1)].b = tri.color.r * tri.idensity.b;
                    }
                    pDepthBuffer[i * (display_w) + j] = tex_w;
                }
                t += tstep;
            }
        }
    }

    dy1 = tri.p[2].y - tri.p[1].y;
    dx1 = tri.p[2].x - tri.p[1].x;
    dv1 = tri.t[2].v - tri.t[1].v;
    du1 = tri.t[2].u - tri.t[1].u;
    dw1 = tri.t[2].w - tri.t[1].w;

    if (dy1)
        dax_step = dx1 / (float)abs(dy1);
    if (dy2)
        dbx_step = dx2 / (float)abs(dy2);

    du1_step = 0, dv1_step = 0;
    if (dy1)
        du1_step = du1 / (float)abs(dy1);
    if (dy1)
        dv1_step = dv1 / (float)abs(dy1);
    if (dy1)
        dw1_step = dw1 / (float)abs(dy1);

    if (dy1)
    {
        for (int i = tri.p[1].y; i <= tri.p[2].y; i++)
        {
            int ax = tri.p[1].x + (float)(i - tri.p[1].y) * dax_step;
            int bx = tri.p[0].x + (float)(i - tri.p[0].y) * dbx_step;

            float tex_su = tri.t[1].u + (float)(i - tri.p[1].y) * du1_step;
            float tex_sv = tri.t[1].v + (float)(i - tri.p[1].y) * dv1_step;
            float tex_sw = tri.t[1].w + (float)(i - tri.p[1].y) * dw1_step;

            float tex_eu = tri.t[0].u + (float)(i - tri.p[0].y) * du2_step;
            float tex_ev = tri.t[0].v + (float)(i - tri.p[0].y) * dv2_step;
            float tex_ew = tri.t[0].w + (float)(i - tri.p[0].y) * dw2_step;

            if (ax > bx)
            {
                swap(ax, bx);
                swap(tex_su, tex_eu);
                swap(tex_sv, tex_ev);
                swap(tex_sw, tex_ew);
            }

            tex_u = tex_su;
            tex_v = tex_sv;
            tex_w = tex_sw;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                tex_w = (1.0f - t) * tex_sw + t * tex_ew;

                if (tex_w > pDepthBuffer[i * display_w + j])
                {
                    if (tri.texID != -1)
                    {
                        color3i rgb = textureList[tri.texID].pixel[tex_u / tex_w + tex_v / tex_w * (textureList[tri.texID].w)];

                        display.pixel[j + i * (display_w - 1)].r = rgb.r * tri.idensity.r;
                        display.pixel[j + i * (display_w - 1)].g = rgb.g * tri.idensity.g;
                        display.pixel[j + i * (display_w - 1)].b = rgb.b * tri.idensity.b;
                    }
                    else
                    {
                        display.pixel[j + i * (display_w - 1)].r = tri.color.r * tri.idensity.r;
                        display.pixel[j + i * (display_w - 1)].g = tri.color.r * tri.idensity.g;
                        display.pixel[j + i * (display_w - 1)].b = tri.color.r * tri.idensity.b;
                    }
                    pDepthBuffer[i * (display_w) + j] = tex_w;
                }
                t += tstep;
            }
        }
    }
}
#endif

Engine3D::Engine3D(const char *name, int w_width, int w_heigth)
{
    engine2d.setup(name, w_width, w_heigth, SDL_WINDOW_SHOWN);
    display_w = w_width;
    display_h = w_heigth;
#ifdef USE_TEXTURE
    pDepthBuffer = new float[display_w * display_h];
#endif

    // Projection Matrix
    matProj = Matrix_MakeProjection(90.0f, (float)display_h / (float)display_w, 0.1f, 1000.0f);
}

bool Engine3D::update(float fElapsedTime)
{
    fDeltaTime = fElapsedTime;

    engine2d.color(0, 0, 0);
    engine2d.fill(0, 0, display_w, display_h);

#ifdef USE_TEXTURE
    if (prevDisplayRes_w != display_w || prevDisplayRes_h != display_h)
    {
        if (display_w > 0 && display_h > 0)
        {
            prevDisplayRes_w = display_w;
            prevDisplayRes_h = display_h;
            if (pDepthBuffer != NULL && pDepthBuffer != nullptr)
                free(pDepthBuffer);
            pDepthBuffer = new float[display_w * display_h];
        }
    }

    for (int i = 0; i < display_w * display_h; i++)
        pDepthBuffer[i] = 0.0f;
#endif

    mat4x4 matRotZ, matRotX;

    // Rotation
    matRotX = Matrix_MakeRotationX(fTheta);
    matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);

    mat4x4 matTrans;
    matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 0.0f);

    mat4x4 matWorld;
    matWorld = Matrix_MakeIdentity();
    matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX);
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

    cam.update();

    vector<triangle> vecTrianglesToRaster;

    // Draw Triangles
    for (auto obj : objectList)
    {
        mesh m = obj.translated;
        for (auto tri : m.tris)
        {
            triangle triProjected, triTransformed, triViewed;

            triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
            triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
            triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
            triTransformed.color = tri.color;
            triTransformed.idensity = tri.idensity;
            triTransformed.texID = tri.texID;
            triTransformed.t[0] = tri.t[0];
            triTransformed.t[1] = tri.t[1];
            triTransformed.t[2] = tri.t[2];

            vec3d vCameraRay = Vector_Sub(triTransformed.p[0], cam.position);

            if (Vector_DotProduct(tri.normal, vCameraRay) < 0.0f)
            {
                // Illumination
                color3f dp;
                dp.r = 0;
                dp.g = 0;
                dp.b = 0;
                for (auto lit : lightList)
                {
                    vec3d triA = Vector_Avarage(tri.p[0], tri.p[1], tri.p[2]);
                    switch (lit.type)
                    {
                    case point:
                        dp.r += lit.strength * map(distance(triA, lit.position), 0.0f, lit.distance, 1.0f, 0.0f);
                        dp.g += lit.strength * map(distance(triA, lit.position), 0.0f, lit.distance, 1.0f, 0.0f);
                        dp.b += lit.strength * map(distance(triA, lit.position), 0.0f, lit.distance, 1.0f, 0.0f);

                        dp.r *= lit.color.r;
                        dp.g *= lit.color.g;
                        dp.b *= lit.color.b;
                        break;

                    case direction:
                        dp.r += max(0.1f, Vector_DotProduct(lit.direction, tri.normal)) * lit.strength;
                        dp.g += max(0.1f, Vector_DotProduct(lit.direction, tri.normal)) * lit.strength;
                        dp.b += max(0.1f, Vector_DotProduct(lit.direction, tri.normal)) * lit.strength;

                        dp.r *= lit.color.r;
                        dp.g *= lit.color.g;

                    case spot:
                        for (int i = 0; i < 3; i++)
                        {
                            if (lit.direction.x > 0 && tri.p[i].x < lit.position.x - lit.spread_x)
                                break;
                            if (lit.direction.x < 0 && tri.p[i].x > lit.position.x + lit.spread_x)
                                break;

                            if (lit.direction.y > 0 && tri.p[i].y < lit.position.y - lit.spread_y)
                                break;
                            if (lit.direction.y < 0 && tri.p[i].y > lit.position.y + lit.spread_y)
                                break;

                            if (lit.direction.z > 0 && tri.p[i].z < lit.position.z - lit.spread_z)
                                break;
                            if (lit.direction.z < 0 && tri.p[i].z > lit.position.z + lit.spread_z)
                                break;
                        }

                        dp.r += lit.strength * map(distance(triA, lit.position), 0.0f, lit.distance, 1.0f, 0.0f);
                        dp.g += lit.strength * map(distance(triA, lit.position), 0.0f, lit.distance, 1.0f, 0.0f);
                        dp.b += lit.strength * map(distance(triA, lit.position), 0.0f, lit.distance, 1.0f, 0.0f);

                        dp.r *= lit.color.r;
                        dp.g *= lit.color.g;
                        dp.b *= lit.color.b;
                        break;
                    }
                }

                /*triTransformed.color.r *= dp;
            triTransformed.color.g *= dp;
            triTransformed.color.b *= dp;*/

                triTransformed.idensity.r = dp.r;
                triTransformed.idensity.g = dp.g;
                triTransformed.idensity.b = dp.b;

                // Convert World Space --> View Space
                triViewed.p[0] = Matrix_MultiplyVector(cam.matView, triTransformed.p[0]);
                triViewed.p[1] = Matrix_MultiplyVector(cam.matView, triTransformed.p[1]);
                triViewed.p[2] = Matrix_MultiplyVector(cam.matView, triTransformed.p[2]);
                triViewed.color = triTransformed.color;
                triViewed.idensity = triTransformed.idensity;
                triViewed.texID = triTransformed.texID;
                triViewed.t[0] = triTransformed.t[0];
                triViewed.t[1] = triTransformed.t[1];
                triViewed.t[2] = triTransformed.t[2];

                int nClippedTriangles = 0;
                triangle clipped[2];
                vec3d nearBorder;
                nearBorder.z = fNearCutoff;
                vec3d farBorder;
                farBorder.z = 1.0f;

                nClippedTriangles = Triangle_ClipAgainstPlane(nearBorder, farBorder, triViewed, clipped[0], clipped[1]);

                for (int n = 0; n < nClippedTriangles; n++)
                {
                    // Project triangles from 3D --> 2D
                    triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
                    triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
                    triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);
                    triProjected.color = clipped[n].color;
                    triProjected.idensity = clipped[n].idensity;
                    triProjected.texID = clipped[n].texID;
                    triProjected.t[0] = clipped[n].t[0];
                    triProjected.t[1] = clipped[n].t[1];
                    triProjected.t[2] = clipped[n].t[2];

                    triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
                    triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
                    triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

                    triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
                    triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
                    triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

                    triProjected.t[0].w = 1.0f / triProjected.p[0].w;
                    triProjected.t[1].w = 1.0f / triProjected.p[1].w;
                    triProjected.t[2].w = 1.0f / triProjected.p[2].w;

                    triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
                    triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
                    triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

                    // Scale into view
                    vec3d vOffsetView;
                    vOffsetView.x = 1.0f;
                    vOffsetView.y = 1.0f;
                    vOffsetView.z = 0.0f;

                    triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
                    triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
                    triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);

                    triProjected.p[0].x *= 0.5 * (float)display_w;
                    triProjected.p[0].y *= 0.5 * (float)display_h;
                    triProjected.p[1].x *= 0.5 * (float)display_w;
                    triProjected.p[1].y *= 0.5 * (float)display_h;
                    triProjected.p[2].x *= 0.5 * (float)display_w;
                    triProjected.p[2].y *= 0.5 * (float)display_h;

                    vecTrianglesToRaster.push_back(triProjected);
                }
            }
        }
    }

    // Sort triangles from back to front
    sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2)
         {
             float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
             float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
             return z1 > z2;
         });

    for (auto &triToRaster : vecTrianglesToRaster)
    {
        triangle clipped[2];
        list<triangle> listTriangles;
        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++)
        {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                vec3d plane_n;
                vec3d plane_p;
                plane_n.w = 1.0f;
                plane_p.w = 1.0f;

                switch (p)
                {
                case 0:
                    plane_n.x = 0.0f;
                    plane_n.y = 0.0f;
                    plane_n.z = 0.0f;

                    plane_p.x = 0.0f;
                    plane_p.y = 1.0f;
                    plane_p.z = 0.0f;

                    nTrisToAdd = Triangle_ClipAgainstPlane(plane_n, plane_p, test, clipped[0], clipped[1]);
                    break;

                case 1:
                    plane_n.x = 0.0f;
                    plane_n.y = (float)display_h - 1.0f;
                    plane_n.z = 0.0f;

                    plane_p.x = 0.0f;
                    plane_p.y = -1.0f;
                    plane_p.z = 0.0f;

                    nTrisToAdd = Triangle_ClipAgainstPlane(plane_n, plane_p, test, clipped[0], clipped[1]);
                    break;

                case 2:
                    plane_n.x = 0.0f;
                    plane_n.y = 0.0f;
                    plane_n.z = 0.0f;

                    plane_p.x = 1.0f;
                    plane_p.y = 0.0f;
                    plane_p.z = 0.0f;

                    nTrisToAdd = Triangle_ClipAgainstPlane(plane_n, plane_p, test, clipped[0], clipped[1]);
                    break;

                case 3:
                    plane_n.x = (float)display_w - 1.0f;
                    plane_n.y = 0.0f;
                    plane_n.z = 0.0f;

                    plane_p.x = -1.0f;
                    plane_p.y = 0.0f;
                    plane_p.z = 0.0f;

                    nTrisToAdd = Triangle_ClipAgainstPlane(plane_n, plane_p, test, clipped[0], clipped[1]);
                    break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }

        for (auto &t : listTriangles)
        {
            // textured_triangle(t);
            engine2d.fill_triangle(t, t.color.r * t.idensity.r, t.color.g * t.idensity.g, t.color.b * t.idensity.b);
        }
    }
    return engine2d.update();
}

camera *Engine3D::get_camera()
{
    return &cam;
}

void Engine3D::add(object obj)
{
    obj.calculateNormals();
    objectList.push_back(obj);
}
void Engine3D::add(light lit)
{
    lightList.push_back(lit);
}

void Engine3D::addObject(string name, mesh msh, float x, float y, float z, float pitch, float yaw, float roll)
{
    object obj;
    obj.name = name;
    obj.meshData = msh;
    obj.position.x = x;
    obj.position.y = y;
    obj.position.z = z;
    obj.rotation.pitch = pitch;
    obj.rotation.yaw = yaw;
    obj.rotation.roll = roll;
    obj.calculateNormals();
    objectList.push_back(obj);

    objectList[objectList.size() - 1].update(true);
}
void Engine3D::loadObject(string name, string path)
{
    object obj;
    obj.name = name;
    obj.position.x = 0;
    obj.position.y = 0;
    obj.position.z = 0;

    obj.rotation.pitch = 0;
    obj.rotation.yaw = 0;
    obj.rotation.roll = 0;

    objectList.push_back(obj);
    LoadFromObjectFile(objectList.size() - 1, path);
    objectList[objectList.size() - 1].update(true);
}
#ifdef USE_TEXTURES
void Engine3D::loadObject(string name, string path, texture tex)
{
    object obj;
    obj.name = name;
    obj.position.x = 0;
    obj.position.y = 0;
    obj.position.z = 0;

    obj.rotation.pitch = 0;
    obj.rotation.yaw = 0;
    obj.rotation.roll = 0;

    objectList.push_back(obj);
    LoadFromObjectFile(objectList.size() - 1, path, tex);
    objectList[objectList.size() - 1].update(true);
}
#endif

void Light_Add(Engine3D *engine, lightType type, color3f color, float strength, float distance, vec3d position, vec3d direction)
{
    light lit;
    lit.type = type;
    lit.color = color;
    lit.strength = strength;
    lit.distance = distance;
    lit.position = position;
    lit.direction = direction;
    engine->lights.push_back(lit);
}
void Engine3D::addLight(string name, lightType type, color3f color, float strength, float distance, float spread_x, float spread_y, float spread_z, vec3d position, vec3d direction)
{
    light lit;
    lit.name = name;
    lit.type = type;
    lit.color = color;
    lit.strength = strength;
    lit.distance = distance;
    lit.spread_x = spread_x;
    lit.spread_y = spread_y;
    lit.spread_z = spread_z;
    lit.position = position;
    lit.direction = direction;
    lightList.push_back(lit);
}

#endif