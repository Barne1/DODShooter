#pragma once
#include "HugoMath.h"

enum class Allegiance
{
	PLAYER,
	ENEMY,
	MAX
};

class Bullet
{
public:
	void BulletInit(Allegiance a);

	void Update(float DeltaTime);

	Allegiance GetAllegiance() { return allegiance; }
	Vector2 GetDirection() { return direction; }
	float GetSpeed() { return speed; }
	Vector2 GetFrameMovement();
	const static int size = 25;

protected:
	Allegiance allegiance;
	Vector2 direction = {0, 1};
	Vector2 frameMovement = {0,0};
	float speed = 1000;
};