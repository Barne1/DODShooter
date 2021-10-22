#pragma once
#include "System.h"
#include "CircleCollider.h"
#include "EntityIDManager.h"

class SystemManager;

class CollisionSystem : public System
{
public:
	CollisionSystem();
	void HandleCollisions(ComponentManager* compMan, EntityIDManager* entityManager, SystemManager* sysManager);
};

