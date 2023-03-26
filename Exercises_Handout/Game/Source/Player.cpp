#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"
#include "Camera.h"

#define FACING_LEFT false
#define FACING_RIGHT true
#define IDLE_STATIC 1
#define IDLE 2
#define RUNNING 3
#define JUMPING 4
#define FALLING 5
#define CHARGING 6
#define DYING 7
#define INITCHARGING 8
#define ENDCHARGING 9
#define WINING 10
#define ATTACKING 11
#define SPECIAL 12


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	// idle player
	idleanim.PushBack({ 0, 0, 32, 32 });
	idleanim.loop = false;
	idleanim.speed = 0.0f;
	
	player_num = app->scene->number_of_players;
}

Player::~Player() {
	number_of_players--;

	if (cameraPlayer != nullptr)
	{
		cameraPlayer->assigned = false;
		cameraPlayer = nullptr;
	}
}

bool Player::Awake() {
	// Get Player parameters from XML
	tile.x = parameters.attribute("x").as_int();
	tile.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	// initilize textures
	texture = app->tex->Load(texturePath);

	currentAnim = &idleanim;

	switch (player_num) {
	case 0:
		kb_up = SDL_SCANCODE_W;
		kb_left = SDL_SCANCODE_A;
		kb_down = SDL_SCANCODE_S;
		kb_right = SDL_SCANCODE_D;
		break;
	case 1:
		kb_up = SDL_SCANCODE_T;
		kb_left = SDL_SCANCODE_F;
		kb_down = SDL_SCANCODE_G;
		kb_right = SDL_SCANCODE_H;
		break;
	case 2:
		kb_up = SDL_SCANCODE_I;
		kb_left = SDL_SCANCODE_J;
		kb_down = SDL_SCANCODE_K;
		kb_right = SDL_SCANCODE_L;
		break;
	case 3:
		kb_up = SDL_SCANCODE_UP;
		kb_left = SDL_SCANCODE_LEFT;
		kb_down = SDL_SCANCODE_DOWN;
		kb_right = SDL_SCANCODE_RIGHT;
		break;
	default:
		break;
	}

	//TODO 3: Assign one camera that is not assigned yet to the current player.
	ListItem<Camera*>* cameraItem;
		

	return true;
}

bool Player::Update()
{
	LOG("Num of players: %d", number_of_players);
	
	//PLAYER MOVEMENT
	Movement();

	//WINNING SEQUENCE
	
	//ANIMATION STATE MACHINE
	for (ListItem<Camera*>* cameraItem = app->render->cameras.start; cameraItem != NULL; cameraItem = cameraItem->next)
	{
		SDL_RenderSetClipRect(app->render->renderer, &cameraItem->data->screen_section);
		SDL_Rect rect = currentAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, tile.x * 32, tile.y * 32, cameraItem->data, &rect);
		currentAnim->Update();
	}
	SDL_RenderSetClipRect(app->render->renderer, nullptr);
	
	CameraMovement();

	return true;
}

bool Player::PostUpdate() {

	return true;
}

void Player::CameraMovement()
{
	if (cameraPlayer == nullptr)
		return;

	cameraPlayer->FollowPlayer(this);
}

bool Player::CleanUp()
{
	return true;
}

void Player::Movement()
{
	if (!app->scene->questMenu)
	{
		if (app->input->GetKey(kb_up) == KEY_DOWN && tile.y > 0)
		{
			tile.y--;
		}
		if (app->input->GetKey(kb_down) == KEY_DOWN && tile.y < 39)//Change to 39
		{
			tile.y++;
		}
		if (app->input->GetKey(kb_left) == KEY_DOWN && tile.x > 0)
		{
			tile.x--;
		}
		if (app->input->GetKey(kb_right) == KEY_DOWN && tile.x < 59)//Change to 59
		{
			tile.x++;
		}
	}

}