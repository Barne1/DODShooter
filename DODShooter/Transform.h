#pragma once
#include "SDL.h"
#include "HugoMath.h"

struct Transform
{
	Vector2 pos;
	Vector2 scale;
	float rotationDeg;

	void SafeAddRotation(float Deg) {
		rotationDeg += Deg;
		while (rotationDeg < 0 || rotationDeg > 360)
		{
			rotationDeg += rotationDeg > 360 ? -360 : 360;
		}
	}
};