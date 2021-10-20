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
		
		Transform* t = compMan->GetComponent<Transform>(e);
		StaticTexture* tex = compMan->GetComponent<StaticTexture>(e);
		int halfWidth = tex->srcRect.w * 0.5f;
		int halfHeight = tex->srcRect.h * 0.5f;

		if(tex == nullptr)
			printf("123");
		if ((t->pos.x + halfWidth) < 0 || (t->pos.x - halfWidth) > ScreenDimensions.x) //out of bounds horizontally
			return;
		if ((t->pos.y + halfHeight) < 0 || (t->pos.y - halfHeight) > ScreenDimensions.y) //out of bounds vertically
			return;

		SDL_Rect dstRect = {t->pos.x - halfWidth, t->pos.y - halfHeight, tex->srcRect.w * t->scale.y, tex->srcRect.h * t->scale.y};
		SDL_RenderCopy(renderer, tex->texture, &tex->srcRect, &dstRect);
	}
}
