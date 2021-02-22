#pragma once
#include"Component.h"
#include"text.h"
#include"TextureManager.h"

class Scoreboard :public Component
{
	SDL_Texture* texture[6];
	SDL_Renderer* renderer;
	SDL_Rect srcRect{}, destRect{};
	int score;
public:
	Scoreboard(SDL_Renderer* renderer);
	void init(void);
	void draw(void);
	void convertScore(void);
	void addToScore(int val);
	void reset(void);
};

