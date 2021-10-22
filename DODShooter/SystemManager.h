#pragma once
#include "ECSConstants.h"
#include "EntityIDManager.h"
#include "ComponentManager.h"
#include "TextureManager.h"
#include "Keyboardstate.h"

#include "SystemRender.h"
#include "SystemUpdate.h"
#include "SystemMovement.h"
#include "CollisionSystem.h"

#include "Bullet.h"
#include "CircleCollider.h"
#include "Enemy.h"

class SystemManager
{
public:
	SystemManager(const TextureManager* texManager);

	Entity CreateEntity(bool isPlayer = false);
	bool AddTextureComponent(Entity e, TextureManager::TextureTypes textureType, SDL_Rect* srcRect = nullptr, SDL_BlendMode blendmode = SDL_BLENDMODE_NONE);
	bool AddTransformComponent(Entity e, Vector2 size, Vector2 initialPos = Vector2(0,0), float rotationDeg = 0);
	bool AddPlayerComponent(Entity e);
	bool AddBulletComponent(Entity e, Allegiance a);
	bool AddCircleColliderComponent(Entity e, float radius, ColliderType type);
	bool AddEnemyComponent(Entity e);
	bool AddSpawnerComponent(Entity e, Vector2 pos);
	
	void SpawnBulletPrefab(Allegiance allegiance, Vector2 initialPos);
	void SpawnEnemyPrefab(Vector2 initialPos);
	void SpawnSpawner(Vector2 initialPos);

	bool HasComponent(Entity e, ComponentType componentType);

	void Render(Vector2 screenDimensions, SDL_Renderer* renderer);
	void UpdateEntities(float DeltaTime, MyKeyBoardState keyboardState);
	void Move(Vector2 dimensions);
	void HandleCollision();
	void DestroyAllPending();

	void MarkForDestroy(Entity e);
	Vector2 GetPlayerPos();

protected:
	void DestroyEntity(Entity e);
	void EntitySignatureChanged(Entity e);

	std::set<Entity> ToDestroy;

	EntityIDManager entityManager;
	ComponentManager compManager;

	SystemRender renderSystem;
	SystemUpdate updateSystem;
	SystemMovement movementSystem;
	CollisionSystem collisionSystem;

	const TextureManager* textureManager;

	Entity player;
};

