#include "room.h"

#include "RaylibFunctions.h"
#include "window.h"
#include "Functions.h"

#include <raylib.h>
#include <iostream>
#include <vector>

FunctionClass StringFunctions;
DragSystem DragSystem;

int RoomClass::mainMenu() {

	Rectangle rectangles[2]{
		{ GetScreenWidth() / 2 - 200, 50, 400, 100 }, // xPos(ScreenWidth/2 - RecWidth/2), yPos, RecWidth, RecHeight
		{ GetScreenWidth() / 2 - 200, 200, 400, 100 } // xPos(ScreenWidth/2 - RecWidth/2), yPos, RecWidth, RecHeight
	};
	//Rectangles are made in the loop to automatically update the position if the window size would change

	ClearBackground(BLACK);
	BeginDrawing();

	Functions.drawButtonRect(rectangles[0], "Start", 80, RAYWHITE, DARKGRAY, 10);
	Functions.drawButtonRect(rectangles[1], "Settings", 80, RAYWHITE, DARKGRAY, 10);

	if (IsMouseButtonDown(0)) {
		for (int i = 0; i < sizeof(rectangles) / sizeof(rectangles[0]); i++) {
			if (CheckCollisionPointRec(GetMousePosition(), rectangles[i])) {
				switch (i) {
				case 0:
					Room.setRoomID(1);
					break;
				case 1:
					Room.setRoomID(2);
					break;
				}
			}
		}
	}

	EndDrawing();

	return 0;
}

bool rect1Pressed = true, rect2Pressed = true;

int RoomClass::settingsMenu() {

	Rectangle rectangles[2]{
		{ GetScreenWidth() / 2 - 200, 50, 400, 100 }, // xPos(ScreenWidth/2 - RecWidth/2), yPos, RecWidth, RecHeight
		{ GetScreenWidth() / 2 - 200, 200, 400, 100 } // xPos(ScreenWidth/2 - RecWidth/2), yPos, RecWidth, RecHeight
	};

	ClearBackground(BLACK);
	BeginDrawing();

	Functions.drawButtonRect(rectangles[0], "Fullscreen", 60, RAYWHITE, DARKGRAY, 10);
	Functions.drawButtonRect(rectangles[1], "Back", 60, RAYWHITE, DARKGRAY, 10);

	if (IsMouseButtonDown(0)) {
		for (int i = 0; i < sizeof(rectangles) / sizeof(rectangles[0]); i++) {
			if (CheckCollisionPointRec(GetMousePosition(), rectangles[i])) { // Todo: create functions and remove this nesting and wierd varibles like "rect2Pressed"
				switch (i) {
				case 0:
					if (rect1Pressed == false) {
						ToggleFullscreen();
						Window.setHeight(GetScreenHeight());
						Window.setWidth(GetScreenWidth());
						rect1Pressed = true;
					}
					break;

				case 1:
					if (rect2Pressed == false) {
						Room.setRoomID(0);
						rect2Pressed = true;
						WaitTime(0.1); // I am too lazy to implement the rect2Pressed for room 0 aka the main menu so to not click on settings directly when pressing back we wait 0.1 seconds (this works somehow and even better than I tought)
					}
					break;
				}
			}
		}
	}

	if (IsMouseButtonReleased(0)) {
		rect1Pressed = false;
		rect2Pressed = false;
	}

	EndDrawing();

	return 0;
}

int RoomClass::mainRoom() {
	Camera2D camera = Functions.createCamera();

	Rectangle rectangles[6]{
		{ 0, -100, 400, 100 }, // xPos, yPos, RecWidth, RecHeight
		{ 400, -100, 400, 100 }, 
		{ 800, -100, 400, 100 },
		{ 1200, -100, 400, 100 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};

	Rectangle UIRects[]{
		{ 0, 0, 200, 200 }, // xPos, yPos, RecWidth, RecHeight
		{ 0, 200, 200, 200 },
		{ 0, 400, 200, 200 },
		{ 0, 600, 200, 200 },
		{ 0, 800, 200, 200 }
	};

	Vector2 triangles[6]{
		{50, 1010},
		{10, 1040},
		{50, 1070},
		{150, 1010},
		{150, 1070},
		{190, 1040}
	};

	int pageNum = 1;
	std::vector<std::string> currentUI = Functions.getUITextures(pageNum);

	std::string xInput = "", yInput = "";
	int keyboardInput = 0;
	std::string posString;

	while (getRoomID() == 1) {
		camera = Functions.updateCamera(camera);
		ClearBackground(BLACK);
		BeginDrawing();
		Functions.drawUI(currentUI, UIRects, sizeof(UIRects) / sizeof(UIRects[0]), pageNum, triangles);
		BeginMode2D(camera);

		Functions.drawButtonRect(rectangles[0], "Back", 80, RAYWHITE, GRAY, 10);
		Functions.drawButtonRect(rectangles[3], "Update", 80, RAYWHITE, GRAY, 10);

		if (xInput == "") // Todo: Better implementation or just move to function to make it look good atleast
			Functions.drawButtonRect(rectangles[1], "Enter width", 40, RAYWHITE, GRAY, 10);
		else 
			Functions.drawButtonRect(rectangles[1], xInput, 40, RAYWHITE, GRAY, 10);

		if (yInput == "")
			Functions.drawButtonRect(rectangles[2], "Enter height", 40, RAYWHITE, GRAY, 10);
		else
			Functions.drawButtonRect(rectangles[2], yInput, 40, RAYWHITE, GRAY, 10);

		for (int i = 0; i < sizeof(rectangles) / sizeof(rectangles[0]); i++) {
			if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), rectangles[i])) { // Todo: better implementation
				switch (i) {
				case 0:
					if (IsMouseButtonPressed(0))
						setRoomID(0);
					break;

				case 1:
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), xInput.c_str(), 40, 10).x < (rectangles[i].width - 40) && keyboardInput != 0) // using 10 due to spacing, removing 40 due to outline // just make this a function already...
						xInput += (char(keyboardInput));
					
					if (GetKeyPressed() == KEY_BACKSPACE && xInput.size() > 0)
						xInput.pop_back();
					break;

				case 2:
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), yInput.c_str(), 40, 10).x < (rectangles[i].width - 40) && keyboardInput != 0 ) // using 10 due to spacing, removing 40 due to outline
						yInput += (char(keyboardInput));

					if (GetKeyPressed() == KEY_BACKSPACE && yInput.size() > 0)
						yInput.pop_back();
					break; // I need more coffee

				case 3:
					if (IsMouseButtonPressed(0) && StringFunctions.stringIsInt(xInput) && StringFunctions.stringIsInt(yInput)) {
						rectangles[4] = {0, 0, std::stof(xInput) + 10, std::stof(yInput) + 10}; // the "+ 10" is to make the rect the right size whilst the outline is 5 thick
						rectangles[5] = {rectangles[4].x + 5, rectangles[4].y + 5, std::stof(xInput), std::stof(yInput)}; // inner size of the rectangle
						TextureMap.reloadTextures(); // for some reason this seems to change the order of the textures, please look in to
						pageNum = 1;
						currentUI = Functions.getUITextures(pageNum);
					}
					break;

				case 4:
					break;

				case 5:
					posString = "X: " + std::to_string(int(GetScreenToWorld2D(GetMousePosition(), camera).x) - 5) + " Y: " + std::to_string(int(GetScreenToWorld2D(GetMousePosition(), camera).y) - 5); // "-5" due to the inner rectangle being offset by 5 from the outside of it
					EndMode2D();
					DrawText(posString.c_str(), 200, 0, 50, RAYWHITE); // draw fixed to screen
					BeginMode2D(camera);
					break;

				default:
					std::cout << "The dev is stupid also this error should be impossible" << std::endl; // Todo: better error
					break;
				}
			}
		}

		if (IsMouseButtonPressed(0)) {
			if (CheckCollisionPointTriangle(GetMousePosition(), triangles[0], triangles[1], triangles[2])) {
				if (pageNum > 1)
					pageNum--;
				else
					pageNum = Functions.getAmountOfPages();
		}

			if (CheckCollisionPointTriangle(GetMousePosition(), triangles[3], triangles[4], triangles[5])) {
				if (pageNum < Functions.getAmountOfPages())
					pageNum++;
				else
					pageNum = 1;
			}

			currentUI = Functions.getUITextures(pageNum);
		}


		DrawRectangleLinesEx(rectangles[4], 5, RAYWHITE);

		EndMode2D();
		DragSystem.update(currentUI, UIRects, rectangles[5], camera);
		EndDrawing();

		if (WindowShouldClose()) {
			return 1;
		}
	}

	return 0;
}