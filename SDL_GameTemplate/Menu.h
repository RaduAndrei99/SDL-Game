#pragma once
#include"SDL.h"
#include "Component.h"
#include "text.h"
#include "SDL_ttf.h"
#include"TextureManager.h"

class Menu :public Component {
	
	SDL_Renderer *renderer;
	Text* start, * quit, *difficulty,*easy,*medium,*hard;

	int nrElem;
	int selected;
	bool isDiff;

public:

	Menu(SDL_Renderer *renderer);

	void draw();
	void init();

	void operator ++(int);
	void operator --(int);

	int getSelected(void);
	void setIsDiff(bool s)
	{
		this->isDiff = s;
	}

	bool getIsDiff(void)
	{
		return isDiff;
	}
};