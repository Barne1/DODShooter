#pragma once
#include "ECSConstants.h"
#include <unordered_map>

using ArrayIndex = Entity; //Array indeces should be same size as Entities

//Ugly but only thing that works
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity e) = 0;
};

template<typename T>
class ComponentArray : IComponentArray
{
public:
	void Insert(Entity e, T component);
	void Remove(Entity e);
	T* GetData(Entity e);
	void EntityDestroyed(Entity e) override;

private:
	T compArray[MAX_ENTITIES]; //packed array
	std::unordered_map<Entity, ArrayIndex> entityToArrayIndex;
	std::unordered_map<ArrayIndex, Entity> arrayIndexToEntity; //this is needed for reordering the packed array on deletions
	ArrayIndex compArrSize;
};

template<typename T>
void ComponentArray<T>::Insert(Entity e, T component)
{
	if (entityToArrayIndex.find(e) != entityToArrayIndex.end())
	{
		return;
	}

	ArrayIndex newIndex = compArrSize; //new index is at the end of the array
	entityToArrayIndex[e] = newIndex;
	arrayIndexToEntity[newIndex] = e;
	compArray[newIndex] = component;
	compArrSize++;
}

template<typename T>
void ComponentArray<T>::Remove(Entity e)
{
	if (entityToArrayIndex.find(e) == entityToArrayIndex.end())
	{
		printf("Could not remove %d\n", e);
		return;
	}

	ArrayIndex removedEntityIndex = entityToArrayIndex[e];
	ArrayIndex fillerIndex = compArrSize - 1; //filler is used for the entity that "fills" the empty spot

	compArray[removedEntityIndex] = compArray[fillerIndex]; //the fill itself. The last spot in the array migrates to the now empty spot
	ArrayIndex newFillerIndex = removedEntityIndex;
	compArrSize--; //the fillers old spot is now ripe for an overwrite

	//update the index map
	Entity filler = arrayIndexToEntity[fillerIndex]; //find the fillers entity ID
	entityToArrayIndex[filler] = newFillerIndex; //repoint the entityToArrayMap to the proper spot for the filler
	arrayIndexToEntity[newFillerIndex] = filler; //And repoint the other map as well for future removals

	entityToArrayIndex.erase(e); //remove the old entity from the map
	arrayIndexToEntity.erase(fillerIndex); //since we have repointed the other Array Indeces, we should also remove the old filler index

}

template<typename T>
T* ComponentArray<T>::GetData(Entity e)
{
	if (entityToArrayIndex.find(e) == entityToArrayIndex.end())
	{
		return nullptr;
	}
	return &compArray[entityToArrayIndex[e]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity e)
{
	if (entityToArrayIndex.find(e) != entityToArrayIndex.end()) //if the entity was in the list
	{
		Remove(e);
	}
}
