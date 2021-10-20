#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for (int i = 0; i < TEXTURE_TOTAL; i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
}

bool TextureManager::LoadTextures(SDL_Renderer* renderer)
{
    for (int i = 0; i < TEXTURE_TOTAL; i++)
    {
        const std::string path = mainPath + paths[i];
        SDL_Surface* tempSurface = IMG_Load(path.c_str());
        if(tempSurface == nullptr)
        {
            printf("Could not load image %s! IMG Error: %s\n", path.c_str(), IMG_GetError());
            return false;
        }
        textures[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }

    return true;
}

SDL_Texture* TextureManager::GetTexture(TextureTypes textureType) const
{
    return textures[textureType];
}
