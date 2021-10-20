#pragma once
#include <queue>
#include "ECSConstants.h"

class EntityIDManager
{
public:
	EntityIDManager();

	Entity CreateEntity();
	void DestroyEntity(Entity e);

	void SetSignature(Entity e, Signature s);
	Signature GetSignature(Entity e);

protected:
	std::queue<Entity> unusedIDs; //available IDs
	Signature signatures[MAX_ENTITIES]; //all signatures for each id, find with enitity ID as index
	Entity currentEntities = 0;
};

