#include "SystemMovement.h"
#include "SystemManager.h"
#include <math.h>

SystemMovement::SystemMovement()
{
	systemSignature = COMPONENT_PLAYER | COMPONENT_BULLET | COMPONENT_ENEMY;  //special override below that turns out true if we have either a bullet or a player
}

void SystemMovement::Move(Vector2 dimensions, ComponentManager* compManager, SystemManager* sysManager)
{
	for (Entity e : entities)
	{
		Transform* t = nullptr;
		t = compManager->GetComponent<Transform>(e);
		Player* p = nullptr;
		p = compManager->GetComponent<Player>(e);
		if (p != nullptr)
		{
			float newX = t->pos.x;
			float newY = t->pos.y;
			newX += p->GetFrameMovement().x;
			newY += p->GetFrameMovement().y;
			 
			newX = std::max(0.f, newX);
			newX = std::min(dimensions.x, newX);
			newY = std::max(0.f, newY);
			newY = std::min(dimensions.y, newY);

			t->pos = Vector2(newX, newY);
			p->SetLastPos(t->pos);
			continue;
		}
		//else
		Bullet* b = nullptr;
		b = compManager->GetComponent<Bullet>(e);
		if (b != nullptr)
		{
			float newX = t->pos.x;
			float newY = t->pos.y;
			newX += b->GetFrameMovement().x;
			newY += b->GetFrameMovement().y;

			int boundsBuffer = 100;
			int lowerXBounds = 0 - boundsBuffer;
			int lowerYBounds = 0 - boundsBuffer;
			int upperXBounds = dimensions.x + boundsBuffer;
			int upperYBounds = dimensions.y + boundsBuffer;

			bool outOfBounds = (
				newX < lowerXBounds ||
				newX > upperXBounds ||
				newY < lowerYBounds ||
				newY > upperYBounds );

			if (outOfBounds)
			{
				sysManager->MarkForDestroy(e);
				continue;
			}
			//else

			t->pos = Vector2(newX, newY);
			continue;
		}
		//else
		Enemy* enemy = nullptr;
		enemy = compManager->GetComponent<Enemy>(e);
		if (enemy != nullptr)
		{
			float newX = t->pos.x;
			float newY = t->pos.y;
			newX += enemy->GetFrameMovement().x;
			newY += enemy->GetFrameMovement().y;

			int boundsBuffer = 500;
			int lowerXBounds = 0 - boundsBuffer;
			int lowerYBounds = 0 - boundsBuffer;
			int upperXBounds = dimensions.x + boundsBuffer;
			int upperYBounds = dimensions.y + boundsBuffer;

			bool outOfBounds = (
				newX < lowerXBounds ||
				newX > upperXBounds ||
				newY < lowerYBounds ||
				newY > upperYBounds);

			if (outOfBounds)
			{
				sysManager->MarkForDestroy(e);
				continue;
			}
			//else

			t->pos = Vector2(newX, newY);
			enemy->SetLastPos(t->pos);
			continue;
		}
	}
}

void SystemMovement::EntitySignatureChanged(Entity e, Signature s)
{
	if (s & systemSignature) //special override that turns out true if we have either a bullet or a player
	{
		entities.insert(e);
	}
	else
	{
		entities.erase(e);
	}
}
