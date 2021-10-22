#include "SystemManager.h"

SystemManager::SystemManager(const TextureManager* texManager)
{
	textureManager = texManager;
}

Entity SystemManager::CreateEntity(bool isPlayer)
{
	Entity e = entityManager.CreateEntity();
	if (player)
		this->player = e;
	return e;
}

bool SystemManager::AddTextureComponent(Entity e, TextureManager::TextureTypes textureType, SDL_Rect* srcRect, SDL_BlendMode blendmode)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_TEXTURE)
	{
		printf("Entity %d already has TextureComponent!\n",e );
		return false;
	}
	//else
	StaticTexture t;
	SDL_Texture* tex = textureManager->GetTexture(textureType);
	t.Init(tex, srcRect, blendmode);
	compManager.SetComponent<StaticTexture>(e, t);
	
	s = s | COMPONENT_TEXTURE;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

bool SystemManager::AddTransformComponent(Entity e, Vector2 size, Vector2 initialPos, float rotationDeg)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_TRANSFORM)
	{
		printf("Entity %d already has TransformComponent!\n", e);
		return false;
	}

	Transform t;
	t.pos = initialPos;
	t.size = size;
	t.rotationDeg = rotationDeg;

	compManager.SetComponent<Transform>(e, t);

	s = s | COMPONENT_TRANSFORM;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

bool SystemManager::AddPlayerComponent(Entity e)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_PLAYER)
	{
		printf("Entity %d already has Player Component!\n", e);
		return false;
	}

	Player p;

	compManager.SetComponent<Player>(e, p);

	s = s | COMPONENT_PLAYER;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

bool SystemManager::AddBulletComponent(Entity e, Allegiance a)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_BULLET)
	{
		printf("Entity %d already has Bullet Component!\n", e);
		return false;
	}

	Bullet b;
	b.BulletInit(a);
	compManager.SetComponent<Bullet>(e, b);

	s = s | COMPONENT_BULLET;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

bool SystemManager::AddCircleColliderComponent(Entity e, float radius, ColliderType type)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_COLLIDER)
	{
		printf("Entity %d already has CircleCollider Component!\n", e);
		return false;
	}

	CircleCollider c;
	c.type = type;
	c.radius = radius;
	compManager.SetComponent<CircleCollider>(e, c);

	s = s | COMPONENT_COLLIDER;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

bool SystemManager::AddEnemyComponent(Entity e)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_ENEMY)
	{
		printf("Entity %d already has Enemy Component!\n", e);
		return false;
	}

	Enemy enemy;

	compManager.SetComponent<Enemy>(e, enemy);

	s = s | COMPONENT_ENEMY;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

bool SystemManager::AddSpawnerComponent(Entity e, Vector2 pos)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_SPAWNER)
	{
		printf("Entity %d already has Spawner Component!\n", e);
		return false;
	}

	Spawner spawner;
	spawner.Init(pos);

	compManager.SetComponent<Spawner>(e, spawner);

	s = s | COMPONENT_SPAWNER;
	entityManager.SetSignature(e, s);
	EntitySignatureChanged(e);

	return true;
}

void SystemManager::SpawnBulletPrefab(Allegiance allegiance, Vector2 initialPos)
{
	Entity bullet = CreateEntity();
	AddTransformComponent(bullet, Vector2(Bullet::size, Bullet::size), initialPos);

	TextureManager::TextureTypes textureEnum;
	ColliderType colliderType;
	if (allegiance == Allegiance::PLAYER)
	{
		textureEnum = TextureManager::TEXTURE_PLAYERBULLET;
		colliderType = COLLIDER_PLAYER_BULLET;
	}
	else
	{
		textureEnum = TextureManager::TEXTURE_ENEMYBULLET;
		colliderType = COLLIDER_ENEMY_BULLET;
	}
	AddTextureComponent(bullet, textureEnum, nullptr, SDL_BLENDMODE_BLEND);
	AddBulletComponent(bullet, allegiance);
	AddCircleColliderComponent(bullet, Bullet::size*0.5f, colliderType);
}

void SystemManager::SpawnEnemyPrefab(Vector2 initialPos)
{
	Entity enemy = CreateEntity();
	AddTransformComponent(enemy, Vector2(Enemy::size, Enemy::size), initialPos);
	AddTextureComponent(enemy, TextureManager::TEXTURE_ENEMY, nullptr, SDL_BLENDMODE_BLEND);
	AddEnemyComponent(enemy);
	AddCircleColliderComponent(enemy, Enemy::size * 0.5f, COLLIDER_ENEMY);
}

void SystemManager::SpawnSpawner(Vector2 initialPos)
{
	Entity e = CreateEntity();
	AddSpawnerComponent(e, initialPos);
}

bool SystemManager::HasComponent(Entity e, ComponentType componentType)
{
	Signature s = entityManager.GetSignature(e);
	return (s & componentType);
}

void SystemManager::Render(Vector2 screenDimensions, SDL_Renderer* renderer)
{
	renderSystem.RenderEntities(screenDimensions, renderer, &compManager);
}

void SystemManager::UpdateEntities(float DeltaTime, MyKeyBoardState keyboardState)
{
	updateSystem.UpdateEntities(DeltaTime, keyboardState, this, &compManager);
}

void SystemManager::Move(Vector2 dimensions)
{
	movementSystem.Move(dimensions, &compManager, this);
}

void SystemManager::HandleCollision()
{
	collisionSystem.HandleCollisions(&compManager, &entityManager, this);
}

void SystemManager::DestroyAllPending()
{
	for (Entity e : ToDestroy)
	{
		DestroyEntity(e);
	}
	ToDestroy.clear();
}

void SystemManager::MarkForDestroy(Entity e)
{
	ToDestroy.insert(e);
}

Vector2 SystemManager::GetPlayerPos()
{
	Vector2 pos = compManager.GetComponent<Transform>(player)->pos;
	return pos;
}

void SystemManager::DestroyEntity(Entity e)
{
	Signature s = entityManager.GetSignature(e);
	if (s & COMPONENT_TRANSFORM)
	{
		compManager.RemoveComponent<Transform>(e);
	}
	if (s & COMPONENT_TEXTURE)
	{
		compManager.RemoveComponent<StaticTexture>(e);
	}
	if (s & COMPONENT_PLAYER)
	{
		compManager.RemoveComponent<Player>(e);
	}
	if (s & COMPONENT_BULLET)
	{
		compManager.RemoveComponent<Bullet>(e);
	}
	if (s & COMPONENT_COLLIDER)
	{
		compManager.RemoveComponent<CircleCollider>(e);
	}
	if (s & COMPONENT_ENEMY)
	{
		compManager.RemoveComponent<Enemy>(e);
	}
	if (s & COMPONENT_SPAWNER)
	{
		compManager.RemoveComponent<Spawner>(e);
	}
	
	renderSystem.EntityDestroyed(e);
	updateSystem.EntityDestroyed(e);
	movementSystem.EntityDestroyed(e);
	collisionSystem.EntityDestroyed(e);
	entityManager.DestroyEntity(e);
}
void SystemManager::EntitySignatureChanged(Entity e)
{
	Signature s = entityManager.GetSignature(e);
	renderSystem.EntitySignatureChanged(e,s);
	updateSystem.EntitySignatureChanged(e,s);
	movementSystem.EntitySignatureChanged(e,s);
	collisionSystem.EntitySignatureChanged(e,s);
}