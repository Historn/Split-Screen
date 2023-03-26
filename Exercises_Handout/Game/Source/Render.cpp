#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Camera.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render(bool startEnabled) : Module(startEnabled)
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{
	//Clears all the cameras enabled
	for (ListItem<Camera*>* camItem = cameras.start; camItem != cameras.end; ++camItem) {
		if (camItem != nullptr)
		{
			delete camItem;
			camItem = nullptr;
		}
	}
	cameras.Clear();
}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{

		margin = config.child("margin").attribute("value").as_int();

		n_cameras_columns = config.child("n_cameras_columns").attribute("value").as_int();
		n_cameras_rows = config.child("n_cameras_rows").attribute("value").as_int();
		n_cameras_aux = config.child("n_cameras_aux").attribute("value").as_int();
		orientation = (Orientation)config.child("orientation").attribute("value").as_int();

		CreateSplitScreen();
	}

	return ret;
}

void Render::CreateSplitScreen()
{

	uint final_width = 0;									//the final width that the camera will have in every case.
	uint final_height = 0;									//the final height that the camera will have in every case.
	uint n_cameras = 0;										//will be replaced by n_cameras_rows or n_cameras_columns depending on the orientation.
	uint n_cameras_max = 0;									//maximum number of cameras.


	uint n_cameras_columns_aux = 0;							//number of cameras in the last column. it is assigned the value of n_cameras_axu if the orientation is vertical automatically.
	uint n_cameras_rows_aux = 0;							//number of cameras in the last row. it is assigned the value of n_cameras_axu if the orientation is horizontal automatically.

	switch (orientation)
	{
	case Orientation::NO_TYPE:
		LOG("the orientaiton NO_TYPE is not valid.");																	//don't have cameras aux. 
		break;
	case Orientation::SQUARES:
		//TODO 0: Calculate the max number of cameras in n_cameras_max with n_cameras_columns and n_cameras_rows
																								//Calcule the max number of cameras in this case.
		n_cameras_aux = 0;																		//Doesn't have cameras aux in this case. 
		break;
	case Orientation::HORIZONTAL:
		n_cameras_columns_aux = n_cameras_aux;
		n_cameras_max = (n_cameras_rows - 1) * n_cameras_columns + n_cameras_columns_aux;		//Calcule the max number of cameras in this case. 
		break;
	case Orientation::VERTICAL:
		n_cameras_rows_aux = n_cameras_aux;
		n_cameras_max = n_cameras_rows * (n_cameras_columns - 1) + n_cameras_rows_aux;			//Calcule the max number of cameras in this case.
		break;
	default:
		LOG("The orientaiton is not correct.");
		break;
	}

	/* The + 1 is because there will always be one more margin */
	float width = (app->win->screenSurface->w - ((n_cameras_columns + 1) * margin)) / n_cameras_columns;				//screen width - the sum of all margin (width) / number of columns
	float width_aux = (app->win->screenSurface->w - ((n_cameras_columns_aux + 1) * margin)) / n_cameras_columns_aux;	//the same but with differents number of columns

	float height = (app->win->screenSurface->h - ((n_cameras_rows + 1) * margin)) / n_cameras_rows;						//screen height - the sum of all margin (height) / number of rows
	float height_aux = (app->win->screenSurface->h - ((n_cameras_rows_aux + 1) * margin)) / n_cameras_rows_aux;			//the same but with differents number of rows


	for (uint i = 0; i < n_cameras_max; ++i)
	{
		Camera* camera_aux = nullptr;
		camera_aux = new Camera();

		if (orientation == Orientation::HORIZONTAL || orientation == Orientation::SQUARES)							//it is the same for the case of horizontal orientation as the case of square orientation, but the square will not enter in the else. 
		{
			final_height = height;																						//assign the height, is the same in all the positions.
			n_cameras = n_cameras_columns;

			if (n_cameras_max - i > n_cameras_columns_aux)																//if the camera is not in the last row.
			{
				final_width = width;																					//assign the normal width.
				//TODO 1: Calculate the position of every camera in the screen in camera_aux->screen_section.

																														//formulas to calculate the x and y of the screen section.
																														//the x have the % and the y the / to set the order from left to the right.
			}
			else																										//if the camera is in the last row. the square orientation will not enter here.
			{
				final_width = width_aux;																				//assign the width_aux in the last row becuase is different.
				n_cameras_aux = n_cameras_columns_aux;
				//Now the number of cameras is not the same.
				camera_aux->screen_section.x = margin + (i % n_cameras_aux * (final_width + margin));					//formulas to calculate the x and y of the screen section in the last row.				
				camera_aux->screen_section.y = margin + (i / n_cameras * (final_height + margin));						//the x have the % and the y the / to set the order left left to the right.
			}
		}
		else if (orientation == Orientation::VERTICAL)
		{
			final_width = width;																						//assign the width, is the same in all the positions.
			n_cameras = n_cameras_rows;

			if (n_cameras_max - i > n_cameras_rows_aux)																	//if the camera is not in the last column.
			{
				final_height = height;																					//assign the normal height.
				//TODO 4: Calculate the position of every camera in the screen in camera_aux->screen_section.
				//now we do not count from left to right, now we count from top to bottom.

																														//formulas to calculate the x and y of the screen section.
																														//the x have the / and the y the % to set the order from up to the down.
			}
			else
			{
				final_height = height_aux;																				//assign the height_aux in the last column becuase is different.
				n_cameras_aux = n_cameras_rows_aux;
				//TODO 4: Calculate the position of every camera in the screen in camera_aux->screen_section.
				//In the last column it happens the same, we don’t have the same number of cameras.

																														//formulas to calculate the x and y of the screen section in the last column.	
																														//the x have the / and the y the % to set the order from up to the down.
			}
		}

		camera_aux->rect.w = camera_aux->screen_section.w = final_width;									//assign the final width in the rect and the screen_section defined above in each case.
		camera_aux->rect.h = camera_aux->screen_section.h = final_height;									//assign the final height in the rect and the screen_section definded above in each case.

		cameras.Add(camera_aux);
	}
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		debug = !debug;
		if (!debug)
		{
			for (uint i = cameras.Count(); i > 1; i = cameras.Count())
			{
				cameraSaves.Add(cameras.end->data);
				cameras.Del(cameras.end);
			}

			width_of_first_camera = cameras.start->data->rect.w;
			height_of_first_camera = cameras.start->data->rect.h;

			cameras.start->data->rect.w = cameras.start->data->screen_section.w = app->win->screenSurface->w - margin * 2;
			cameras.start->data->rect.h = cameras.start->data->screen_section.h = app->win->screenSurface->h - margin * 2;

		}
		else
		{
			for (uint i = cameraSaves.Count(); i > 0; i = cameraSaves.Count())
			{
				cameras.Add(cameraSaves.end->data);
				cameraSaves.Del(cameraSaves.end);
			}

			cameras.start->data->rect.w = cameras.start->data->screen_section.w = width_of_first_camera;
			cameras.start->data->rect.h = cameras.start->data->screen_section.h = height_of_first_camera;

		}
	}
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}


// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, Camera* camera, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect = { 0,0,0,0 }; //Section in screen
	//TODO 2: Calculate the new position of rect in screen. Remember that you calculate the position on screen in camera_aux->screen_section.
	//Transform the rect in the word to the rect in screen =======================
	rect.x = -camera->rect.x + x * scale;
	rect.y = -camera->rect.y + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Camera* camera_, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera && camera_ != NULL)
	{
		rec.x = -camera_->rect.x + rect.x * scale;
		rec.y = -camera_->rect.y + rect.y * scale;
		rec.x += camera_->screen_section.x;
		rec.y += camera_->screen_section.y;
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, cameras.start->data->rect.x + x1 * scale, cameras.start->data->rect.y + y1 * scale, cameras.start->data->rect.x + x2 * scale, cameras.start->data->rect.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::IsOnCamera(const int& x, const int& y, const int& w, const int& h, Camera* camera) const
{
	float scale = app->win->GetScale();

	SDL_Rect r = { x * scale,y * scale,w * scale,h * scale };

	return SDL_HasIntersection(&r, &camera->rect);
}