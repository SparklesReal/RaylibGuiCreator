#include "room.h"

#include "../Functions.h"
#include "../RaylibFunctions.h"
#include "../filesystem/FilesystemFunction.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <RaylibAdditions.hpp>

DragSystem Drag;

int RoomClass::mainRoom() {
	Camera2D camera = RaylibFunctions::createCamera();

	Rectangle rectangles[2]{
		{ 0, 0, MainRoom.size.x + 10, MainRoom.size.y + 10 }, // xPos, yPos, RecWidth, RecHeight
		{ rectangles[0].x + 5, rectangles[0].y + 5, MainRoom.size.x, MainRoom.size.y }
	}; // Btw this is stupid and super hard to understand and I should just rewrite this but eh // Buttons are rewritten atleast

	std::unordered_map<std::string, RaylibAdditions::ButtonClass> buttonMap{ // Eh array might be better, I don't know how much more memory this uses
		{ "BackButton",		RaylibAdditions::ButtonClass(Rectangle{ 200, 0, 300, 100 }, "Back", 60, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "xInput",			RaylibAdditions::ButtonClass(Rectangle{ 500, 0, 300, 100 }, "", 40, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "yInput",			RaylibAdditions::ButtonClass(Rectangle{ 800, 0, 300, 100 }, "", 40, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "UpdateButton",	RaylibAdditions::ButtonClass(Rectangle{ 1100, 0, 300, 100 }, "Update", 40, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "SaveButton",		RaylibAdditions::ButtonClass(Rectangle{1400, 0, 300, 100 }, "Save", 40, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
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
	std::vector<std::string> currentUI = RaylibFunctions::getUITextures(pageNum);

	std::string xInput = "", yInput = "";
	int keyboardInput = 0;
	std::string posString;
	bool updateCam = true;

	while (getRoomID() == 1) {
		if (!updateCam && RaylibFunctions::allKeysReleased())
			updateCam = true;
		if (updateCam)
			camera = RaylibFunctions::updateCamera(camera, 2); // System to change the speed (in settings or keybind)
		ClearBackground(BLACK);
		BeginDrawing();

		RaylibAdditions::drawButton(&buttonMap.at("BackButton"));
		RaylibAdditions::drawButton(&buttonMap.at("UpdateButton"));
		RaylibAdditions::drawButton(&buttonMap.at("SaveButton"));

		if (xInput == "") // Todo: Better implementation or just move to function to make it look good atleast
			buttonMap.at("xInput").text = "Enter width";
		else 
			buttonMap.at("xInput").text = xInput;
		RaylibAdditions::drawButton(&buttonMap.at("xInput"));

		if (yInput == "")
			buttonMap.at("yInput").text = "Enter height";
		else
			buttonMap.at("yInput").text = yInput;
		RaylibAdditions::drawButton(&buttonMap.at("yInput"));

		RaylibAdditions::updateButtonstates(&buttonMap);

		BeginMode2D(camera);

		//if(!CheckCollisionPointRec(GetMousePosition(), Rectangle{0, 0, 200, 1080}))

		for (int i = 0; i < buttonMap.size(); i++) {
			auto it = std::next(buttonMap.begin(), i);
			if (it->second.state != 0) {
				if (it->first == "BackButton" && it->second.state == 2) {
					setRoomID(0);
					Drag.clearMap();
					break;
				}

				if (it->first == "xInput" && it->second.state == 1) {
					updateCam = false;
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), xInput.c_str(), it->second.textSize, 10).x < (it->second.rect.width - 40) && keyboardInput != 0) // using 10 due to spacing, removing 40 due to outline // just make this a function already...
						xInput += (char(keyboardInput));

					if (IsKeyPressed(KEY_BACKSPACE) && xInput.size() > 0)
						xInput.pop_back();
					break;
				}

				if (it->first == "yInput" && it->second.state == 1) {
					updateCam = false;
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), yInput.c_str(), it->second.textSize, 10).x < (it->second.rect.width - 40) && keyboardInput != 0) // using 10 due to spacing, removing 40 due to outline
						yInput += (char(keyboardInput));

					if (IsKeyPressed(KEY_BACKSPACE) && yInput.size() > 0)
						yInput.pop_back();
					break;
				}

				if (it->first == "UpdateButton" && it->second.state == 2) {
					if (NormalFunctions::stringIsInt(xInput) && NormalFunctions::stringIsInt(yInput)) {
						rectangles[0] = { 0, 0, std::stof(xInput) + 10, std::stof(yInput) + 10 }; // the "+ 10" is to make the rect the right size whilst the outline is 5 thick
						rectangles[1] = { rectangles[0].x + 5, rectangles[0].y + 5, std::stof(xInput), std::stof(yInput) }; // inner size of the rectangle
						TextureMap.reloadTextures(); // for some reason this seems to change the order of the textures (only first time), please look into
						pageNum = 1;
						currentUI = RaylibFunctions::getUITextures(pageNum);
					}
					break;
				}

				if (it->first == "SaveButton" && it->second.state == 2) {
					std::string saveLocation = FileSystemFunctions::setSaveLocation();
					FileSystemFunctions::exportToFile(rectangles[1], saveLocation);
				}

			}
		}

		if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), rectangles[1])) {
			posString = "X: " + std::to_string(int(GetScreenToWorld2D(GetMousePosition(), camera).x) - 5) + " Y: " + std::to_string(int(GetScreenToWorld2D(GetMousePosition(), camera).y) - 5); // "-5" due to the inner rectangle being offset by 5 from the outside of it
			EndMode2D();
			DrawText(posString.c_str(), 200, 100, 50, RAYWHITE);
			BeginMode2D(camera);
		}

		if (IsMouseButtonPressed(0) && RaylibFunctions::getAmountOfPages() != 0) {

			if (CheckCollisionPointTriangle(GetMousePosition(), triangles[0], triangles[1], triangles[2])) {
				if (pageNum > 1)
					pageNum--;
				else
					pageNum = RaylibFunctions::getAmountOfPages();
			}

			if (CheckCollisionPointTriangle(GetMousePosition(), triangles[3], triangles[4], triangles[5])) {
				if (pageNum < RaylibFunctions::getAmountOfPages())
					pageNum++;
				else
					pageNum = 1;
			}

			currentUI = RaylibFunctions::getUITextures(pageNum);
		}


		DrawRectangleLinesEx(rectangles[0], 5, RAYWHITE);

		EndMode2D();
		Drag.update(currentUI, UIRects, rectangles[1], camera);
		RaylibFunctions::drawUI(currentUI, UIRects, sizeof(UIRects) / sizeof(UIRects[0]), pageNum, triangles);
		EndDrawing();

		if (WindowShouldClose()) {
			std::string saveName = "Autosave.gui"; // Do not overwrite autosave, please fix :)
			FileSystemFunctions::exportToFile(rectangles[1], saveName);
			return 1;
		}
	}

	return 0;
}