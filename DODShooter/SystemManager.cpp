#include "SystemManager.h"

SystemManager::SystemManager(const TextureManager* texManager)
{
	textureManager = texManager;
}

Entity SystemManager::CreateEntity()
{
	return entityManager.CreateEntity();
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
		printf("Entity %d already has TransformComponent!\n", e);
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
		printf("Entity %d already has BulletComponent!\n", e);
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

void SystemManager::SpawnBulletPrefab(Allegiance allegiance, Vector2 initialPos)
{
	Entity bullet = CreateEntity();
	AddTransformComponent(bullet, Vector2(Bullet::size, Bullet::size), initialPos);

	TextureManager::TextureTypes textureEnum = 
		(allegiance == Allegiance::PLAYER ? TextureManager::TEXTURE_PLAYERBULLET : TextureManager::TEXTURE_ENEMYBULLET);
	AddTextureComponent(bullet, textureEnum, nullptr, SDL_BLENDMODE_BLEND);

	AddBulletComponent(bullet, allegiance);
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
	
	renderSystem.EntityDestroyed(e);
	updateSystem.EntityDestroyed(e);
	entityManager.DestroyEntity(e);
	movementSystem.EntityDestroyed(e);
}
void SystemManager::EntitySignatureChanged(Entity e)
{
	Signature s = entityManager.GetSignature(e);
	renderSystem.EntitySignatureChanged(e,s);
	updateSystem.EntitySignatureChanged(e,s);
	movementSystem.EntitySignatureChanged(e,s);
}