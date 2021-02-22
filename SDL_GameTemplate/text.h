#pragma once
#include "SDL.h"
#include"Component.h"
#include "SDL_ttf.h"

class Text :public Component {
	SDL_Renderer *renderer;

	TTF_Font* font;

	SDL_Color color;

	SDL_Surface* surfaceMessage;

	SDL_Texture* Message;

	SDL_Rect destRect{}, srcRect{};

	bool isHighlight;

public:

	Text(const char* f, const char* msg,SDL_Renderer *renderer, int x, int y, int h, int w,bool isH, SDL_Color col);

	void init(void);

	void draw(void);
	void setIsHighlight(bool s);
	bool getIsHighlight(void);
};
