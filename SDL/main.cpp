#include "SDL.h"
#include "sstream"
#include <iostream>

#define WINHEIGHT  600
#define WINWIDTH  800
#define PADDLEHEIGHT 100
#define PADDLEWIDTH 14
#define BALLWIDTH 20
#define BALLHEIGHT 20
//Variables
SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;
SDL_Rect BallRec, PaddleLeft, PaddleRight;
//bool for start engine
bool run = true;

//prototypes
void PaddleRect();
void Update();
void Render(SDL_Renderer* render);
void Clean();

//rec speed and collision controls
float BallSpeedx = 5, BallSpeedy = 5;
float PaddleSpeedLeft = 15.0f, PaddleSpeedRight = 15.0f;

int main(int argc, char* argv[])
{
	enum Buttons
	{
		PaddleOneUp = 0,
		PaddleOneDown,
		PaddleTwoUp,
		PaddleTwoDown,
	};
	//for buttons call enum
	bool buttons[4] = {};

	const int FPS = 60; //for frame control
	const int FrameDelay = 1000 / 60;
	Uint32 FrameStart;
	int FrameTime;

	SDL_Init(SDL_INIT_EVERYTHING);

	win = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINWIDTH, WINHEIGHT, 0);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	//Init Paddle
	PaddleRect();

	while (run)
	{
		//Mix_PlayChannel(-1, Song, 0);
		FrameStart = SDL_GetTicks();
		//Functions
		Update();
		
		Render(ren);
		//Events Handling
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				run = false;
			}

			//key function control with bool
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					run = false;
				}
				else if (event.key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = true;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = true;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = true;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = true;
				}
			}//handling key with bool
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = false;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = false;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = false;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = false;
				}
			}
		}
		//handling bool true behaviour
		if (buttons[Buttons::PaddleOneUp] && PaddleLeft.y != 0)
		{
			PaddleLeft.y -= 15;
		}
		else if (buttons[Buttons::PaddleOneDown] && PaddleLeft.y + PaddleLeft.h != 600)
		{
			PaddleLeft.y += 15;
		}
		if (buttons[Buttons::PaddleTwoUp] && PaddleRight.y != 0)
		{
			PaddleRight.y -= 15;
		}
		else if (buttons[Buttons::PaddleTwoDown] && PaddleRight.y + PaddleRight.h != 600)
		{
			PaddleRight.y += 15;
		}

		FrameTime = SDL_GetTicks() - FrameStart;
		if (FrameDelay > FrameTime)
		{
			SDL_Delay(FrameDelay - FrameTime);
		}
	}
	Clean();

	return 0;
}

void PaddleRect()
{
	//Ballrect
	BallRec.x = 400; BallRec.y = 300; BallRec.w = BALLWIDTH; BallRec.h = BALLHEIGHT;
	//leftP
	PaddleLeft.x = 20; PaddleLeft.y = (WINHEIGHT / 2) - 50; PaddleLeft.w = PADDLEWIDTH; PaddleLeft.h = PADDLEHEIGHT;
	//RightP
	PaddleRight.x = 760; PaddleRight.y = (WINHEIGHT / 2) - 50; PaddleRight.w = PADDLEWIDTH; PaddleRight.h = PADDLEHEIGHT;
}

void Update()
{
	//Initial Start Speed
	BallRec.x += BallSpeedx;
	BallRec.y += BallSpeedy;
	//If Out of Boundary reset to below
	if (BallRec.x >= 800)
	{
		BallRec.x -= BallSpeedx;
		BallRec.y -= BallSpeedy;
		BallRec.x = 400;
		BallRec.y = 300;
	}
	else if (BallRec.x <= 0)
	{
		BallRec.x += BallSpeedx;
		BallRec.y += BallSpeedy;
		BallRec.x = 400;
		BallRec.y = 300;
	}
	//Right P
	if (BallRec.x >= 740 && BallRec.y <= PaddleRight.y + 100 && BallRec.y >= PaddleRight.y)
	{
		BallSpeedx *= -1;
	}
	//Left P
	else if (BallRec.x <= 30 && BallRec.y <= PaddleLeft.y + 100 && BallRec.y >= PaddleLeft.y)
	{
		BallSpeedx *= -1;
	}
	//Top Wall Collision
	else if (BallRec.y <= 0 || BallRec.y + 20 >= 600)
	{
		BallSpeedy *= -1;
	}
}
void Render(SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 0, 0, 0, 0xFF);
	SDL_RenderClear(render);

	SDL_SetRenderDrawColor(ren, 0, 0, 255, 100);
	SDL_RenderFillRect(ren, &BallRec);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 100);
	SDL_RenderFillRect(ren, &PaddleLeft);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 100);
	SDL_RenderFillRect(ren, &PaddleRight);

	SDL_RenderPresent(ren);
}
void Clean()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
}