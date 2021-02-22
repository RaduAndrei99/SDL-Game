#include "component.h"
#include <SDL.H>
#include "TextureManager.h"

class Background : public Component
{
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
	SDL_Rect srcRect{}, destRect{};

public:
	Background() = default;
	Background(const char* path, SDL_Renderer* renderer);
	void setTex(const char* path);
	void init(void);
	void draw(void);
};