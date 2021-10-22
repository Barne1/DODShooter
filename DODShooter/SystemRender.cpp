#include "SystemRender.h"

SystemRender::SystemRender() : System()
{
	systemSignature = COMPONENT_TRANSFORM | COMPONENT_TEXTURE;
}

void SystemRender::RenderEntities(Vector2 ScreenDimensions, SDL_Renderer* renderer, ComponentManager* compMan)
{
	SDL_RenderClear(renderer);

	for (Entity e : entities)
	{
		Transform* t = nullptr;
		t = compMan->GetComponent<Transform>(e);
		StaticTexture* tex = nullptr;
		tex = compMan->GetComponent<StaticTexture>(e);
		int width = t->size.x;
		int height = t->size.y;
		int halfWidth = width * 0.5f;
		int halfHeight = height * 0.5f;

		if ((t->pos.x + halfWidth) < 0 || (t->pos.x - halfWidth) > ScreenDimensions.x) //out of bounds horizontally
		{
			continue;
		}
		if ((t->pos.y + halfHeight) < 0 || (t->pos.y - halfHeight) > ScreenDimensions.y) //out of bounds vertically
		{
			continue;
		}

		SDL_Rect dstRect = {t->pos.x - halfWidth, t->pos.y - halfHeight, width, height};
		SDL_RenderCopy(renderer, tex->texture, tex->srcRect, &dstRect);
	}
}
