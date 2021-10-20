#include "System.h"

void System::EntityDestroyed(Entity e)
{
	entities.erase(e);
}

void System::EntitySignatureChanged(Entity e, Signature s)
{
	if((s & systemSignature) == systemSignature) //if Signature s has all flags of systemSignature
	{
		entities.insert(e); //If we already have the element its not inserted again due to how set works
	}
	else
	{
		entities.erase(e);
	}
}
