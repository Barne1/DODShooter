#pragma once
#include "System.h"

class SystemManager;

class SystemMovement : public System
{
public:
	SystemMovement();
	void Move(Vector2 dimensions, ComponentManager* compManager, SystemManager* sysManager);
	void EntitySignatureChanged(Entity e, Signature s) override;
};

