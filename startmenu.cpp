#include "room.h"

#include "RaylibFunctions.h"
#include "window.h"

#include <raylib.h>
#include <iostream>

RaylibFunctionsClass Functions;

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

	Rectangle rectangles[3]{
		{ 0, -100, 400, 100 }, // xPos, yPos, RecWidth, RecHeight
		{ 400, -100, 400, 100 }, 
		{ 800, -100, 400, 100 },
	};

	std::string xInput = "", yInput = "";
	int keyboardInput = 0;

	while (getRoomID() == 1) {
		camera = Functions.updateCamera(camera);
		ClearBackground(BLACK);
		BeginDrawing();
		BeginMode2D(camera);

		Functions.drawButtonRect(rectangles[0], "Back", 80, RAYWHITE, GRAY, 10);

		if (xInput == "") // Todo: Better implementation or just move to function to make it look good atleast
			Functions.drawButtonRect(rectangles[1], "Enter width", 40, RAYWHITE, GRAY, 10);
		else 
			Functions.drawButtonRect(rectangles[1], xInput, 40, RAYWHITE, GRAY, 10);

		if (yInput == "")
			Functions.drawButtonRect(rectangles[2], "Enter height", 40, RAYWHITE, GRAY, 10);
		else
			Functions.drawButtonRect(rectangles[2], yInput, 40, RAYWHITE, GRAY, 10);

		for (int i = 0; i < sizeof(rectangles) / sizeof(rectangles[0]); i++) {
			if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), rectangles[i])) { // Todo: create a better implementation
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

				default:
					std::cout << "The dev needs more coffee also this error should be impossible" << std::endl; // Todo: better error
					break;
				}
			}
		}

		EndMode2D();
		EndDrawing();


		if (WindowShouldClose()) {
			return 1;
		}
	}

	return 0;
}