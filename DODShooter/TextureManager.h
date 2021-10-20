#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class TextureManager
{
//Data in regard to paths and enums
#pragma region
	public:
		enum TextureTypes
		{
			TEXTURE_BACKGROUND,
			TEXTURE_PLAYER,
			TEXTURE_TOTAL
		};
	protected:
		const std::string mainPath = "Images\\";
		const std::string paths[TEXTURE_TOTAL] = {
			"background.png",
			"player.png"
		};
#pragma endregion This region contains paths and enums corresponding to them
//Class data
public:
	TextureManager();
	~TextureManager();

	//Fills the array with textures
	bool LoadTextures(SDL_Renderer* renderer);

	SDL_Texture* GetTexture(TextureTypes textureType) const;

protected:
	SDL_Texture* textures[TEXTURE_TOTAL];
};

