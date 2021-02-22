#pragma once
#include"Player.h"

class Enemy:public Player {
	
	bool next;

public:

	Enemy()=default;
	Enemy(const char* path,SDL_Renderer* renderer, int posX, int posY);

	void init(void) override;

	void draw(void) override;
	void setTex(const char* path);
	void setVelocityY(int y);

	void processPos(void);

	SDL_Point mapCentre(void);

	SDL_Rect getDest(void)
	{
		return this->destRect;
	}
};

