#pragma once
#include "SDL.h"
#include "HugoMath.h"

struct Color
{
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
};

struct StaticTexture
{
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect;
	Color tint;
	SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;

	void Init(SDL_Texture* tex, SDL_Rect src)
	{
		texture = tex;
		srcRect = src;
	}
};