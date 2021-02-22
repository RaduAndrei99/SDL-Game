#pragma once
#include <SDL.H>
#include"Map.h"

extern Map* map;


class Game {
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isMenu=false;
	int difficulty;

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update() const;
	void render() const;
	void clean() const;

	bool running() const { return isRunning; }
	bool getIsMenu(void);
	void handleAndDrawMenu(void);
};
