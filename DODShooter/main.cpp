#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "TextureManager.h"
#include "Keyboardstate.h"
#include "HugoMath.h"
#include "SystemManager.h"

const char* windowTitle = "DODShooter Hugo Lindroth";

//This struct is a container for important data often passed
struct SDLData
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Vector2 dimensions = {1024, 768};
};

//forward declarations
bool initSDL(SDLData* out_data);
void close(SDLData* out_data);

int main(int argc, char* argv[])
{
	SDLData coreData;
	SDL_Event myEvent;
	TextureManager textureManager;
	SystemManager systemManager(&textureManager);

	//Ecs

#pragma region
	if (!initSDL(&coreData))
	{
		printf("Could not initialize SDL!\n");
		return -1;
	}
	if (!textureManager.LoadTextures(coreData.renderer))
	{
		printf("Could not load Textures!\n"); //no return here since some textures might not be needed
	}
#pragma endregion Initalizations

	Entity background = systemManager.CreateEntity();
	systemManager.AddTransformComponent(background, { coreData.dimensions.x, coreData.dimensions.y}, { coreData.dimensions.x * 0.5f, coreData.dimensions.y * 0.5f });
	systemManager.AddTextureComponent(background, TextureManager::TEXTURE_BACKGROUND, nullptr, SDL_BLENDMODE_NONE);

	Entity player = systemManager.CreateEntity();
	systemManager.AddTransformComponent(player, {100,100}, {coreData.dimensions.x * 0.5f, coreData.dimensions.y * 0.5f });
	systemManager.AddTextureComponent(player, TextureManager::TEXTURE_PLAYER, nullptr, SDL_BLENDMODE_BLEND);
	systemManager.AddPlayerComponent(player);
	systemManager.AddCircleColliderComponent(player, 50, COLLIDER_PLAYER);

	systemManager.SpawnSpawner({ coreData.dimensions.x * 0.2f, coreData.dimensions.y * 0.01f});
	systemManager.SpawnSpawner({ coreData.dimensions.x * 0.5f, coreData.dimensions.y * 0.01f});
	systemManager.SpawnSpawner({ coreData.dimensions.x * 0.8f, coreData.dimensions.y * 0.01f});

#pragma region
	
	bool quit = false;
	Uint32 previousTime = 0;
	MyKeyBoardState keyboardState;
	while (!quit)
	{
		Uint32 time = SDL_GetTicks();
		float DeltaTime = (time - previousTime) * 0.001;
		previousTime = time;

		//Poll event queue/stack
		while (SDL_PollEvent(&myEvent) > 0)
		{
			if (myEvent.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			else if(myEvent.type == SDL_KEYDOWN) //Bitwise operations to check if e.type is keydown or keyup 
			{
				if (myEvent.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
					break;
				}
				else
				{
					switch (myEvent.key.keysym.sym)
					{
					default:
						break;
					case SDLK_w: keyboardState.forward = true; break;
					case SDLK_s: keyboardState.backward = true; break;
					case SDLK_d: keyboardState.right = true; break;
					case SDLK_a: keyboardState.left = true; break;
					case SDLK_SPACE: keyboardState.space = true; break;
					}
				}
			}
			else if (myEvent.type == SDL_KEYUP) //Bitwise operations to check if e.type is keydown or keyup 
			{
				switch (myEvent.key.keysym.sym)
				{
				default:
					break;
				case SDLK_w: keyboardState.forward = false; break;
				case SDLK_s: keyboardState.backward = false; break;
				case SDLK_d: keyboardState.right = false; break;
				case SDLK_a: keyboardState.left = false; break;
				case SDLK_SPACE: keyboardState.space = false; break;
				}
			}
			
		}
		systemManager.UpdateEntities(DeltaTime, keyboardState);
		systemManager.Move(coreData.dimensions);
		systemManager.HandleCollision();
		systemManager.DestroyAllPending();

		systemManager.Render(coreData.dimensions, coreData.renderer);
		//SDL_RenderCopy(coreData.renderer, textureManager.GetTexture(TextureManager::TEXTURE_BACKGROUND), nullptr, nullptr);
		SDL_RenderPresent(coreData.renderer);
	}
#pragma endregion Main loop

	close(&coreData);

	return 0;
}

bool initSDL(SDLData* out_data)
{
	//Main SDL systems init
	Uint32 initFlags = SDL_INIT_VIDEO; //Events implied to init with video
	if (SDL_Init(initFlags) < 0)
	{
		printf("Could not initalize SDL flags! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//SDL_image init
	int flags = IMG_INIT_PNG | IMG_INIT_JPG;
	int flagsInitialized = IMG_Init(flags); //IMG_Init returns flags successfully initialized, should be the same as the flags we passed in
	if (flagsInitialized != flags)
	{
		printf("Could not initialize SDL_Image! IMG Error: %s\n", IMG_GetError());
		return false;
	}

	//SDL Window init
	Uint32 windowFlags = 0;
	SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, out_data->dimensions.x, out_data->dimensions.y, windowFlags);
	if (window == nullptr)
	{
		printf("Could not create window! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	//SDL Renderer init
	Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC; //using hardware acceleration and Vsync
	int index = -1; //-1 means init first renderer to support requested flags
	SDL_Renderer* renderer = SDL_CreateRenderer(window, index, rendererFlags);
	if (renderer == nullptr)
	{
		printf("Could not create renderer! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	out_data->window = window;
	out_data->renderer = renderer;

	return true;
}

void close(SDLData* out_data)
{
	SDL_DestroyWindow(out_data->window);
	out_data->window = nullptr;
	SDL_DestroyRenderer(out_data->renderer);
	out_data->renderer = nullptr;
	
	//Texture deallocation happens in destructor

	SDL_Quit();
	IMG_Quit();
}