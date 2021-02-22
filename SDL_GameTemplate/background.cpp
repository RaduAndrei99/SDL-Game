#include"background.h"

Background::Background(const char* path, SDL_Renderer* renderer) : renderer(renderer)
{
	setTex(path);
}

void Background::setTex(const char* path)
{
	texture = TextureManager::LoadTexture(path, renderer);
}

void Background::init(void)
{
	this->destRect.x = 0;
	this->destRect.y = 0;

	this->destRect.w = 1920;
	this->destRect.h = 1080;
}

void Background::draw(void)
{
	TextureManager::Draw(texture, srcRect, destRect, renderer);
}
