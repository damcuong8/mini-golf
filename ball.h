#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "entity.h"
#include "Math.h"

using namespace std;

class Ball : public Entity
{
public:
    Ball(Vector2f p_pos, SDL_Texture* p_tex, SDL_Texture* p_pointTex, SDL_Texture* p_powerMTexBG, SDL_Texture* p_powerMTexFG, int p_index);
    Vector2f& getVelocity()
    {
        return velocity;
    }

    Vector2f& getInitialMousepos()
    {
        return initialMousePos;
    }

    vector<Entity> getArrows()
    {
        return arrows;
    }

    vector<Entity> getPowerBar()
    {
        return powerBars;
    }

    int getScore()
    {
        return score;
    }

    bool isWin()
    {
        return win;
    }

    void setInitialMousePos(float x, float y);
    void setVelocity(float x, float y);
    void setLaunchedVelocity(float x, float y);
    void update(double deltaTime, bool mouseDown, bool mousePressed, std::vector<Entity> holes, std::vector<Entity> tiles, Mix_Chunk* chargeSfx, Mix_Chunk* swingSfx, Mix_Chunk* holeSfx);
    void setWin(bool p_win);

private:

    bool canMove = true;
    bool win = false;
    bool Playswing = true;
    Vector2f velocity;
    Vector2f launchedVelocity;
    Vector2f initialMousePos;
    Vector2f tagret;
    float velocity1D;
    float launchedVelocity1D;
    float friction = 0.001;
    int dirX = 1;
    int dirY = 1;
    int index;
    int score = 0;
    vector<Entity> arrows;
    vector<Entity> powerBars;
};
