#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "List.h"

struct SDL_Texture;
class Camera;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

private:

	void Movement();

	//- Camera
	void CameraMovement(/*float dt*/);

public:

	Animation* currentAnim = nullptr;

	Animation idleanim;

	bool godMode = false;

	Camera* cameraPlayer = nullptr;

	int player_num = 0; //The number of player. 0 is the first one.

private:

	// Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	int number_of_players; //Total player num

	SDL_Scancode kb_up = SDL_SCANCODE_UNKNOWN;
	SDL_Scancode kb_left = SDL_SCANCODE_UNKNOWN;
	SDL_Scancode kb_down = SDL_SCANCODE_UNKNOWN;
	SDL_Scancode kb_right = SDL_SCANCODE_UNKNOWN;

};

#endif // __PLAYER_H__