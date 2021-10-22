#include "Bullet.h"
#include <stdio.h>

void Bullet::BulletInit(Allegiance a)
{
	allegiance = a;
	if (allegiance == Allegiance::PLAYER)
		direction = Vector2(0, -1);
	else
		direction = Vector2(0, 1);
}

void Bullet::Update(float DeltaTime)
{
	frameMovement = direction * speed * DeltaTime;
}

Vector2 Bullet::GetFrameMovement()
{
	return frameMovement;
}
