#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Player.h"
#include "Point.h"

class Camera
{
public:
	Camera();

	void FollowPlayer(Player* player);

public:
	SDL_Rect rect = { 0, 0, 0, 0 }; //The actual camera coordinates in the world
	SDL_Rect screen_section = { 0, 0, 0, 0 }; //The section on the screen it covers (ex. player one gets 0, 0, w/2, h/2)
	bool assigned = false;
	uint number_player = 0;
};