#pragma once
#include "SDL.h"
#include "ECSConstants.h"
#include "ComponentManager.h"
#include <set>

class System
{	
public:
	void EntityDestroyed(Entity e);
	void EntitySignatureChanged(Entity e, Signature s);
protected:
	Signature systemSignature; //The signature for all components required for the system to allow an Entity
	std::set<Entity> entities;
};

