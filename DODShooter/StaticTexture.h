#pragma once
#include "SDL.h"
#include "HugoMath.h"

/*struct Color
{
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
};*/

struct StaticTexture
{
	SDL_Texture* texture = nullptr;
	SDL_Rect* srcRect;
	//Color tint;

	void Init(SDL_Texture* tex, SDL_Rect* src = nullptr, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND)
	{
		texture = tex;
		srcRect = src;

		SDL_SetTextureBlendMode(texture, blendMode);
	}
};