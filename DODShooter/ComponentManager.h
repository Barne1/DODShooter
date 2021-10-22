#pragma once
#include "ECSConstants.h"
#include "ComponentArray.h"
#include "Transform.h"
#include "StaticTexture.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "CircleCollider.h"
#include "Spawner.h"
#include <unordered_map>
#include <memory>

class ComponentManager
{
public:
	template<typename T>
	T* GetComponent(Entity e);
	template<typename T>
	void SetComponent(Entity e, T type);
	template<typename T>
	void RemoveComponent(Entity e);
	void EntityDestroyed(Entity e, Signature s);

	template<typename T>
	ComponentArray<T>* GetComponentArray();

private:
	//Component Arrays
	ComponentArray<Transform> transformArray;
	ComponentArray<StaticTexture> textureArray;
	ComponentArray<Player> playerArray;
	ComponentArray<Bullet> bulletArray;
	ComponentArray<CircleCollider> colliderArray;
	ComponentArray<Enemy> enemyArray;
	ComponentArray<Spawner> spawnerArray;
};


template<typename T>
inline T* ComponentManager::GetComponent(Entity e)
{
	ComponentArray<T>* arr = GetComponentArray<T>();
	if (arr != nullptr)
	{
		return arr->GetData(e);
	}
	else
		return nullptr;
}

template<typename T>
inline void ComponentManager::SetComponent(Entity e, T type)
{
	ComponentArray<T>* arr = GetComponentArray<T>();
	if (arr != nullptr)
		arr->Insert(e, type);
}

template<typename T>
inline void ComponentManager::RemoveComponent(Entity e)
{
	ComponentArray<T>* arr = GetComponentArray<T>();
	if (arr != nullptr)
		arr->Remove(e);
}

inline void ComponentManager::EntityDestroyed(Entity e, Signature s)
{
	if (s & COMPONENT_TRANSFORM)
		transformArray.EntityDestroyed(e);
	if (s & COMPONENT_TEXTURE)
		textureArray.EntityDestroyed(e);
	if (s & COMPONENT_PLAYER)
		playerArray.EntityDestroyed(e);
	if (s & COMPONENT_BULLET)
		bulletArray.EntityDestroyed(e);
	if (s & COMPONENT_COLLIDER)
		colliderArray.EntityDestroyed(e);
	if (s & COMPONENT_ENEMY)
		enemyArray.EntityDestroyed(e);
	if (s & COMPONENT_SPAWNER)
		spawnerArray.EntityDestroyed(e);
}

template<typename T>
inline ComponentArray<T>* ComponentManager::GetComponentArray()
{
	//dynamic cast to make the compiler go away, even though it will never return wrong since if typeid
	//I dont have time to make a more complicated system with templates
	if(typeid(T) == typeid(Transform))
		return dynamic_cast<ComponentArray<T>*>(&transformArray);
	if (typeid(T) == typeid(StaticTexture))
		return dynamic_cast<ComponentArray<T>*>(&textureArray);
	if (typeid(T) == typeid(Player))
		return dynamic_cast<ComponentArray<T>*>(&playerArray);
	if (typeid(T) == typeid(Bullet))
		return dynamic_cast<ComponentArray<T>*>(&bulletArray);
	if (typeid(T) == typeid(CircleCollider))
		return dynamic_cast<ComponentArray<T>*>(&colliderArray);
	if (typeid(T) == typeid(Enemy))
		return dynamic_cast<ComponentArray<T>*>(&enemyArray);
	if (typeid(T) == typeid(Spawner))
		return dynamic_cast<ComponentArray<T>*>(&spawnerArray);
	//else
	return nullptr;
}
