#include "CollisionSystem.h"
#include "SystemManager.h"

CollisionSystem::CollisionSystem()
{
	systemSignature = COMPONENT_COLLIDER;
}

void CollisionSystem::HandleCollisions(ComponentManager* compMan, EntityIDManager* entityManager, SystemManager* sysManager)
{
	for (Entity e : entities)
	{
		CircleCollider* mainCollider = compMan->GetComponent<CircleCollider>(e);
		Vector2 mainPos = compMan->GetComponent<Transform>(e)->pos;
		bool collisionHappened = false;

		//TODO: look into iterators to only iterate over not visited entities, use set::find?
		for (Entity otherEntity : entities)
		{
			if (otherEntity == e)
				continue;

			CircleCollider* other = compMan->GetComponent<CircleCollider>(otherEntity);
			Vector2 otherPos = compMan->GetComponent<Transform>(otherEntity)->pos;

			float overlapDistance = mainCollider->radius + other->radius;
			float overlapDistanceSqr = overlapDistance * overlapDistance; //sqr because sqrt is expensive

			float distanceBetweenEntitiesSqr = (otherPos - mainPos).GetLengthSqr();

			if(distanceBetweenEntitiesSqr < overlapDistanceSqr)
			{
				mainCollider->Collide(other->type);
				collisionHappened = true;
			}
		}

		if (!collisionHappened)
			continue;

		//After we found all collisions, handle them
		Uint8 componentsWithCollisions = COMPONENT_PLAYER | COMPONENT_BULLET | COMPONENT_ENEMY;
		Signature entitySignature = entityManager->GetSignature(e);
		if (entitySignature & componentsWithCollisions)
		{
			int* collisions = mainCollider->Collisions;
			if (entitySignature & COMPONENT_PLAYER)
			{
				Player* p = compMan->GetComponent<Player>(e);
				for (int i = 0; i < COLLIDER_TYPE_MAX; i++)
				{
					if (i == COLLIDER_PLAYER || i == COLLIDER_PLAYER_BULLET)
						continue;

					ColliderType type = static_cast<ColliderType>(i);
					int amountOfCollisions = collisions[i];
					for (int j = 0; j < amountOfCollisions; j++)
					{
						sysManager->MarkForDestroy(e);
						printf("\n\nGAME OVER!\n\n");
					}
					collisions[i] = 0; //reset collisions
				}
			}
			else if (entitySignature & COMPONENT_BULLET)
			{
				Bullet* b = compMan->GetComponent<Bullet>(e);
				for (int i = 0; i < COLLIDER_TYPE_MAX; i++)
				{
					if(b->GetAllegiance() == Allegiance::PLAYER && (i == COLLIDER_PLAYER || i == COLLIDER_PLAYER_BULLET))
						continue;
					if (b->GetAllegiance() == Allegiance::ENEMY && (i == COLLIDER_ENEMY || i == COLLIDER_ENEMY_BULLET))
						continue;

					ColliderType type = static_cast<ColliderType>(i);
					int amountOfCollisions = collisions[i];
					for (int j = 0; j < amountOfCollisions; j++)
					{
						sysManager->MarkForDestroy(e);
					}
					collisions[i] = 0; //reset collisions
				}
			}
			else if (entitySignature & COMPONENT_ENEMY)
			{
				Enemy* enemy = compMan->GetComponent<Enemy>(e);
				for (int i = 0; i < COLLIDER_TYPE_MAX; i++)
				{
					if (i == COLLIDER_ENEMY || i == COLLIDER_ENEMY_BULLET)
						continue;

					ColliderType type = static_cast<ColliderType>(i);
					int amountOfCollisions = collisions[i];
					for (int j = 0; j < amountOfCollisions; j++)
					{
						sysManager->MarkForDestroy(e);
					}
					collisions[i] = 0; //reset collisions
				}
			}
		}
	}
}
