#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Math.h"


class Entity
{
public:
    Entity(Vector2f p_pos, SDL_Texture* p_tex);
    Vector2f& getPos()
    {
        return pos;
    }

    float getAngle()
    {
        return angle;
    }

    Vector2f getScale()
    {
        return scale;
    }

    void setPos(float x, float y);
    void setScale(float w, float h);
    void setAngle(float angle);
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();
private:
    float angle;
    Vector2f scale = Vector2f(1,1);
    Vector2f pos;
    SDL_Texture* tex;
    SDL_Rect currentFrame;
};
