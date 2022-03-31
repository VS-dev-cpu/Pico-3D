#pragma once

#include "Physics.h"
#include "gameClock.h"
#include "Mixer.h"
#include "include.h"

using namespace std;

class Engine2D
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;

    void setup(const char *name, int w, int h, Uint32 flags);

    int width, heigth;
    const Uint8 *key;
    int cursor_x, cursor_y;
    bool mousebutton_r, mousebutton_l;
    int scroll_wheel_x, scroll_wheel_y;

    char text[32];
    int textlen = 0;
    SDL_Keycode keyboard_sym;

    void cursor(int en);

    void color(Uint8 r, Uint8 g, Uint8 b);

    void draw_pixel(int x, int y);
    void draw_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b);
    void draw_line(int x1, int y1, int x2, int y2);
    void draw_line(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

    void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b);
    void draw_triangle(triangle t, Uint8 r, Uint8 g, Uint8 b);

    void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b);
    void fill_triangle(triangle t, Uint8 r, Uint8 g, Uint8 b);

    void draw_texture(texture tex, int x1, int y1, int x2, int y2);
    void draw_texture_wh(texture tex, int x, int y, int w, int h);
    void draw_texture(texture tex, int x, int y);

    void draw_text(string text, int size, bool outline, int x, int y);

    void fill(int x, int y, int w, int h);
    bool update();

    texture load_texture(const char *path);
    color3i get_pixel(texture tex, int x, int y);

private:
    Uint32 get_pixel(SDL_Surface *surface, int x, int y);

    color3i rgb;

    char composition[32];
    Sint32 textcursor;
    Sint32 selection_len;

    TTF_Font *font32;
    TTF_Font *font32_outline;
    TTF_Font *font24;
    TTF_Font *font24_outline;
    TTF_Font *font16;
    TTF_Font *font16_outline;
};

void Engine2D::setup(const char *name, int w, int h, Uint32 flags)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    width = w;
    heigth = h;

    const char *font_path = "res/font/cocogoose.ttf";

    font32 = TTF_OpenFont(font_path, 32);
    font32_outline = TTF_OpenFont(font_path, 32);
    font24 = TTF_OpenFont(font_path, 24);
    font24_outline = TTF_OpenFont(font_path, 24);
    font16 = TTF_OpenFont(font_path, 16);
    font16_outline = TTF_OpenFont(font_path, 16);

    if (font32_outline)
        TTF_SetFontOutline(font32_outline, 3);
    if (font24_outline)
        TTF_SetFontOutline(font24_outline, 3);
    if (font16_outline)
        TTF_SetFontOutline(font16_outline, 3);

    SDL_StartTextInput();

    update();
}

bool Engine2D::update()
{
    SDL_RenderPresent(renderer);
    SDL_GetWindowSize(window, &width, &heigth);
    SDL_GetRelativeMouseState(&cursor_x, &cursor_y);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mousebutton_l = true;
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                mousebutton_r = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mousebutton_l = false;
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                mousebutton_r = false;
            }
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) // scroll up
            {
                scroll_wheel_y -= 1;
            }
            else if (event.wheel.y < 0) // scroll down
            {
                scroll_wheel_y += 1;
            }

            if (event.wheel.x > 0) // scroll right
            {
                scroll_wheel_x += 1;
            }
            else if (event.wheel.x < 0) // scroll left
            {
                scroll_wheel_x -= 1;
            }
            break;

        case SDL_TEXTINPUT:
            strcat(text, event.text.text);
            textlen += 1;
            break;

        case SDL_TEXTEDITING:
            strcpy(composition, event.edit.text);
            textcursor = event.edit.start;
            selection_len = event.edit.length;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && textlen >= 1)
            {
                text[textlen - 1] = NULL;
                textlen--;
            }
            switch (event.key.keysym.sym)
            {
            default:
                keyboard_sym = event.key.keysym.sym;
            }
            break;

        default:
            break;
        }
    }

    key = SDL_GetKeyboardState(NULL);

    return true;
}

void Engine2D::cursor(int en)
{
    SDL_ShowCursor(en);
}

void Engine2D::color(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
}

void Engine2D::draw_pixel(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

void Engine2D::draw_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    draw_pixel(x, y);
}

void Engine2D::draw_line(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Engine2D::draw_line(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    draw_line(x1, y1, x2, y2);
}

void Engine2D::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

void Engine2D::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    draw_triangle(x1, y1, x2, y2, x3, y3);
}

void Engine2D::draw_triangle(triangle t, Uint8 r, Uint8 g, Uint8 b)
{
    draw_triangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, r, g, b);
}

void Engine2D::fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1 > y2)
    {
        swap(y1, y2);
        swap(x1, x2);
    }
    if (y1 > y3)
    {
        swap(y1, y3);
        swap(x1, x3);
    }
    if (y2 > y3)
    {
        swap(y2, y3);
        swap(x2, x3);
    }

    t1x = t2x = x1;
    y = y1; // Starting points
    dx1 = (int)(x2 - x1);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1);
    if (dx2 < 0)
    {
        dx2 = -dx2;
        signx2 = -1;
    }
    else
        signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1)
    { // swap values
        swap(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2)
    { // swap values
        swap(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2)
        goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            i++;
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                    t1xp = signx1; // t1x += signx1;
                else
                    goto next1;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2; // t2x += signx2;
                else
                    goto next2;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next2:
        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        draw_line(minx, y, maxx, y); // Draw line from min to max points found on the y
                                     // Now increase y
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2)
            break;
    }
next:
    // Second half
    dx1 = (int)(x3 - x2);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1)
    { // swap values
        swap(dy1, dx1);
        changed1 = true;
    }
    else
        changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                {
                    t1xp = signx1;
                    break;
                } // t1x += signx1;
                else
                    goto next3;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
            if (i < dx1)
                i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2;
                else
                    goto next4;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next4:

        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        draw_line(minx, y, maxx, y);
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y > y3)
            return;
    }
}

void Engine2D::fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    fill_triangle(x1, y1, x2, y2, x3, y3);
}

void Engine2D::fill_triangle(triangle t, Uint8 r, Uint8 g, Uint8 b)
{
    fill_triangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, r, g, b);
}

void Engine2D::draw_texture(texture tex, int x1, int y1, int x2, int y2)
{
    int w = x2 - x1;
    int h = y2 - y1;

    float step_x = tex.w / w;
    float step_y = tex.h / h;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            color3i c = tex.pixel[(int)(step_x * x + step_y * y * (tex.w - 1))];
            color(c.r, c.g, c.b);
            draw_pixel(x, y);
        }
    }
}

void Engine2D::draw_texture_wh(texture tex, int x, int y, int w, int h)
{
    float step_x = tex.w / w;
    float step_y = tex.h / h;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            color3i c = tex.pixel[(int)(step_x * x + step_y * y * (tex.w - 1))];
            color(c.r, c.g, c.b);
            draw_pixel(x, y);
        }
    }
}

void Engine2D::draw_texture(texture tex, int x, int y)
{
    draw_texture(tex, x, y, tex.w, tex.h);
}

void Engine2D::draw_text(string text, int size, bool outline, int x, int y)
{
    TTF_Font *font;
    switch (size)
    {
    case 16:
        if (outline)
        {
            font = font16_outline;
        }
        else
        {
            font = font16;
        }
        break;

    case 24:
        if (outline)
        {
            font = font24_outline;
        }
        else
        {
            font = font24;
        }
        break;

    case 32:
        if (outline)
        {
            font = font32_outline;
        }
        else
        {
            font = font32;
        }
        break;

    default:
        font = font24;
        break;
    }
    SDL_Color c;
    c.r = rgb.r;
    c.g = rgb.g;
    c.b = rgb.b;

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), c);
    SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, message, &src, &dst);
    SDL_FreeSurface(surfaceMessage);
}

void Engine2D::fill(int x, int y, int w, int h)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderFillRect(renderer, &r);
}

texture Engine2D::load_texture(const char *path)
{
    texture tex;
    SDL_Surface *s = IMG_Load(path);
    if (s == NULL)
        return tex;
    SDL_UnlockSurface(s);
    tex.w = s->w;
    tex.h = s->h;
    for (int y = 1; y < tex.h; y++)
    {
        for (int x = 1; x < tex.w; x++)
        {
            SDL_Color rgb;
            Uint32 data = get_pixel(s, x, y);
            SDL_GetRGB(data, s->format, &rgb.r, &rgb.g, &rgb.b);
            color3i color;
            color.r = rgb.r;
            color.g = rgb.g;
            color.b = rgb.b;
            tex.pixel.push_back(color);
        }
    }
    SDL_FreeSurface(s);
    return tex;
}

color3i Engine2D::get_pixel(texture tex, int x, int y)
{
    return tex.pixel[x + tex.w * (tex.w - 1)];
}

Uint32 Engine2D::get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0; /* shouldn't happen, but avoids warnings */
    }
}