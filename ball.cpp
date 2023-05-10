#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "ball.h"
#include "Math.h"


using namespace std;

Ball::Ball(Vector2f p_pos, SDL_Texture* p_tex, SDL_Texture* p_pointTex, SDL_Texture* p_powerMTexBG, SDL_Texture* p_powerMTexFG, int p_index)
:Entity(p_pos, p_tex)
{
    index = p_index;
    arrows.push_back(Entity(Vector2f(-60,-60), p_pointTex));
    powerBars.push_back(Entity(Vector2f(-64, -64), p_powerMTexBG));
    powerBars.push_back(Entity(Vector2f(-64, -64), p_powerMTexFG));
}

void Ball::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}

void Ball::setLaunchedVelocity(float x, float y)
{
    launchedVelocity.x = x;
    launchedVelocity.y = y;
}

void Ball::setInitialMousePos(float x, float y)
{
    initialMousePos.x = x;
    initialMousePos.y = y;
}

void Ball::setWin(bool p_win)
{
    win = p_win;
}

void Ball::update(double deletaTime, bool mouseDown, bool mousePressed, vector<Entity> holes, vector<Entity> tiles, Mix_Chunk* holeSfx, Mix_Chunk* hit_airSfx, Mix_Chunk* swingSfx)
{
    if(win)
    {//dịch chuyển dần vào lỗ và thu nhỏ kích thước khi win
        if(getPos().x < tagret.x)
        {
            setPos(getPos().x += 0.1*deletaTime, getPos().y);
        }
        else if(getPos().x > tagret.x)
        {
            setPos(getPos().x -= 0.1*deletaTime, getPos().y);
        }
        if(getPos().y < tagret.y)
        {
            setPos(getPos().x, getPos().y += 0.1*deletaTime);
        }
        else if(getPos().y > tagret.y)
        {
            setPos(getPos().x, getPos().y -= 0.1*deletaTime);
        }
        setScale(getScale().x - 0.0025*deletaTime, getScale().y - 0.0025*deletaTime);
        return;
    }

    for(Entity& hole : holes)
    {
        if(10*velocity1D + SDL_sqrt(SDL_pow(getPos().x+8 - (hole.getPos().x+8), 2) + SDL_pow(getPos().y+8 - (hole.getPos().y+11), 2))<8)
    {
        Mix_PlayChannel(-1, holeSfx, 0);
        setWin(true);
        tagret.x = hole.getPos().x;
        tagret.y = hole.getPos().y + 3;
    }
    }

    if(mousePressed && canMove)
    {
        Mix_PlayChannel(-1, hit_airSfx, 0);
        int mouseX = 0;
        int mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        setInitialMousePos(mouseX,mouseY);
    }

    if(mouseDown && canMove)
    {
        Playswing = false;
        int mouseX = 0;
        int mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        setVelocity((mouseX - getInitialMousepos().x)/-150, (mouseY - getInitialMousepos().y)/-150);
        setLaunchedVelocity((mouseX - getInitialMousepos().x)/-150, (mouseY - getInitialMousepos().y)/-150);
        velocity1D = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
        launchedVelocity1D = velocity1D;

        arrows.at(0).setPos(getPos().x, getPos().y + 8 - 32);
        arrows.at(0).setAngle(SDL_atan2(getVelocity().y, getVelocity().x)*(180/M_PI) + 90);

        dirX = velocity.x/abs(velocity.x);
        dirY = velocity.y/abs(velocity.y);

        powerBars.at(0).setPos(getPos().x + 8 + 40, getPos().y - 32);
        powerBars.at(1).setPos(getPos().x + 8 + 40 +4, getPos().y - 32 + 4 + 28 - 28*powerBars.at(1).getScale().y);

        if(velocity1D > 1)
        {
            velocity1D = 1;
            launchedVelocity1D = 1;
        }

        powerBars.at(1).setScale(1, velocity1D/1);
    }
    else
    {
        if(!Playswing)
        {
            Mix_PlayChannel(-1, swingSfx, 0);
            Playswing = true;
            score++;
        }
        arrows.at(0).setPos(-60,-60);
        powerBars.at(0).setPos(-64,-64);
        powerBars.at(1).setPos(-64,-64);
        setPos(getPos().x + getVelocity().x*deletaTime, getPos().y + getVelocity().y*deletaTime);
        if(getVelocity().x > 0.0001 || getVelocity().x < -0.0001 || getVelocity().y > 0.0001 || getVelocity().y < -0.0001)
        {
            if(velocity1D > 0)
            {
                velocity1D -= friction*deletaTime;
            }

            else
            {
                velocity1D = 0;
            }
            velocity.x = (velocity1D/launchedVelocity1D)*abs(launchedVelocity.x)*dirX;
            velocity.y = (velocity1D/launchedVelocity1D)*abs(launchedVelocity.y)*dirY;
        }
        else
        {
            setVelocity(0,0);
            int mouseX = 0;
            int mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            setInitialMousePos(mouseX, mouseY);
            canMove = true;
        }

        if(getPos().x + getCurrentFrame().w > 640/(2-index))
        {
            setVelocity(-abs(getVelocity().x), getVelocity().y);
            dirX = -1;
        }
        else if(getPos().x < 0 + index*320)
        {
            setVelocity(abs(getVelocity().x), getVelocity().y);
            dirX = 1;
        }
        else if(getPos().y + getCurrentFrame().h > 480)
        {
            setVelocity(getVelocity().x, -abs(getVelocity().y));
            dirY = -1;
        }
        else if(getPos().y < 0)
        {
            setVelocity(getVelocity().x, abs(getVelocity().y));
            dirY = 1;
        }

        for(Entity& tile : tiles)
        {
            float newX = getPos().x + getVelocity().x*deletaTime;
            float newY = getPos().y;

            if(newX + 16 > tile.getPos().x && newX < tile.getPos().x + tile.getCurrentFrame().w && newY + 16 > tile.getPos().y && newY < tile.getPos().y + tile.getCurrentFrame().h-3)
            {
                setVelocity(getVelocity().x*-1, getVelocity().y);
                dirX *= -1;
            }

            newX = getPos().x;
            newY = getPos().y + getVelocity().y*deletaTime;

            if(newX + 16 > tile.getPos().x && newX < tile.getPos().x + tile.getCurrentFrame().w && newY + 16 > tile.getPos().y && newY < tile.getPos().y + tile.getCurrentFrame().h-3)
            {
                    setVelocity(getVelocity().x, getVelocity().y*-1);
                    dirY *= -1;
            }
        }
    }
}














