![](https://github.com/Historn/Split-Screen/blob/main/docs/images/title.png?raw=true)

# Useful Information
- Made by: [Arnau Jiménez](https://github.com/Historn)
- Click [here](https://github.com/Historn/Split-Screen) to visit project's Github repository.
- Click [here](https://docs.google.com/presentation/d/1z1sDcuq-cnlALRzvc3pt_n_3Wq_aYstBhH8iRBt2Ryk/edit?usp=sharing) to check slides presentation.

## Table of Contents
<details>
  <summary>⮞ Table of Contents</summary>
  <ol>
    <li>
      <a> Introduction</a>
    </li>
    <li>
      <a> What is a split screen?</a>
    </li>
    <li>
      <a> When should a split screen appear?</a>
    </li>
    <li>
      <a> Things to take into consideration when making a split screen</a>
    </li>
    <li>
      <a> Implementing a Split Screen</a>
    </li>
    <li>
      <a> Application to our projects</a>
    </li>
    <li>
      <a> References</a>
    </li>
  </ol>
</details>

# 1. Introduction
Hello! My name is Arnau Jiménez and I am a second-year bachelor's student in video game design and development at the [CITM](https://www.citm.upc.edu/) at [Universitat Politècnica de Catalunya](https://www.upc.edu/en).

This website is built for information regarding my Personal Research lecture I delivered about Split Screens in video games for Project II subject.

<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

# 2. What is a split screen?
A split screen is a software program technique where the screen is separated into two or more pieces to display and control different media at the same time. In videogames the objective is to allow the players play in the same game, move around the world without depending on the others or do other things such as resource management.

![](https://github.com/Historn/Split-Screen/blob/main/docs/images/types_splitscreen.png?raw=true)

[Image Source](https://en.wikipedia.org/wiki/Split_screen_(computing)#:~:text=In%20its%20most%20easily%20understood,simultaneously%20without%20being%20close%20to)

<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

# 3. When should a split screen appear?
The split screen should be created for a local multiplayer game or for a game where we need to display things in different windows such as resource management games. This way we can play with multiple players without Internet connection in the same screen or manage resources and other stuff in our game without moving on from the gameplay screen.

<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

# 4. Things to take into consideration when making a split screen
The more number of cameras that are used during the gameplay, the higher the performance impact is. This is because each camera is redrawing the whole scene displayed multiplying the performance impact by the number of cameras that have been created. 

Also taking into account that we are splitting n times our screen, each player looses a percentage of the screen then the visibility will be lesser.

<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

***

# 5. Implementing a Split Screen

## Let's do it!

We have a map, with a player that can move within the tiles. So we will try to create a multiplayer game by adding some more players and cameras for focusing on each one of them. The example goes through some exercices that are prepared to understand how a split screen creation works:

First we need to know what does a split screen needs to work correctly. The main steps we need to follow are:
- Have the proper parameters on the config file of the program.
- Load the parameters from the config file.
- Create and render the number of cameras that we want to use for the program.
- Create the number of players that are going to play.
- Draw the players on each camera where they appear.

![](https://github.com/Historn/Split-Screen/blob/main/docs/gifs/quijote.gif?raw=true)

[Image source](https://giphy.com)

### TODOS

Now, you will go through six TODOS in which you will have to solve certain code exercises in order for the split screen to work completly. The main foundations are allready implemented so if you solve the issues the program will run correctly. You can find the answers here and a solved program in my [github repository](https://github.com/Historn/Split-Screen).

#### TODO 0

Edit the config.xml file to complete split screen and cameras parameters and in Render.cpp calculate the max number of cameras in n_cameras_max with n_cameras_columns and n_cameras_rows.

```c++
// config.xml file changes:

	<renderer>
		<vsync value="false"/>
		<!--TODO 0: Set the values to have 4 cameras.-->
		<margin value="10"/> <!--Changes margin size between cameras, default is 10-->
		<n_cameras_columns value="2"/>
		<n_cameras_rows value="2"/>
		<n_cameras_aux value="1"/> <!--n_cameras_aux works for HORIZONTAL and VERTICAL orientations.-->
		<orientation value="2"/>
		<!--the orientation in int: 1: SQUARES, 2: HORIZONTAL, 3: VERTICAL-->
	</renderer>
	
// Render.cpp file changes:
case Orientation::SQUARES:
		//TODO 0: Calculate the max number of cameras in n_cameras_max with n_cameras_columns and n_cameras_rows
		n_cameras_max = n_cameras_columns * n_cameras_rows;		//Calcule the max number of cameras in this case.
		n_cameras_aux = 0;						//Doesn't have cameras aux in this case. 
		break;	
```

#### TODO 1

Calculate the position of every camera in the screen in camera_aux->screen_section.

```c++
// Render.cpp file changes:
if (n_cameras_max - i > n_cameras_columns_aux)	//if the camera is not in the last row.
	{
		final_width = width;			//assign the normal width.
		//TODO 1: Calculate the position of every camera in the screen in camera_aux->screen_section.

		camera_aux->screen_section.x = margin + (i % n_cameras * (final_width + margin));	//formulas to calculate the x and y of the screen section.
		camera_aux->screen_section.y = margin + (i / n_cameras * (final_height + margin));	//the x have the % and the y the / to set the order from left to the right.
	}
```

#### TODO 2

Calculate the new position of rect in screen.

```c++
// Render.cpp file changes:

	//TODO 2: Calculate the new position in rect_in_screen. Remember that you calculate the position on screen in camera_aux->screen_section.
	//Transform the rect in the word to the rect in screen =======================
	rect.x = -camera->rect.x + x * scale;
	rect.y = -camera->rect.y + y * scale;

	//Move the rect_in_screen to their correct screen =========================== 	
	rect.x += camera->screen_section.x;
	rect.y += camera->screen_section.y;
```

#### TODO 3

Assign one camera that is not assigned yet to the current player.

```c++
// Player.cpp file changes:

	//TODO 3: Assign one camera that is not assigned yet to the current player.
	ListItem<Camera*>* cameraItem = app->render->cameras.start;
	while (cameraItem != NULL)
	{
		if (cameraItem->data->assigned == false)
		{
			cameraItem->data->assigned = true;
			cameraPlayer = cameraItem->data;
			break;
		}
		cameraItem = cameraItem->next;
	}
```

#### TODO 4

Calculate the position of every camera in the screen in camera_aux->screen_section.

```c++
if (n_cameras_max - i > n_cameras_rows_aux)		//if the camera is not in the last column.
			{
				final_height = height;																					//assign the normal height.
				//TODO 4: Calculate the position of every camera in the screen in camera_aux->screen_section.
				//now we do not count from left to right, now we count from top to bottom.

				camera_aux->screen_section.x = margin + (i / n_cameras * (final_width + margin));	//formulas to calculate the x and y of the screen section.
				camera_aux->screen_section.y = margin + (i % n_cameras * (final_height + margin));	//the x have the / and the y the % to set the order from up to the down.
			}
			else
			{
				final_height = height_aux;		//assign the height_aux in the last column becuase is different.
				n_cameras_aux = n_cameras_rows_aux;
				//TODO 4: Calculate the position of every camera in the screen in camera_aux->screen_section.
				//In the last column it happens the same, we don’t have the same number of cameras.
				camera_aux->screen_section.x = margin + (i / n_cameras * (final_width + margin));	//formulas to calculate the x and y of the screen section in the last column.	
				camera_aux->screen_section.y = margin + (i % n_cameras_aux * (final_height + margin));		//the x have the / and the y the % to set the order from up to the down.
			}
```

#### TODO 5

Draw the players inside each camera.

```c++
// Player.cpp file changes:

	//TODO 5: Draw the players inside each camera.
	for (ListItem<Camera*>* cameraItem = app->render->cameras.start; cameraItem != NULL; cameraItem = cameraItem->next)
	{
		SDL_RenderSetClipRect(app->render->renderer, &cameraItem->data->screen_section);
		SDL_Rect rect = currentAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, tile.x * 32, tile.y * 32, cameraItem->data, &rect);
		currentAnim->Update();
	}
	SDL_RenderSetClipRect(app->render->renderer, nullptr);
```
<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

# 6. Application to our projects

Now is time for you and your team to implement what we have done into your projects, then you will have an easy and totally functional split screen mode for your games!

## Improvements

As we have seen, we can create squared cameras but... There's any way to improve it visually and its performance impact? Actually, yes.

Voronoi Split Screen is a dynamic type of split screen where the viewport splits itself into the cameras needed depending on the players positions. If the players are close to each other, then we just have one camera rendering both players, but once they move around and they get farther then the screen is splitted procedurally by calculating the perpendicular bisector of two points (ex. Player1 and Player2 positions). 

In this way, when the players are together, a single camera doesn't use as many resources as a splitted screen.

![](https://github.com/Historn/Split-Screen/blob/main/docs/gifs/voronoi_split_screen.gif?raw=true)

[Image source](https://philarchive.org/archive/KARSV](https://mattwoelk.github.io/voronoi_split_screen_notes/))

<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

***

# 7. References

- [Split Screen Wikipedia](https://en.wikipedia.org/wiki/Split_screen_(computing))
- [Fandom: Split Screen](https://ultimatepopculture.fandom.com/wiki/Split_screen_(video_games))
- [Split Screen: Videogame History through Local Multiplayer Design](https://philarchive.org/archive/KARSV)
- [Voronoi split screen: A Quick Tour](https://mattwoelk.github.io/voronoi_split_screen_notes/)
- [Math for Game Programmers: Juicing Your Cameras With Math](https://www.youtube.com/watch?v=tu-Qe66AvtY&t=1575s)

<p align="right">(<a href="https://historn.github.io/Split-Screen/">Back to top</a>)</p>

***
