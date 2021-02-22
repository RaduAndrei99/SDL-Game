#include "Game.h"
#include"background.h"
#include <iostream>
#include <fstream>
#include"text.h"
#include "Map.h"
#include "Player.h"
#include"Enemy.h"
#include"Menu.h"
#include"Scoreboard.h"

using namespace std;


extern Map* map=nullptr;

Background* back;

Player* player;

Enemy* enemy[7];

Text* gameOver;

Menu* menu;

Scoreboard* scoreboard;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr)
{
	this->difficulty = 3;
}

Game::~Game()
{
	delete player;
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized" << endl;
		auto window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window != nullptr)
		{
			std::cout << "Window created" << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer != nullptr)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created" << endl;
		}
		isRunning = true;
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		isRunning = false;
	}
	menu = new Menu(renderer);

	map = new Map(renderer);
	map->LoadMap();

	back = new Background("assets/robo.png", renderer);
	back->init();
	player = new Player("assets/idle.png", "assets/headFocus.png", renderer);
	player->init();
	gameOver = new Text("assets/font.ttf", "Game over! Press r to reload.", renderer, 200, 400, 200, 1500, false, {252,209,42,255 });
	gameOver->init();
	scoreboard = new Scoreboard(renderer);

	ifstream f;
	int x, y;
	char path[] = "levelxenemypos.txt";
	path[5] = map->getLevel() + '0';
	f.open(path);//try-catch
	for (int i = 0; i < 7; ++i)
	{
		f >> x;
		f >> y;
		enemy[i] = new Enemy("assets/alienYellow.png",renderer,x,y);
		enemy[i]->init();
	}
	f.close();
	isMenu = true;
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	//cout << "VelocitateX: " << player->getVelocityX() << " VelocitateY: " << player->getVelocityY()<<"\n";

	SDL_Point aux = player->mapCentre();

	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_RIGHT:
			if (player->getLifes()!=0)
			{
				player->setVelocityX(10);
				player->setPressedKey(SDLK_RIGHT);
				player->setIsMoving(true);
				
				if (map->getWhatIsOnTile(aux.y, aux.x) == 5 || map->getWhatIsOnTile(aux.y+1, aux.x) == 5 || map->getWhatIsOnTile(aux.y+2, aux.x) == 5)
				{
					player->setIsOnLadder(true);
				}
				else
				{
					player->setIsOnLadder(false);
				}
			}
			player->setIsGoingRight(true);
			break;

		case SDLK_LEFT:
			if (player->getLifes() != 0)
			{
				player->setVelocityX(-10);
				player->setPressedKey(SDLK_LEFT);
				player->setIsMoving(true);

				if (map->getWhatIsOnTile(aux.y, aux.x) == 5 || map->getWhatIsOnTile(aux.y + 1, aux.x) == 5 || map->getWhatIsOnTile(aux.y + 2, aux.x) == 5)
				{
					player->setIsOnLadder(true);
				}
				else
				{
					player->setIsOnLadder(false);
				}
			}
			player->setIsGoingRight(false);
			break;

		case SDLK_UP:
			if (player->getLifes() != 0)
			{
				if (player->getIsOnPlatform() == true || player->getIsOnLadder() == true)
				{
					if (map->getWhatIsOnTile(aux.y+1, aux.x) == 5 )
					{
						player->setVelocityY(-10);
						player->setIsOnLadder(true);// sunt pe scara
						player->setIsOnPlatform(false);

					}
					else
					{

						player->setVelocityY(-30);//nu sunt pe scara, trebuie sa sar
						player->setIsOnPlatform(false);
						player->setIsOnLadder(false);
					}
					player->setPressedKey(SDLK_UP);
					player->setIsMoving(true);
				}
			}
			break;

		case SDLK_DOWN:
			if (player->getLifes() != 0)
			{
				if (map->getWhatIsOnTile(aux.y, aux.x) == 5 || map->getWhatIsOnTile(aux.y+1, aux.x) == 5 || map->getWhatIsOnTile(aux.y+2, aux.x) == 5)
				{
					player->setVelocityY(10);
					player->setIsMoving(true);
					player->setIsOnLadder(true);
				}
				player->setPressedKey(SDLK_DOWN);
			}
			break;

		case SDLK_ESCAPE:
			this->isMenu = true;
			back->setTex("assets/robo.png");
			break;

		case SDLK_r:
			player->reset();
			map->LoadMap();
			scoreboard->reset();
			player->setLifes(3);
			break;

		}
		break;

	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		case SDLK_RIGHT:
			player->setVelocityX(0);
			player->setIsMoving(false);
			break;

		case SDLK_LEFT:
			player->setVelocityX(0);
			player->setIsMoving(false);
			break;

		case SDLK_UP:
			player->setVelocityY(0);
			player->setIsMoving(false);
			break;

		case SDLK_DOWN:
			player->setVelocityY(0);
			player->setIsMoving(false);

		}
		break;

	default:
		break;
	}

	SDL_Point enem[7];
	for (int i = 0; i < difficulty; ++i)
	{
		enem[i] = enemy[i]->mapCentre();
	}

	player->checkEnemyCollision(enem,this->difficulty);
	if (player->getLifes() == 0)
	{
		scoreboard->reset();
	}

	if (player->getDestRect().x > 1880)
	{
		map->setLevel(2);
		map->LoadMap();
		player->reset();
	}

}


void Game::update() const
{
	char path1[] = "assets/x.png";//pt. mers la dreapta
	char path2[] = "assets/xb.png";//pt. mers la stanga
	char path3[] = "assets/ux.png";//pt. animatia de pe scara

	if (map->getIsReloading()==true)
	{
		map->LoadMap();
	}
	else
	{/*
		if (player->getVelocity().x == 0 && player->getVelocity().y == 0)
		{
			if (player->getIsOnLadder() == true)
			{
				path3[8] = player->getClimbAnim() + '0';
				player->setTex(path3);
			}
			else
			{
				player->setTex("assets/idle.png");
			}
		}
		else
		{
			switch (player->getPressedKey())
			{
			case SDLK_RIGHT:
			{
				path1[7] = player->getNext() + '0';
				player->setTex(path1);
				player->setNext();

				break;
			}

			case SDLK_LEFT:
			{
				path2[7] = player->getNext() + '0';
				player->setTex(path2);
				player->setNext();

				break;
			}

			case SDLK_UP:
			{
				if (player->getIsOnLadder() == true)
				{
					path3[8] = player->getClimbAnim() + '0';
					player->setClimbAnim();
					player->setTex(path3);
					SDL_Delay(60);
				}
				else
				{
					if (player->getIsGoingRight() == true)
					{
						player->setTex("assets/jump1.png");
					}
					else
					{
						player->setTex("assets/jump2.png");
					}
				}

				break;
			}

			case SDLK_DOWN:
			{
				if (player->getIsOnLadder() == true)
				{
					path3[8] = player->getClimbAnim() + '0';
					player->setClimbAnim();
					player->setTex(path3);
					SDL_Delay(60);
				}

				break;
			}

			}
		}

		*/

		if (player->getVelocity().x > 0)
		{
			path1[7] = player->getNext() + '0';
			player->setTex(path1);
			player->setNext();
		}

		if(player->getVelocity().x<0)
		{
			path2[7] = player->getNext() + '0';
			player->setTex(path2);
			player->setNext();
		}

		if ((player->getVelocity().y > 0 && player->getIsOnLadder()==true) || (player->getVelocity().y < 0 && player->getIsOnLadder() == true))
		{
			path3[8] = player->getClimbAnim() + '0';
			player->setClimbAnim();
			player->setTex(path3);
			SDL_Delay(60);
		}

		if (player->getDamaged() == true)
		{
			player->setTex("assets/hit.png");
			player->setDamaged(false);
		}

		if (player->getVelocity().x == 0 && player->getVelocity().y == 0)
		{
			player->setTex("assets/idle.png");
		}

		if (player->getLifes() == 0)
		{
			player->setTex("assets/dead.png");
			player->setVelocityX(0);

		}


		scoreboard->addToScore(player->pickItems());
		scoreboard->convertScore();
		player->openBlue();
	
		player->processPos();
		
		for (int i = 0; i < this->difficulty; ++i)
		{
			enemy[i]->processPos();
		}

		player->processGravity();

	}

	//afisare(lvl);
}

void Game::render() const
{
	SDL_RenderClear(renderer);

	back->draw();
	map->DrawMap();
	player->draw();

	for (int i = 0; i<this->difficulty; ++i)
	{
		enemy[i]->draw();
	}
	scoreboard->draw();

	/*
	enemy[0]->draw();
	enemy[1]->draw();
	enemy[2]->draw();
	*/
	SDL_SetRenderDrawColor(renderer, 255, 40, 100, 255); 
	
	SDL_Rect rect;
	SDL_Point aux =player->mapPos();
	rect.w = 32;
	rect.h = 32;

	//cout << aux.x << " " << aux.y << "\n";
	//cout << player->getDestRect().x << " " << player->getDestRect().y << "\n\n";


	rect.x = (aux.x) * 32;
	rect.y = (aux.y) * 32;

	SDL_RenderDrawRect(renderer, &rect);
	

	//rect.x = (aux.x) * 32;;
	//rect.y = (aux.y) * 32;

	//SDL_RenderDrawRect(renderer, &rect);

	//SDL_RenderDrawPoint(renderer,player->getDestRect().x+40, player->getDestRect().y+45);

	/*

	rect.x = (aux.x + 1) * 32;
	rect.y = (aux.y + 0) * 32;
	rect.w = 32;
	rect.h = 32;
	SDL_RenderDrawRect(renderer, &rect);

	rect.x = (aux.x + 2) * 32;
	rect.y = (aux.y + 1) * 32;
	SDL_RenderDrawRect(renderer, &rect);

	rect.x = (aux.x + 2) * 32;
	rect.y = (aux.y + 2) * 32;
	SDL_RenderDrawRect(renderer, &rect);

	rect.x = (aux.x + 0) * 32;
	rect.y = (aux.y + 1) * 32;
	SDL_RenderDrawRect(renderer, &rect);

	rect.x = (aux.x + 0) * 32;
	rect.y = (aux.y + 2) * 32;
	SDL_RenderDrawRect(renderer, &rect);
*/
	if (player->getLifes() == 0)
	{
		gameOver->draw();
	}

	SDL_RenderPresent(renderer);
}

bool Game::getIsMenu(void)
{
	return this->isMenu;
}

void Game::clean() const
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_QUIT;
	std::cout << "Game cleaned" << std::endl;
}


void Game::handleAndDrawMenu(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{

			case SDLK_DOWN:
			{
				(*menu)++;
				break;
			}

			case SDLK_UP:
			{
				(*menu)--;
				break;
			}

			case SDLK_RETURN:
			{
				if (menu->getIsDiff() == false)
				{
					if (menu->getSelected() == 0)
					{
						this->isMenu = false;
						back->setTex("assets/back.png");
					}
					else
					{
						if (menu->getSelected() == 1)
						{
							menu->setIsDiff(true);
						}
						else
						{
							isRunning = false;
						}
					}
				}
				else
				{
					if (menu->getSelected() == 0)
					{
						this->difficulty = 3;
						for (int i = 0; i < difficulty; ++i)
						{
							enemy[i]->setVelocityX(5);
						}
					}
					else
					{
						if (menu->getSelected() == 1)
						{
							this->difficulty = 4;
							for (int i = 0; i < difficulty; ++i)
							{
								enemy[i]->setVelocityX(10);
							}
						}
						else
						{
							this->difficulty = 5;
							for (int i = 0; i < difficulty; ++i)
							{
								enemy[i]->setVelocityX(15);
							}
						}
					}

					menu->setIsDiff(false);
				}
				break;
			}
		}
	}


	SDL_RenderClear(renderer);

	back->draw();
	menu->draw();

	SDL_RenderPresent(renderer);
}