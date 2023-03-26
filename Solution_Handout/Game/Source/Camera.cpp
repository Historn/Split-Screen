#include "Camera.h"
#include "Window.h"
#include "App.h"
#include "Map.h"


Camera::Camera()
{
}


void Camera::FollowPlayer(Player* player)
{
	fPoint target_pos((player->tile.x*32) * app->win->GetScale(), (player->tile.y*32) * app->win->GetScale());

	rect.x = target_pos.x+16 - screen_section.w/2;
	rect.y = target_pos.y+16 - screen_section.h/2;

	if ((rect.x + screen_section.w) >= app->map->mapData.width*32) {
		rect.x = (app->map->mapData.width*32) - screen_section.w;
	}
	if ((rect.y + screen_section.h) >= app->map->mapData.height*32) {
		rect.y = (app->map->mapData.height*32) - screen_section.h;
	}
	if (rect.x <= 0) {
		rect.x = 0;
	}
	if (rect.y <= 0) {
		rect.y = 0;
	}
}
