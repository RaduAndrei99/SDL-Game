#include"text.h"
#include"TextureManager.h"
#include<iostream>

using namespace std;

Text::Text(const char* f, const char* msg,SDL_Renderer *renderer,int x,int y,int h,int w,bool isH,SDL_Color col) : renderer(renderer)
{
	destRect.x = x;
	destRect.y = y;
	destRect.w = w;
	destRect.h = h;
	isHighlight = isH;
	
	if (TTF_Init() == -1) 
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	this->font = TTF_OpenFont(f, 100);

	color = col;

	surfaceMessage = TTF_RenderText_Solid(this->font, msg, color);

	Message = SDL_CreateTextureFromSurface(this->renderer, surfaceMessage);
}

void Text::init(void)
{
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.h = 0;
	srcRect.w = 0;
}

void Text::draw(void)
{
	if (isHighlight == true)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &destRect);
	}
	TextureManager::Draw(Message, srcRect, destRect, renderer);
}

void Text::setIsHighlight(bool s)
{
	this->isHighlight = s;
}

bool Text::getIsHighlight(void)
{
	return this->isHighlight;
}