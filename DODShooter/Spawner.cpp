#include "Spawner.h"
#include "SystemManager.h"

void Spawner::Update(float DeltaTime, SystemManager* sysManager)
{
	timer = std::max(0.f, timer-DeltaTime);
	if (timer < 0.0001f)
	{
		sysManager->SpawnEnemyPrefab(position);
		timer = timeBetweenSpawns;
	}
}
