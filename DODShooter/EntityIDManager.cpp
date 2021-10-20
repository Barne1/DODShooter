#include "EntityIDManager.h"
#include <stdio.h>

EntityIDManager::EntityIDManager()
{
	for (Entity i = 0; i < MAX_ENTITIES; i++)
	{
		unusedIDs.push(i);
	}
}

Entity EntityIDManager::CreateEntity()
{
	if (currentEntities >= MAX_ENTITIES)
	{
		printf("Entitiy limit reached!\n");
	}
	Entity id = unusedIDs.front();
	unusedIDs.pop();
	currentEntities++;

	return id;
}

void EntityIDManager::DestroyEntity(Entity e)
{
	if (e < MAX_ENTITIES)
	{
		signatures[e] = 0;
		currentEntities--;
		unusedIDs.push(e);
		return;
	}
	printf("Entity %d out of range %d! Cannot destroy\n", e, MAX_ENTITIES);
}

void EntityIDManager::SetSignature(Entity e, Signature s)
{
	if (e < MAX_ENTITIES)
	{
		signatures[e] = s;
		return;
	}
	printf("Entity %d out of range %d! Cannot set\n", e, MAX_ENTITIES);
}

Signature EntityIDManager::GetSignature(Entity e)
{
	if(e < MAX_ENTITIES)
	{
		return signatures[e];
	}
	//else
	printf("Entity %d out of range %d! Cannot get\n", e, MAX_ENTITIES);
	return 0;
}
