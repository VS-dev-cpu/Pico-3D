#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

#include "3DEngine.h"
#else
#include "include/3DEngine.h"
#endif

Mixer mix(4);
// engine3d.engine2d engine3d.engine2d("Base 3D", 720, 480, SDL_WINDOW_SHOWN);
Engine3D engine3d("Base (3D)", 720, 480);
gameClock clk;

float pitch, yaw;
float sensitivity = 1.0f;

bool running = true;

bool init()
{
    // Setup
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    clk.update();

    engine3d.engine2d.cursor(0);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Loading

    engine3d.loadObject("map", "Map.obj");
    engine3d.loadObject("cube", "human.obj");
    engine3d.get_object("cube")->rotateYaw(90);

    // mix.load("res/sfx/music1.wav", "asdf");

    // Preparing

    engine3d.get_object("cube")->move(0.0f, 10.0f, 20.0f);
    engine3d.get_object("cube")->update(true);
    for (int i = 0; i < engine3d.get_object("cube")->meshData.tris.size(); i++)
    {
        engine3d.get_object("cube")->meshData.tris[i].color.r = 255;
        engine3d.get_object("cube")->meshData.tris[i].color.g = 0;
        engine3d.get_object("cube")->meshData.tris[i].color.b = 0;
    }

    engine3d.addLight("cube_light", spot, WHITEf, 0.5f, 50.0f, POS_NULL, POS_NULL);
    engine3d.get_light(0)->move(engine3d.get_object("cube")->position);

    engine3d.get_camera()->setPosition(0.0f, 5.0f, 0.0f);

    // mix.play_n("asdf", -1);

    return true;
}

bool load = init();

bool loop()
{
    engine3d.engine2d.color(0, 0, 0);
    engine3d.engine2d.fill(0, 0, 720, 480);
    engine3d.get_light(0)->position = engine3d.get_object("cube")->position;

    /*int rot; // === (int)angle(engine3d.get_camera()->position, engine3d.get_object("cube")->position);
    int dist = distance(engine3d.get_camera()->position, engine3d.get_object("cube")->position);
    mix.setDirection(0, rot, dist, 5);*/

    float speed = 10.0f * clk.deltaTime;
    if (engine3d.engine2d.key[SDL_SCANCODE_LCTRL])
        speed = 20.0f * clk.deltaTime;

    if (engine3d.engine2d.key[SDL_SCANCODE_SPACE])
        engine3d.get_camera()->moveY(speed);
    if (engine3d.engine2d.key[SDL_SCANCODE_LSHIFT])
        engine3d.get_camera()->moveY(-speed);

    if (engine3d.engine2d.key[SDL_SCANCODE_W])
        engine3d.get_camera()->moveZ(speed);
    if (engine3d.engine2d.key[SDL_SCANCODE_S])
        engine3d.get_camera()->moveZ(-speed);

    if (engine3d.engine2d.key[SDL_SCANCODE_D])
        engine3d.get_camera()->moveX(speed);
    if (engine3d.engine2d.key[SDL_SCANCODE_A])
        engine3d.get_camera()->moveX(-speed);

    pitch += engine3d.engine2d.cursor_y * sensitivity;
    yaw += engine3d.engine2d.cursor_x * sensitivity;

    engine3d.get_camera()->setPitch(pitch);
    engine3d.get_camera()->setYaw(yaw);

    float cubeSpeed = 5.0f * clk.deltaTime;

    if (engine3d.get_object("cube")->position.x > engine3d.get_camera()->position.x + 1)
    {
        engine3d.get_object("cube")->move(-cubeSpeed, 0.0f, 0.0f);
    }
    else if (engine3d.get_object("cube")->position.x < engine3d.get_camera()->position.x - 1)
    {
        engine3d.get_object("cube")->move(cubeSpeed, 0.0f, 0.0f);
    }

    if (engine3d.get_object("cube")->position.y > engine3d.get_camera()->position.y + 1)
    {
        engine3d.get_object("cube")->move(0.0f, -cubeSpeed, 0.0f);
    }
    else if (engine3d.get_object("cube")->position.y < engine3d.get_camera()->position.y - 1)
    {
        engine3d.get_object("cube")->move(0.0f, cubeSpeed, 0.0f);
    }

    if (engine3d.get_object("cube")->position.z > engine3d.get_camera()->position.z + 1)
    {
        engine3d.get_object("cube")->move(0.0f, 0.0f, -cubeSpeed);
    }
    else if (engine3d.get_object("cube")->position.z < engine3d.get_camera()->position.z - 1)
    {
        engine3d.get_object("cube")->move(0.0f, 0.0f, cubeSpeed);
    }
    engine3d.get_object("cube")->setPitch(0);
    engine3d.get_object("cube")->rotateYaw(5);

    if (distance(engine3d.get_object("cube")->position, engine3d.get_camera()->position) < 3.0f)
    {
        cout << "You died!\n";
        return false;
    }

    engine3d.get_object("cube")->update(false);

    if (engine3d.engine2d.key[SDL_SCANCODE_ESCAPE])
        return false;

    clk.update();

    bool running = engine3d.update(clk.deltaTime);
    return running;
}

#ifdef __EMSCRIPTEN__
void emLoop()
{
    loop();
}
#endif

int main()
{

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(emLoop, 0, 1);
#else
    while (running)
    {
        running = loop();
    }
#endif

    SDL_Quit();

    return 0;
}