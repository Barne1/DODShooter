#pragma once
#include "HugoMath.h"

class SystemManager;

class Spawner
{
public:
	void Init(Vector2 pos) { position = pos; };
	void Update(float DeltaTime, SystemManager* sysManager);
protected:
	Vector2 position;
	float timer = 0;
	float timeBetweenSpawns = 2.5f;
};

