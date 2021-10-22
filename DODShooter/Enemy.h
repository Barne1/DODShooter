#pragma once
#include "HugoMath.h"

class SystemManager;

class Enemy
{
public:
	void Update(float DeltaTime, SystemManager* sysManager);
	Vector2 GetFrameMovement() { return FrameMovement; }
	void SetLastPos(Vector2 pos) { lastPos = pos; } //used to keep track of our position without transform
	static const int size = 75;
protected:
	Vector2 lastPos;
	Vector2 Direction = { 0, 1 };
	Vector2 FrameMovement = Vector2(0, 0);
	float Speed = 100;
	float sideFluctuation = 3;
	float sideSwingSpeed = 0.01f;
	float timeBetweenShots = 1.f;
	float shotTimer;
};