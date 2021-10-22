#include "Enemy.h"
#include "SystemManager.h"

void Enemy::Update(float DeltaTime, SystemManager* sysManager)
{
	shotTimer = std::max(0.f, shotTimer-DeltaTime);
	FrameMovement.x = std::sin(lastPos.y * sideSwingSpeed) * sideFluctuation;
	FrameMovement.y = Direction.y * Speed * DeltaTime;

	if (shotTimer < 0.00001f)
	{
		shotTimer = timeBetweenShots;

		int bulletBuffer = 50;
		Vector2 bulletPos = Vector2(lastPos.x, lastPos.y + bulletBuffer);
		sysManager->SpawnBulletPrefab(Allegiance::ENEMY, bulletPos);
	}
}
