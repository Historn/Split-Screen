#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"
#include "List.h"

#include "SDL/include/SDL.h"

enum class Orientation {
	NO_TYPE = 0,
	SQUARES,				//All cameras have the same width and height, even if it is not square order the maximum number of cameras.
	HORIZONTAL,				//The cameras aux will have more width than the rest to occupy the whole row.
	VERTICAL,				//The cameras aux will have more height than the rest to occupy the whole column.
};

class Camera;

class Render : public Module
{
public:

	Render(bool startEnabled);

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	//Creates a split screen
	void CreateSplitScreen();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, Camera* camera, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, Camera* camera_, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	bool IsOnCamera(const int& x, const int& y, const int& w, const int& h, Camera* camera) const;

public:

	SDL_Renderer* renderer = nullptr;
	List<Camera*> cameras; //Cameras enabled
	List<Camera*> cameraSaves; //Cameras that has to save info
	SDL_Rect viewport;
	SDL_Color background;

	uint							width_of_first_camera = 0;
	uint							height_of_first_camera = 0;

	float							margin = 0;								//size of margin.							
	uint							n_cameras_columns = 0;					//number of columns.
	uint							n_cameras_rows = 0;						//number of rows.
	uint							n_cameras_aux = 0;						//number of cameras in the last row or column (regardless of the orientation, selected if its rows or columns in the orientation).
	Orientation						orientation = Orientation::NO_TYPE;		//orientation of the cameras, look the declaration for more information.

	bool debug = true;
};

#endif // __RENDER_H__