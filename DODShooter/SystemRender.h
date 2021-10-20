#pragma once
#include "System.h"

class SystemRender : public System
{
public:
	SystemRender();
	void RenderEntities(Vector2 ScreenDimensions, SDL_Renderer* renderer, ComponentManager* compMan);
};

