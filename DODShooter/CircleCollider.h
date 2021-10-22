#pragma once
#include "HugoMath.h"

struct CircleCollider
{
public:
	CircleCollider()
	{
		for (int i = 0; i < COLLIDER_TYPE_MAX; i++)
		{
			Collisions[i] = 0;
		}
	}
	float radius;
	ColliderType type;
	int Collisions[ColliderType::COLLIDER_TYPE_MAX];
	void Collide(ColliderType type)
	{
		Collisions[type]++;
	}
};