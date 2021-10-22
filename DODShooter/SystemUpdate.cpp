#include "SystemUpdate.h"
#include "SystemManager.h"

SystemUpdate::SystemUpdate()
{
	systemSignature = COMPONENT_PLAYER | COMPONENT_BULLET | COMPONENT_ENEMY | COMPONENT_SPAWNER; //override for multiple types
}

void SystemUpdate::UpdateEntities(float DeltaTime, MyKeyBoardState keyboardState, SystemManager* sysManager, ComponentManager* compManager)
{
	for (Entity e : entities)
	{
		Player* player = nullptr;
		player = compManager->GetComponent<Player>(e);
		if (player != nullptr)
		{
			player->Update(DeltaTime, keyboardState, sysManager);
			continue;
		}
		//else
		Bullet* b = nullptr;
		b = compManager->GetComponent<Bullet>(e);
		if (b != nullptr)
		{
			b->Update(DeltaTime);
			continue;
		}
		//else
		Enemy* enemy = nullptr;
		enemy = compManager->GetComponent<Enemy>(e);
		if (enemy != nullptr)
		{
			enemy->Update(DeltaTime, sysManager);
			continue;
		}
		Spawner* spawner = nullptr;
		spawner = compManager->GetComponent<Spawner>(e);
		if (spawner != nullptr)
		{
			spawner->Update(DeltaTime, sysManager);
			continue;
		}
	}
}

void SystemUpdate::EntitySignatureChanged(Entity e, Signature s)
{
	if (s & systemSignature) //override for multiple types
	{
		entities.insert(e); 
	}
	else
	{
		entities.erase(e);
	}
}
