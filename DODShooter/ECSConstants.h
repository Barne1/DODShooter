#pragma once
#include <bitset>

//constants and type names
using Entity = uint16_t;
const Entity MAX_ENTITIES = 1000;

using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 32; //fits inside a 8 bit integer

using Signature = ComponentType;

enum ComponentSignature
{
	COMPONENT_TRANSFORM = 0b00000001,
	COMPONENT_TEXTURE = 0b00000010,
	COMPENENT_MAX
};