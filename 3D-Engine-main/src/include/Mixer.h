#pragma once

#include "include.h"

struct audio
{
    string name;
    string path;

    Mix_Chunk *chunk;

    float volume;

    int channel;
};

class Mixer
{
public:
    Mixer(int channels);

    void load(string path, string name);
    void load(string path, string name, int channel);

    void unLoad(string name);
    void unLoad(int ID);

    void play(string name, int channel);
    void play(string name);

    void play_n(string name, int channel, int n);
    void play_n(string name, int n);

    void play(int ID, int channel);
    void play(int ID);

    void play_n(int ID, int channel, int n);
    void play_n(int ID, int n);

    bool active(int channel);

    void setDirection(int channel, float angle, float distance, float strength);

private:
    vector<audio> audioList;
};

Mixer::Mixer(int channels)
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, channels, 2048);
}

void Mixer::load(string path, string name)
{
    audio a;
    a.name = name;
    a.path = path;

    a.chunk = Mix_LoadWAV(path.c_str());
    a.volume = 1.0f;
    a.channel = 0;

    audioList.push_back(a);
}
void Mixer::load(string path, string name, int channel)
{
    audio a;
    a.name = name;
    a.path = path;

    a.chunk = Mix_LoadWAV(path.c_str());
    a.volume = 1.0f;
    a.channel = channel;

    audioList.push_back(a);
}

void Mixer::unLoad(string name)
{
    int s = audioList.size();
    for (int i = 0; i < s; i++)
    {
        if (audioList[i].name == name)
        {
            vector<audio> tmp;
            for (int x = 0; x < audioList.size(); x++)
            {
                if (x != i)
                {
                    tmp.push_back(audioList[x]);
                }
            }
            audioList.clear();
            audioList = tmp;
            break;
        }
    }
}
void Mixer::unLoad(int ID)
{
    vector<audio> tmp;
    for (int x = 0; x < audioList.size(); x++)
    {
        if (x != ID)
        {
            tmp.push_back(audioList[x]);
        }
    }
    audioList.clear();
    audioList = tmp;
}

void Mixer::play(string name, int channel)
{
    int s = audioList.size();
    for (int i = 0; i < s; i++)
    {
        if (audioList[i].name == name)
        {
            play(i, channel);
            return;
        }
    }
}
void Mixer::play(string name)
{
    int s = audioList.size();
    for (int i = 0; i < s; i++)
    {
        if (audioList[i].name == name)
        {
            play(i);
            return;
        }
    }
}

void Mixer::play_n(string name, int channel, int n)
{
    int s = audioList.size();
    for (int i = 0; i < s; i++)
    {
        if (audioList[i].name == name)
        {
            play_n(i, channel, n);
            return;
        }
    }
}
void Mixer::play_n(string name, int n)
{
    int s = audioList.size();
    for (int i = 0; i < s; i++)
    {
        if (audioList[i].name == name)
        {
            play_n(i, n);
            return;
        }
    }
}

void Mixer::play(int ID, int channel)
{
    Mix_PlayChannel(channel, audioList[ID].chunk, 0);
}
void Mixer::play(int ID)
{
    Mix_PlayChannel(audioList[ID].channel, audioList[ID].chunk, 0);
}

void Mixer::play_n(int ID, int channel, int n)
{
    Mix_PlayChannel(channel, audioList[ID].chunk, n);
}
void Mixer::play_n(int ID, int n)
{
    Mix_PlayChannel(audioList[ID].channel, audioList[ID].chunk, n);
}

bool Mixer::active(int channel)
{
    return Mix_Playing(channel);
}

void Mixer::setDirection(int channel, float angle, float distance, float strength)
{
    Mix_SetPosition(channel, angle, distance * strength);
}