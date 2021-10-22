#pragma once
#include "System.h"
#include "Keyboardstate.h"
#include "ComponentManager.h"

class SystemManager;

class SystemUpdate : public System
{
public:
	SystemUpdate();
	void UpdateEntities(float DeltaTime, MyKeyBoardState keyboardState, SystemManager* sysManager, ComponentManager* compManager);
	void EntitySignatureChanged(Entity e, Signature s) override;
};

