#pragma once
#include <SDL_image.h>
#include<fstream>
#include<iostream>

using namespace std;


class Map
{
public:
	Map(SDL_Renderer* renderer);
	~Map();

	void LoadMap();
	void DrawMap(void);
	bool getIsReloading(void);
	int getWhatIsOnTile(int i, int j);
	void transferMap(int m[34][61]);
	void setTile(int i, int j, int val);
	int getLevel(void);
	void setLevel(int s);

private:
	SDL_Renderer* renderer;
	SDL_Rect src, dest;
	SDL_Texture* brick;
	SDL_Texture* block;
	SDL_Texture* water;
	SDL_Texture* ladder;
	SDL_Texture* spike;
	SDL_Texture* door;
	SDL_Texture* blueKey;
	SDL_Texture* coinSilver;
	SDL_Texture* gemYellow;

	int map[35][61] = { 0 };
	bool isReloading;
	int level;
};

