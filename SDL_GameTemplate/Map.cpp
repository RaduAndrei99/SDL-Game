#include "Map.h"
#include "TextureManager.h"
#include<iostream>
#include <SDL.h>
#include<fstream>

using namespace std;


Map::Map(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	brick = TextureManager::LoadTexture("assets/brick.png", renderer);
	block = TextureManager::LoadTexture("assets/block.png", renderer);
	water = TextureManager::LoadTexture("assets/water.png", renderer);
	ladder = TextureManager::LoadTexture("assets/Plat/Base_pack/Tiles/ladder_mid.png", renderer);
	spike = TextureManager::LoadTexture("assets/spikes.png", renderer);
	door = TextureManager::LoadTexture("assets/door.png",renderer);
	blueKey = TextureManager::LoadTexture("assets/hud_keyBlue.png",renderer);
	coinSilver= TextureManager::LoadTexture("assets/coinSilver.png", renderer);
	gemYellow = TextureManager::LoadTexture("assets/gemYellow.png", renderer);

	src.x = 0;
	src.y = 0;

	dest.x = 0;
	dest.y = 0;

	dest.w = 32;
	dest.h = 32;
	isReloading = false;

	level = 1;

}

Map::~Map()
{

}

void Map::LoadMap(void)
{
	ifstream f;
	char path[] = "levelx.txt";
	path[5] = this->level + '0';
	f.open(path);//try-catch

	for (int row = 0; row < 34; row++)
	{
		for (int column = 0; column < 61; column++)
		{
			f>>map[row][column];
			//cout << map[row][column] << " ";
		}
		//cout << "\n";
	}
	this -> isReloading = false;

	f.close();
}

void Map::DrawMap()
{
	int type = 0;
	for (int row = 0; row < 34; row++)
	{
		for (int column = 0; column < 61; column++)
		{
			type = map[row][column];

			dest.x = column * 32;
			dest.y = row * 32;
			switch (type)
			{
			case 1:
			{
				TextureManager::Draw(block, src, dest, renderer);
				break;
			}
			case 2:
			{
				TextureManager::Draw(brick, src, dest, renderer);
				break;
			}
			case 3:
			{
				TextureManager::Draw(door, src, dest, renderer);
				break;
			}
			case 4:
			{
				TextureManager::Draw(blueKey,src,dest,renderer);
				break;
			}
			case 6:
			{
				TextureManager::Draw(gemYellow, src, dest, renderer);
				break;
			}
			case 5:
			{
				TextureManager::Draw(ladder, src, dest, renderer);
				break;
			}
			case 10:
			{
				TextureManager::Draw(spike, src, dest, renderer);
				break;
			}

			case 20:
			{
				TextureManager::Draw(coinSilver, src, dest, renderer);
				break;
			}

			default:
				break;
			}
		}
	}
}

bool Map::getIsReloading(void)
{
	return this->isReloading;
}

int Map::getWhatIsOnTile(int i, int j)
{
	return map[i][j];
}

void Map::transferMap(int m[34][61])
{
	for (int row = 0; row < 34; row++)
	{
		for (int column = 0; column < 61; column++)
		{
			m[row][column] = this->map[row][column];
		}
	}
}

void Map::setTile(int i, int j, int val)
{
	this->map[i][j] = val;
}

int Map:: getLevel(void)
{
	return level;
}

void Map::setLevel(int s)
{
	this->level = s;
}