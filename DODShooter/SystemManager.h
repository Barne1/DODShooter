#pragma once
#include "ECSConstants.h"
#include "EntityIDManager.h"
#include "ComponentManager.h"
#include "TextureManager.h"
#include "Keyboardstate.h"
#include "SystemRender.h"
#include "SystemUpdate.h"
#include "SystemMovement.h"
#include "Bullet.h"

class SystemManager
{
public:
	SystemManager(const TextureManager* texManager);

	Entity CreateEntity();
	bool AddTextureComponent(Entity e, TextureManager::TextureTypes textureType, SDL_Rect* srcRect = nullptr, SDL_BlendMode blendmode = SDL_BLENDMODE_NONE);
	bool AddTransformComponent(Entity e, Vector2 size, Vector2 initialPos = Vector2(0,0), float rotationDeg = 0);
	bool AddPlayerComponent(Entity e);
	bool AddBulletComponent(Entity e, Allegiance a);
	
	void SpawnBulletPrefab(Allegiance allegiance, Vector2 initialPos);

	bool HasComponent(Entity e, ComponentType componentType);

	void Render(Vector2 screenDimensions, SDL_Renderer* renderer);
	void UpdateEntities(float DeltaTime, MyKeyBoardState keyboardState);
	void Move(Vector2 dimensions);
	void DestroyAllPending();

	void MarkForDestroy(Entity e);

protected:
	void DestroyEntity(Entity e);
	void EntitySignatureChanged(Entity e);

	std::set<Entity> ToDestroy;

	EntityIDManager entityManager;
	ComponentManager compManager;

	SystemRender renderSystem;
	SystemUpdate updateSystem;
	SystemMovement movementSystem;

	const TextureManager* textureManager;
};

