#pragma once
#include "Keyboardstate.h"
#include "HugoMath.h"

class SystemManager;

class Player
{
public:
	void Update(float DeltaTime, MyKeyBoardState keyboardState, SystemManager* sysManager);
	Vector2 GetFrameMovement() { return FrameMovement; }
	void SetLastPos(Vector2 pos) { lastPos = pos; } //used to keep track of our position without transform
protected:
	Vector2 lastPos;
	Vector2 FrameMovement = Vector2(0,0);
	float Speed = 500;
	float timeBetweenShots = 0.1f;
	float timer;
};

