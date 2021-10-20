#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "TextureManager.h"
#include "EventManager.h"
#include "ECSConstants.h"
#include "SystemRender.h"
#include "EntityIDManager.h"
#include "ComponentManager.h"

const char* windowTitle = "DODShooter Hugo Lindroth";

//This struct is a container for important data often passed
struct SDLData
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Vector2 dimensions = {640, 480};
};

//forward declarations
bool initSDL(SDLData* out_data);
void close(SDLData* out_data);

int main(int argc, char* argv[])
{
	SDLData coreData;
	SDL_Event e;
	TextureManager textureManager;
	EventManager eventManager;

	//Ecs
	EntityIDManager entityManager;
	ComponentManager compManager;
	SystemRender renderSystem;

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

	//TEST CODE
	Entity ent = entityManager.CreateEntity();
	Signature s = COMPONENT_TRANSFORM | COMPONENT_TEXTURE;
	entityManager.SetSignature(ent, s);
	
	Transform tf;
	tf.pos = Vector2(coreData.dimensions.x * 0.5f, coreData.dimensions.y * 0.5f);
	tf.scale = Vector2{0.5f, 0.5f};
	compManager.SetComponent<Transform>(ent, tf);

	StaticTexture tex;
	SDL_Texture* textureTemp = textureManager.GetTexture(TextureManager::TEXTURE_PLAYER);
	int tw, th;
	Uint32 format; int access;
	SDL_QueryTexture(textureTemp, &format, &access, &tw, &th);

	tex.Init(textureTemp, {0,0,tw,th});

	compManager.SetComponent<StaticTexture>(ent, tex);
	tex.srcRect = {0,0,tw,th};

	renderSystem.EntitySignatureChanged(ent, s);


#pragma region
	
	bool quit = false;
	while (!quit)
	{
		//Poll event queue/stack
		while (SDL_PollEvent(&e) > 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) //Bitwise operations to check if e.type is keydown or keyup 
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
					break;
				}
				else
					eventManager.HandleKeyEvent(&e);
			}
			
			renderSystem.RenderEntities(coreData.dimensions, coreData.renderer, &compManager);
			//SDL_RenderCopy(coreData.renderer, textureManager.GetTexture(TextureManager::TEXTURE_BACKGROUND), nullptr, nullptr);
			SDL_RenderPresent(coreData.renderer);
		}
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