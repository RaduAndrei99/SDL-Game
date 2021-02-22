#include "Player.h"
#include "map.h"
#include "TextureManager.h"
#include<iostream>
#include <math.h>
#include <fstream>
#include "Game.h"

using namespace std;

extern Map* map;

Player::Player(const char* path, const char* pathForHeads, SDL_Renderer* renderer) : renderer(renderer)
{
	setTex(path);
	setHeadsTex(pathForHeads);
}

void Player::setTex(const char* path)
{
	texture = TextureManager::LoadTexture(path, renderer);
}

void Player::setHeadsTex(const char* path)
{
	headsTexture = TextureManager::LoadTexture(path, renderer);
}


void Player::init()
{
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.h = 0;
	srcRect.w = 0;

	destRect.x = 50;
	destRect.y = 864;

	destRect.w = 80;
	destRect.h = 90;

	velocity.x = 0;
	velocity.y = 0;

	pressedKey = 0;
	next = 1;
	climbAnim = false;
	isOnPlatform = true;
	orientation = 0;
	isMoving = false;
	isOnLadder = false;
	lifes = 3;
	damaged = false;
	isGoingRight = false;
	blueKey=false;

	headsRect.x = 20;
	headsRect.y = 20;
	headsRect.w = 64;
	headsRect.h = 64;
}

void Player::draw()
{
	TextureManager::Draw(texture, srcRect, destRect, renderer);
	int a = 0;
	int aux = headsRect.x;
	for (unsigned int i = 0; i < lifes; ++i)
	{
		headsRect.x = aux + a;
		TextureManager::Draw(headsTexture, srcRect, headsRect, renderer);
		a = a + 80;
	}
	headsRect.x = 20;
}

int Player::getNext(void)
{
	return this->next;
}

void Player::setNext(void)
{
	if (next == 8)
	{
		next = 1;
	}
	else
	{
		next++;
	}
}

void Player::resetNext(void)
{
	this->next = 1;
}
void Player::setVelocityX(int x)
{
	this->velocity.x = x;
}
void Player::setVelocityY(int y)
{
	this->velocity.y = y;
}

void Player::processPos(void)
{
	this->destRect.x = this->destRect.x + this->velocity.x;

	if (this->destRect.x < -40 || this->checkColision() == true)
	{
		this->destRect.x = this->destRect.x - this->velocity.x;
	}

	this->destRect.y = this->destRect.y + this->velocity.y;

	SDL_Point aux = this->mapPos();

	if ((this->destRect.y > 1080 || this->destRect.y < 0 || this->checkColision() == true))
	{
		this->destRect.y = this->destRect.y - this->velocity.y;
	}
	else
	{
		if ((map->getWhatIsOnTile(aux.y + 3, aux.x) == 1 && this->isOnLadder == false)|| (map->getWhatIsOnTile(aux.y + 3, aux.x) == 5 && map->getWhatIsOnTile(aux.y + 2, aux.x) != 5))
		{
			this->destRect.y = (aux.y * 32)+5;
		}
	}

}

void Player::processGravity(void)
{
	this->checkIfIsOnPlatform();
	if (this->isOnPlatform == false && this->isOnLadder==false)
	{
		if(this->velocity.y<30)
			this->velocity.y +=5;
	}
	else
	{
		if(this->isOnLadder==false)
			this->velocity.y = 0;
	}
}

SDL_Point Player::getVelocity(void)
{
	return this->velocity;
}

void Player::checkIfIsOnPlatform(void)
{
	SDL_Point aux = this->mapPos();

	if (map->getWhatIsOnTile(aux.y + 3, aux.x) == 1)
	{
		this->isOnPlatform = true;
		return;
	}

	if (map->getWhatIsOnTile(aux.y + 3, aux.x) == 5)
	{
		this->isOnPlatform = true;
		return;
	}

	if (map->getWhatIsOnTile(aux.y + 3, aux.x-1) == 1 && this->isGoingRight == true)
	{
		this->isOnPlatform = true;
		return;
	}

	if (map->getWhatIsOnTile(aux.y + 3, aux.x+1) == 1 && this->isGoingRight == false)
	{
		this->isOnPlatform = true;
		return;
	}

	if (map->getWhatIsOnTile(aux.y + 3, aux.x) == 2)
	{
		this->isOnPlatform = true;
		return;
	}
	

	this->isOnPlatform = false;

}

SDL_Point Player::mapPos(void)
{
	SDL_Point ret;

	if (this->isGoingRight == false)
	{
		ret.x = (int)(this->getDestRect().x+25) / 32;
	}
	else
	{
		ret.x = (int)(this->getDestRect().x+60) / 32;
	}

	ret.y=(int)(this->getDestRect().y + 20) / 32;

	return ret;
}

SDL_Point Player::mapCentre(void)
{
	SDL_Point ret;
	ret.x = (int)(this->getDestRect().x+40) / 32;
	ret.y = (int)(this->getDestRect().y+45) / 32;

	return ret;
}


void Player::setIsOnPlatform(bool s)
{
	this->isOnPlatform = s;
}

void Player::setIsOnLadder(bool s)
{
	this->isOnLadder = s;
}
bool Player::getIsOnLadder(void)
{
	return this->isOnLadder;
}

bool Player::getClimbAnim(void)
{
	return this->climbAnim;
}

void Player::setClimbAnim(void)
{
	this->climbAnim = not this->climbAnim;
}

void Player::checkEnemyCollision(SDL_Point enemyPos[],int nr)
{
	if (this->damaged == false)
	{
		SDL_Point aux = this->mapCentre();
		for(int i=0;i<nr;++i)
		{
			if( (aux.x == enemyPos[i].x && aux.y + 1 == enemyPos[i].y) || (aux.x==enemyPos[i].x && aux.y+1==enemyPos[i].y-1) || (aux.x == enemyPos[i].x && aux.y == enemyPos[i].y - 1))
			{
				if (lifes > 1)
				{
					if (this->pressedKey == SDLK_LEFT)
					{
						this->destRect.x += 80;
					}
					else
					{
						this->destRect.x += -80;
					}
				}

				if (lifes != 0)
				{
					lifes--;
				}
				else
				{
					lifes = 0;
				}

				this->damaged = true;
			}
		}

		if (map->getWhatIsOnTile(aux.y + 1, aux.x) == 10)
		{
			if (lifes > 1)
			{
				if (this->pressedKey == SDLK_LEFT)
				{
					this->destRect.x += 80;
				}
				else
				{
					this->destRect.x += -80;
				}
			}

			if (lifes != 0)
			{
				lifes--;
			}
			else
			{
				lifes = 0;
			}

			this->damaged = true;
		}
	}
}

bool Player::checkColision(void)
{
	SDL_Point aux = mapPos();

	if (map->getWhatIsOnTile(aux.y+1, aux.x) == 1)
	{
		return true;
	}

	if (map->getWhatIsOnTile(aux.y + 2, aux.x) == 1)
	{
		return true;
	}
	return false;
}

int Player::pickItems(void)
{
	SDL_Point aux = mapCentre();

	if (map->getWhatIsOnTile(aux.y + 1, aux.x) == 4 )
	{
		map->setTile(aux.y + 1, aux.x, 0);
		this->blueKey = true;
		return 500;
	}

	if (map->getWhatIsOnTile(aux.y + 1, aux.x) == 20)
	{
		map->setTile(aux.y + 1, aux.x, 0);
		return 5;
	}

	if (map->getWhatIsOnTile(aux.y + 1, aux.x) == 6)
	{
		map->setTile(aux.y + 1, aux.x, 0);
		return 200;
	}
	return 0;
}

void Player::openBlue(void)
{
	SDL_Point aux = mapCentre();

	if (map->getWhatIsOnTile(aux.y, aux.x+1) == 3 && this->blueKey==true)
	{
		map->setTile(aux.y, aux.x+1, 0);
		map->setTile(aux.y + 1, aux.x+1, 0);
		map->setTile(aux.y - 1, aux.x+1, 0);
	}

}

bool Player::getIsMoving(void)
{
	return this->isMoving;
}
