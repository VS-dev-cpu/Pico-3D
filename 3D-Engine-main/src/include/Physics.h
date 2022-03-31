#pragma once

#include "include.h"

struct vec2d
{
    float u = 0.0f;
    float v = 0.0f;
    float w = 1.0f;
};

struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;
};

struct rot3d
{
    float pitch = 0;
    float yaw = 0;
    float roll = 0;

    float pitch_rad = 0;
    float yaw_rad = 0;
    float roll_rad = 0;
};

struct mat4x4
{
    float m[4][4] = {0};
};

struct color3i
{
    int r = 255;
    int g = 255;
    int b = 255;
};

struct color3f
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
};

struct triangle
{
    vec3d p[3];
    vec2d t[3];

    vec3d normal;

    int texID = -1;

    color3i color;
    color3f idensity;
};

struct mesh
{
    string name;

    vector<triangle> tris;
};

struct texture
{
    vector<color3i> pixel;
    int w, h;
};

color3f WHITEf;
color3f REDf;
color3f GREENf;
color3f BLUEf;

vec3d POS_NULL;

bool initPhysics()
{
    WHITEf.r = 1.0f;
    WHITEf.g = 1.0f;
    WHITEf.b = 1.0f;

    REDf.r = 1.0f;
    REDf.g = 0.0f;
    REDf.b = 0.0f;

    GREENf.r = 0.0f;
    GREENf.g = 1.0f;
    GREENf.b = 0.0f;

    BLUEf.r = 0.0f;
    BLUEf.g = 0.0f;
    BLUEf.b = 1.0f;
}

float cotf(float a)
{
    return cosf(a) / sinf(a);
}

float distance(vec3d a, vec3d b)
{
    vec3d tmp_a = a;
    vec3d tmp_b = b;
    if (tmp_a.x < tmp_b.x)
    {
        int tmp = tmp_a.x;
        tmp_a.x = tmp_b.x;
        tmp_b.x = tmp;
    }

    if (tmp_a.y < tmp_b.y)
    {
        int tmp = tmp_a.y;
        tmp_a.y = tmp_b.y;
        tmp_b.y = tmp;
    }

    if (tmp_a.z < tmp_b.z)
    {
        int tmp = tmp_a.z;
        tmp_a.z = tmp_b.z;
        tmp_b.z = tmp;
    }

    float c = pow(tmp_a.x - tmp_b.x, 2) + pow(tmp_a.y - tmp_b.y, 2);
    return sqrt(pow(tmp_a.z - tmp_b.z, 2) + c);
}

float map(float in, float in_min, float in_max, float out_min, float out_max)
{
    int x = in;
    if (in >= in_max)
    {
        return out_max;
    }
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float colorMap(Uint8 x)
{
    return x / 255;
}

bool between(vec3d target, vec3d a, vec3d b)
{
    if (a.x > b.x)
        swap(a.x, b.x);
    if (a.y > b.y)
        swap(a.y, b.y);
    if (a.z > b.z)
        swap(a.z, b.z);

    if (a.x < target.x && target.x < b.x && a.y < target.y && target.y < b.y && a.z < target.z && target.z < b.z)
        return true;
    return false;
}

bool collide(vec3d a, triangle b)
{
    vec3d b1 = b.p[0];
    vec3d b2 = b.p[1];
    vec3d b3 = b.p[2];
}

bool collide(triangle a, triangle b)
{
    return true;
}

bool collide(vec3d a, mesh b)
{
    return true;
}

bool collide(triangle a, mesh b)
{
    return true;
}

bool collide(mesh a, mesh b)
{
    return true;
}

string split(string s, string delimiter, int part)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res[part];
}

int isplit(string s, string delimiter, int part)
{
    return stof(split(s, delimiter, part));
}

vec3d Vector_Add(vec3d &v1, vec3d &v2)
{
    vec3d v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    v.z = v1.z + v2.z;
    return v;
}

vec3d Vector_Sub(vec3d &v1, vec3d &v2)
{
    vec3d v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    v.z = v1.z - v2.z;
    return v;
}

vec3d Vector_Mul(vec3d &v1, float k)
{
    vec3d v;
    v.x = v1.x * k;
    v.y = v1.y * k;
    v.z = v1.z * k;
    return v;
}

vec3d Vector_Div(vec3d &v1, float k)
{
    vec3d v;
    v.x = v1.x / k;
    v.y = v1.y / k;
    v.z = v1.z / k;
    return v;
}

vec3d Vector_Avarage(vec3d v1, vec3d v2)
{
    vec3d ret;
    ret.x = (v1.x + v2.x) / 2;
    ret.y = (v1.y + v2.y) / 2;
    ret.z = (v1.z + v2.z) / 2;
    return ret;
}

vec3d Vector_Avarage(vec3d v1, vec3d v2, vec3d v3)
{
    vec3d ret;
    ret.x = (v1.x + v2.x + v3.x) / 3;
    ret.y = (v1.y + v2.y + v3.y) / 3;
    ret.z = (v1.z + v2.z + v3.z) / 3;
    return ret;
}

float Vector_DotProduct(vec3d &v1, vec3d &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector_Length(vec3d &v)
{
    return sqrtf(Vector_DotProduct(v, v));
}

vec3d Vector_Normalise(vec3d &v)
{
    float l = Vector_Length(v);
    vec3d vv;
    vv.x = v.x / l;
    vv.y = v.y / l;
    vv.z = v.z / l;
    return vv;
}

vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2)
{
    vec3d v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd, float &t)
{
    plane_n = Vector_Normalise(plane_n);
    float plane_d = -Vector_DotProduct(plane_n, plane_p);
    float ad = Vector_DotProduct(lineStart, plane_n);
    float bd = Vector_DotProduct(lineEnd, plane_n);
    t = (-plane_d - ad) / (bd - ad);
    vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
    vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
    return Vector_Add(lineStart, lineToIntersect);
}

vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i)
{
    vec3d v;
    v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
    v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
    v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
    v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
    return v;
}

mat4x4 Matrix_MakeIdentity()
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeRotationX(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeRotationY(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeRotationZ(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_MakeTranslation(float x, float y, float z)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    mat4x4 matrix;
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / (fFar - fNear);
    matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2)
{
    mat4x4 matrix;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
    return matrix;
}

mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up)
{
    vec3d newForward = Vector_Sub(target, pos);
    newForward = Vector_Normalise(newForward);

    vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
    vec3d newUp = Vector_Sub(up, a);
    newUp = Vector_Normalise(newUp);

    vec3d newRight = Vector_CrossProduct(newUp, newForward);

    mat4x4 matrix;
    matrix.m[0][0] = newRight.x;
    matrix.m[0][1] = newRight.y;
    matrix.m[0][2] = newRight.z;
    matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = newUp.x;
    matrix.m[1][1] = newUp.y;
    matrix.m[1][2] = newUp.z;
    matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = newForward.x;
    matrix.m[2][1] = newForward.y;
    matrix.m[2][2] = newForward.z;
    matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = pos.x;
    matrix.m[3][1] = pos.y;
    matrix.m[3][2] = pos.z;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 Matrix_QuickInverse(mat4x4 &m) // Only for Rotation/Translation Matrices
{
    mat4x4 matrix;
    matrix.m[0][0] = m.m[0][0];
    matrix.m[0][1] = m.m[1][0];
    matrix.m[0][2] = m.m[2][0];
    matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m.m[0][1];
    matrix.m[1][1] = m.m[1][1];
    matrix.m[1][2] = m.m[2][1];
    matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m.m[0][2];
    matrix.m[2][1] = m.m[1][2];
    matrix.m[2][2] = m.m[2][2];
    matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

vec3d Triangle_CalculateNormal(triangle t)
{
    // Calculate triangle normal
    vec3d normal, line1, line2;

    line1 = Vector_Sub(t.p[1], t.p[0]);
    line2 = Vector_Sub(t.p[2], t.p[0]);

    normal = Vector_CrossProduct(line1, line2);
    normal = Vector_Normalise(normal);
    return normal;
}

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
{
    plane_n = Vector_Normalise(plane_n);

    auto dist = [&](vec3d &p)
    {
        // vec3d n = Vector_Normalise(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
    };

    vec3d *inside_points[3];
    int nInsidePointCount = 0;
    vec3d *outside_points[3];
    int nOutsidePointCount = 0;

    vec2d *inside_tex[3];
    int nInsideTexCount = 0;
    vec2d *outside_tex[3];
    int nOutsideTexCount = 0;

    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0)
    {
        inside_points[nInsidePointCount++] = &in_tri.p[0];
        inside_tex[nInsideTexCount++] = &in_tri.t[0];
    }
    else
    {
        outside_points[nOutsidePointCount++] = &in_tri.p[0];
        outside_tex[nOutsideTexCount++] = &in_tri.t[0];
    }
    if (d1 >= 0)
    {
        inside_points[nInsidePointCount++] = &in_tri.p[1];
        inside_tex[nInsideTexCount++] = &in_tri.t[1];
    }
    else
    {
        outside_points[nOutsidePointCount++] = &in_tri.p[1];
        outside_tex[nOutsideTexCount++] = &in_tri.t[1];
    }
    if (d2 >= 0)
    {
        inside_points[nInsidePointCount++] = &in_tri.p[2];
        inside_tex[nInsideTexCount++] = &in_tri.t[2];
    }
    else
    {
        outside_points[nOutsidePointCount++] = &in_tri.p[2];
        outside_tex[nOutsideTexCount++] = &in_tri.t[2];
    }

    if (nInsidePointCount == 0)
    {
        return 0;
    }

    if (nInsidePointCount == 3)
    {
        out_tri1 = in_tri;

        return 1;
    }

    if (nInsidePointCount == 1 && nOutsidePointCount == 2)
    {
        out_tri1.color = in_tri.color;
        out_tri1.idensity = in_tri.idensity;
        out_tri1.texID = in_tri.texID;

        out_tri1.p[0] = *inside_points[0];
        out_tri1.t[0] = *inside_tex[0];

        float t;
        out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
        out_tri1.t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
        out_tri1.t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
        out_tri1.t[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

        out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
        out_tri1.t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
        out_tri1.t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
        out_tri1.t[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

        return 1;
    }

    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
        out_tri1.color = in_tri.color;
        out_tri2.color = in_tri.color;

        out_tri1.idensity = in_tri.idensity;
        out_tri2.idensity = in_tri.idensity;

        out_tri1.texID = in_tri.texID;
        out_tri2.texID = in_tri.texID;

        out_tri1.p[0] = *inside_points[0];
        out_tri1.p[1] = *inside_points[1];
        out_tri1.t[0] = *inside_tex[0];
        out_tri1.t[1] = *inside_tex[1];

        float t;
        out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
        out_tri1.t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
        out_tri1.t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
        out_tri1.t[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

        out_tri2.p[0] = *inside_points[1];
        out_tri2.t[0] = *inside_tex[1];
        out_tri2.p[1] = out_tri1.p[2];
        out_tri2.t[1] = out_tri1.t[2];
        out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
        out_tri2.t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
        out_tri2.t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
        out_tri2.t[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;

        return 2;
    }
    return 0;
}

float Vector_Angle(vec3d a, vec3d lookdir, vec3d b) // base: a, object: b
{
    cout << Vector_DotProduct(a, b) << endl;

    // if (lookdir.x > b.)
}