#include "EventManager.h"

EventManager::KEYSTATE EventManager::GetKeyState(KEYS key)
{
    return keys[key];
}

void EventManager::SetKeyState(KEYS key, KEYSTATE keystate)
{
    if (keys[key] == keystate) //if we already are in the state, dont notify any subscribers
        return;
    //else
    keys[key] == keystate;

    //Call all subcribers for that state
    if (keystate == KEYSTATE::PRESSED)
    {
        if (!KeySubscribersDown[key].empty())
        {
            for (SubscriberFunction function : KeySubscribersDown[key])
            {
                function(keystate); //alert the subscribers to the key of the new state
            }
        }
    }
    else //keystate::released
    {
        if (!KeySubscribersUp[key].empty())
        {
            for (SubscriberFunction function : KeySubscribersUp[key])
            {
                function(keystate); //alert the subscribers to the key of the new state
            }
        }
    }
    
}

void EventManager::HandleKeyEvent(const SDL_Event* e)
{
    KEYSTATE state;
    if (e->type == SDL_KEYDOWN)
        state = KEYSTATE::PRESSED;
    else if (e->type == SDL_KEYUP)
        state = KEYSTATE::RELEASED;
    else
        return;

    KEYS key;
    switch (e->key.keysym.sym)
    {
    default: return;
    case SDLK_ESCAPE: key = KEY_ESCAPE; break;
    case SDLK_SPACE: key = KEY_SPACE; break;
    case SDLK_w: key = KEY_W; break;
    case SDLK_a: key = KEY_A; break;
    case SDLK_s: key = KEY_S; break;
    case SDLK_d: key = KEY_D; break;
    }

    SetKeyState(key, state);
}

void EventManager::E_SubscribeToKey(KEYS key, KEYSTATE keystate, SubscriberFunction functionPointer)
{
    if (keystate == KEYSTATE::PRESSED || keystate == KEYSTATE::BOTH)
    {
        KeySubscribersDown[key].push_back(functionPointer);
    }
    if (keystate == KEYSTATE::RELEASED || keystate == KEYSTATE::BOTH)
    {
        KeySubscribersUp[key].push_back(functionPointer);
    }
    
}

void EventManager::E_DeSubscribeToKey(KEYS key, KEYSTATE keystate, SubscriberFunction functionPointer)
{
    if (keystate == KEYSTATE::PRESSED || keystate == KEYSTATE::BOTH)
    {
        KeySubscribersDown[key].remove(functionPointer);
    }
    if (keystate == KEYSTATE::RELEASED || keystate == KEYSTATE::BOTH)
    {
        KeySubscribersUp[key].remove(functionPointer);
    }
}
