#include "Player.h"
#include "SystemManager.h"

void Player::Update(float DeltaTime, MyKeyBoardState keyboardState, SystemManager* sysManager)
{
	timer = std::max(0.f, timer - DeltaTime);

	Vector2 newFrameMovement;

	if (keyboardState.forward)
		--newFrameMovement.y;
	if (keyboardState.backward)
		++newFrameMovement.y;
	if (keyboardState.right)
		++newFrameMovement.x;
	if (keyboardState.left)
		--newFrameMovement.x;

	newFrameMovement.Normalize();
	FrameMovement = (newFrameMovement * Speed * DeltaTime);

	if (keyboardState.space && timer < 0.0001f) //00001f to safe against float addition
	{
		timer = timeBetweenShots;
		float bulletBuffer = 25;
		sysManager->SpawnBulletPrefab(Allegiance::PLAYER, Vector2(lastPos.x, lastPos.y - bulletBuffer));
	}
}
