#pragma once
#include <list>
#include "SDL.h"

class EventManager
{
#pragma region
public:
	enum class KEYSTATE
	{
		RELEASED,
		PRESSED,
		BOTH
	};
	enum KEYS
	{
		KEY_W,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_SPACE,
		KEY_ESCAPE,
		KEYS_TOTAL
	};
#pragma endregion ENUMS

	//Below is short hand for a function pointer to a function that returns nothing but takes a KEYSTATE as an argument
	using SubscriberFunction = void(*)(KEYSTATE); //function pointer -> void functionPointerName(Keystate argumentName).

public:
	KEYSTATE GetKeyState(KEYS key);
	void SetKeyState(KEYS key, KEYSTATE keystate);

	void HandleKeyEvent(const SDL_Event* e);

	void E_SubscribeToKey(KEYS key, KEYSTATE keystate, SubscriberFunction functionPointer);
	void E_DeSubscribeToKey(KEYS key, KEYSTATE keystate, SubscriberFunction functionPointer);

private:
	KEYSTATE keys[KEYS_TOTAL];

	/*
	For this section below we have a custom made event system.
	For each key, there is a list of functions that are subscribed to it changing.
	When the key changes, it iterates over the list calling the functions.
	*/
	//These arrays holds a list for a corresponding key
	std::list<SubscriberFunction> KeySubscribersDown[KEYS_TOTAL]; //For when keys are pressed down
	std::list<SubscriberFunction> KeySubscribersUp[KEYS_TOTAL]; //For when keys are released

};

