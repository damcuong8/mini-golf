#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>

#include "ballc.h"
#include "entity.h"


using namespace std;

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* loadTexture(const char* p_filepath);
    void render(int x, int y, SDL_Texture* texture);
    void render(Entity& p_entity);
    void renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color texColor);
    void cleanUp();
    void clear();
    void display();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
    if(window == NULL)
        cout << "Window failed to init. Error " << SDL_GetError() << endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        cout << "Renderer is not valid. Error: " << SDL_GetError() << endl;
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
	if (!(TTF_Init()))
		std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	return true;
}

bool SDLinit = init();


SDL_Texture* RenderWindow::loadTexture(const char* p_filepath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filepath);

    if(texture == NULL)
        cout << "Failed to load texture. Error " << SDL_GetError() << endl;

    return texture;
}

void RenderWindow::render(Entity& p_entity)
{
    SDL_Rect src;

    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    src.w = p_entity.getCurrentFrame().w;
    src.h = p_entity.getCurrentFrame().h;

    SDL_Rect dst;

    dst.x = p_entity.getPos().x + (p_entity.getCurrentFrame().w - p_entity.getCurrentFrame().w*p_entity.getScale().x)/2;
    dst.y = p_entity.getPos().y + (p_entity.getCurrentFrame().h - p_entity.getCurrentFrame().h*p_entity.getScale().y)/2;
    dst.w = p_entity.getCurrentFrame().w*p_entity.getScale().x;
    dst.h = p_entity.getCurrentFrame().h*p_entity.getScale().y;

    SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, p_entity.getAngle(), 0, SDL_FLIP_NONE);
}

void RenderWindow::render(int x, int y, SDL_Texture* p_tex)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;

    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color texColor)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, texColor);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h;

    SDL_Rect dst;

    dst.x = 640/2 - src.w/2 - p_x;
    dst.y = 480/2 - src.h/2 - p_y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, message, &src, &dst);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

RenderWindow window("cuong", 640, 480);
SDL_Texture* bgTexture = window.loadTexture("all/anh/bg.png");
SDL_Texture* ballTexture = window.loadTexture("all/anh/ball.png");
SDL_Texture* holeTexture = window.loadTexture("all/anh/hole.png");
SDL_Texture* pointTexture = window.loadTexture("all/anh/point.png");
SDL_Texture* powerMBGTexture = window.loadTexture("all/anh/powermeter_bg.png");
SDL_Texture* powerMFGTexture = window.loadTexture("all/anh/powermeter_fg.png");
SDL_Texture* ballShadowTexture = window.loadTexture("all/anh/ball_shadow.png");
SDL_Texture* tileDarkTexture32 = window.loadTexture("all/anh/tile32_dark.png");
SDL_Texture* tileDarkTexture64 = window.loadTexture("all/anh/tile64_dark.png");
SDL_Texture* tileLightTexture32 = window.loadTexture("all/anh/tile32_light.png");
SDL_Texture* tileLightTexture64 = window.loadTexture("all/anh/tile64_light.png");
SDL_Texture* countHolebgTexture = window.loadTexture("all/anh/countHolebg.png");
SDL_Texture* scorebgTexture = window.loadTexture("all/anh/scorebg.png");
SDL_Texture* endScreenTexture = window.loadTexture("all/anh/endScreen.png");
SDL_Texture* logoTexture = window.loadTexture("all/anh/logo.png");
SDL_Texture* nenTexture = window.loadTexture("all/anh/nen.png");


Mix_Chunk* hit_airSfx = Mix_LoadWAV("all/sfx/hit_air.mp3");
Mix_Chunk* swingSfx = Mix_LoadWAV("all/sfx/swing.mp3");
Mix_Chunk* holeSfx = Mix_LoadWAV("all/sfx/hole.mp3");

TTF_Font* font48 = TTF_OpenFont("all/font/font.ttf", 48);
TTF_Font* font24 = TTF_OpenFont("all/font/font.ttf", 24);
TTF_Font* font32 = TTF_OpenFont("all/font/font.ttf", 32);

SDL_Color white = {255, 255, 255};

Ball balls[2] = {Ball(Vector2f(0,0), ballTexture, pointTexture, powerMBGTexture, powerMFGTexture,0), Ball(Vector2f(0,0), ballTexture, pointTexture, powerMBGTexture, powerMFGTexture,1)};
vector<Entity> holes = {Entity(Vector2f(0,0), holeTexture),Entity(Vector2f(0,0), holeTexture)};

const int par = 10;//số gậy cần để thắng

vector<Entity> loadTiles(int level)
{
	std::vector<Entity> temp = {};
	switch(level)
	{
		case 0:
			temp.push_back(Entity(Vector2f(64*3, 64*3), tileDarkTexture64));
			temp.push_back(Entity(Vector2f(64*1, 64*3), tileDarkTexture64));

			temp.push_back(Entity(Vector2f(64*3 + 64*5, 64*3), tileLightTexture64));
			temp.push_back(Entity(Vector2f(64*1 + 64*5, 64*3), tileLightTexture64));
		break;
		case 1:
			temp.push_back(Entity(Vector2f(64*2, 64*3), tileDarkTexture64));

			temp.push_back(Entity(Vector2f(64*4 + 64*5, 64*3), tileLightTexture64));
		break;
		case 2:
			temp.push_back(Entity(Vector2f(32*1 + 32*10 + 16, 32*5), tileLightTexture32));
		break;
		case 3:
			temp.push_back(Entity(Vector2f(32*4, 32*7), tileDarkTexture64));
			temp.push_back(Entity(Vector2f(32*3, 32*5), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*6, 32*3), tileDarkTexture32));

			temp.push_back(Entity(Vector2f(32*4 + 64*5, 32*2), tileLightTexture64));
			temp.push_back(Entity(Vector2f(32*3 + 32*10, 32*6), tileLightTexture32));
			temp.push_back(Entity(Vector2f(32*6 + 32*10, 32*9), tileLightTexture32));
		break;
		case 4:
			temp.push_back(Entity(Vector2f(32*3, 32*1), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*1, 32*3), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*5, 32*3), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*3, 32*5), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*7, 32*5), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*7, 32*10), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*3, 32*10), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*5, 32*12), tileDarkTexture32));
			temp.push_back(Entity(Vector2f(32*7, 32*10), tileDarkTexture32));

			temp.push_back(Entity(Vector2f(32*4, 32*7), tileDarkTexture64));
			temp.push_back(Entity(Vector2f(32*8, 32*7), tileDarkTexture64));

			temp.push_back(Entity(Vector2f(32*2 + 32*10, 32*2), tileLightTexture32));
			temp.push_back(Entity(Vector2f(32*5 + 32*10, 32*11), tileLightTexture32));

			temp.push_back(Entity(Vector2f(32*3 + 32*10, 32*1), tileLightTexture64));
			temp.push_back(Entity(Vector2f(32*8 + 32*10, 32*6), tileLightTexture64));
			temp.push_back(Entity(Vector2f(32*3 + 32*10, 32*11), tileLightTexture64));
		break;
	}
	return temp;
}

int level = 0;
vector<Entity> tiles = loadTiles(level);
int state = 0; // 0 = title screen, 1 = game, 2 = end game
bool gameRunning = true;
bool mouseDown = false;
bool mousePressed = false;

bool swingPlay = false;

int getBiggestScore()
{
    int biggestScore = balls[0].getScore();

    if(balls[1].getScore() > biggestScore)
    {
        biggestScore = balls[1].getScore();
    }
    return biggestScore;
}


void loadLevel(int level)
{
    if(level > 4)
    {
        state = 2;
        return;
    }
    balls[0].setVelocity(0,0);
    balls[1].setVelocity(0,0);
    balls[0].setScale(1,1);
    balls[1].setScale(1,1);
    balls[0].setWin(false);
    balls[1].setWin(false);

    tiles = loadTiles(level);

    switch (level)
	{
		case 0:
			balls[0].setPos(24 + 32*4, 24 + 32*11);
			balls[1].setPos(24 + 32*4 + 32*10, 24 + 32*11);

			holes.at(0).setPos(24 + 32*4, 22 + 32*2);
			holes.at(1).setPos(24 + 32*4 + 32*10, 22 + 32*2);
		break;
		case 1:
			balls[0].setPos(24 + 32*4, 24 + 32*11);
			balls[1].setPos(24 + 32*4 + 32*10, 24 + 32*11);

			holes.at(0).setPos(24 + 32*4, 22 + 32*2);
			holes.at(1).setPos(24 + 32*4 + 32*10, 22 + 32*2);
		break;
		case 2:
			balls[0].setPos(8 + 32*7, 8 + 32*10);
			balls[1].setPos(8 + 32*7 + 32*10, 8 + 32*10);

			holes.at(0).setPos(8 + 32*2, 6 + 32*5);
			holes.at(1).setPos(8 + 32*4 + 32*10, 6 + 32*3);
		break;
		case 3:
			balls[0].setPos(24 + 32*4, 24 + 32*5);
			balls[1].setPos(24 + 32*4 + 32*10, 24 + 32*4);

			holes.at(0).setPos(24 + 32*4, 22 + 32*1);
			holes.at(1).setPos(24 + 32*4 + 32*10, 22 + 32*11);
		break;
		case 4:
			balls[0].setPos(24 + 32*2, 24 + 32*12);
			balls[1].setPos(24 + 32*0 + 32*10, 24 + 32*5);

			holes.at(0).setPos(24 + 32*1, 22 + 32*1);
			holes.at(1).setPos(24 + 32*0 + 32*10, 22 + 32*7);
		break;
	}

}

SDL_Event event;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;

void update()
{
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    deltaTime = (double)(currentTick - lastTick)*1000/(double)SDL_GetPerformanceFrequency();

    mousePressed = false;
    // kiểm tra sự kiện để đưa đang hàm ball update
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gameRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = true;
                mousePressed = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = false;
            }
            break;
        }
    }

    if(state == 1)
    {
            for(Ball& ball: balls)
            {
            ball.update(deltaTime, mouseDown, mousePressed, holes, tiles, holeSfx, hit_airSfx, swingSfx);
            }

        if(balls[0].getScale().x < -1 && balls[1].getScale().x < -1)
        {
        level++;
        loadLevel(level);
        }

        if(getBiggestScore() > 10)
        {
            state = 2;
        }
    }

}

const char* getScoreText()
{
    string s = to_string(getBiggestScore());
    s = "Score: " + s;
    return s.c_str();
}

const char* getHoleText(int side)
{
    int tempHole = (level + 1)*2 -1;
    if(side == 1)
    {
        tempHole++;
    }
    string s = to_string(tempHole);
    s = "Hole: " + s;

    return s.c_str();
}


void graphics()
{
    window.clear();
    window.render(0, 0, bgTexture);
    for(Entity& hole : holes)
    {
        window.render(hole);
    }
    for(Ball& ball : balls)
    {
        if(!ball.isWin())
        {
            window.render(ball.getPos().x, ball.getPos().y + 4, ballShadowTexture);
        }
           for(Entity& arrow : ball.getArrows())
            {
                window.render(arrow);
            }
        window.render(ball);
    }
        for(Entity& tile : tiles)
        {
            window.render(tile);
        }

        for(Ball& ball : balls)
        {

            for(Entity& bar : ball.getPowerBar())
            window.render(bar);
        }
    if(state != 2)
    {
        window.render(640/4 - 132/2, 480 -32, countHolebgTexture);
        window.renderCenter(160, -240 + 16, getHoleText(0), font24, white);

        window.render(640/2 + 640/4 - 132/2, 480 -32, countHolebgTexture);
        window.renderCenter(-160, -240 + 16, getHoleText(1), font24, white);

        window.render(640/2  - 196/2, 0, scorebgTexture);
        window.renderCenter( 196/4, 240 - 16, getScoreText(), font24, white);
        window.renderCenter(-196/4, 240 - 16, "PAR: 10", font24, white);
    }
    else
    {
        window.render(0,0, endScreenTexture);

        if(getBiggestScore() <= par)
        {
            window.renderCenter(0,60, "CONGRATULATION!", font48, white);
            window.renderCenter(0, -24, "YOU WIN!", font48, white);
        }
        else
        {
            window.renderCenter(0,60, "YOU LOSE!", font48, white);
            window.renderCenter(0, -24, "LET'S TRY IT AGAIN!", font48, white);
        }
    }
    window.display();
}

void createScreen()
{
    if(!swingPlay)
    {
        Mix_PlayChannel(-1, swingSfx, 0);
        swingPlay = true;
    }

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gameRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button = SDL_BUTTON_LEFT)
            {
                Mix_PlayChannel(-1, holeSfx, 0);
                state = 1;
            }
            break;
        }
    }

    window.clear();
    window.render(0,0, bgTexture);
    window.render(0,0, logoTexture);
    window.render(0,0, nenTexture);
    window.renderCenter(0, -240 + 48 + 16*5, "LEFT CLICK TO START", font32, white);
    window.display();
}

void game()
{
    if(state == 0)
    {
       createScreen();
    }
    else
    {
        update();
        graphics();
    }
}

int main (int argc, char* argv[])
{
    loadLevel(level);
    while(gameRunning)
    {
        game();
    }

    window.cleanUp();
    TTF_CloseFont(font32);
	TTF_CloseFont(font24);
	SDL_Quit();
	TTF_Quit();
    return 0;
}
