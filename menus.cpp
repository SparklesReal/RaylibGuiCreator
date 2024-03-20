#include "room.h"

#include "RaylibFunctions.h"
#include "window.h"
#include "Functions.h"

#include <raylib.h>
#include <iostream>
#include <vector>
#include <unordered_map>

DragSystem Drag;
FileSystem FileSystem;

int RoomClass::mainMenu() {

	std::string GUIFilename = "";

	std::unordered_map<std::string, ButtonClass> buttonMap { // Eh array might be better, I don't know how much more memory this uses
		{ "StartButton", ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 50, 400, 100}, "Start", 80, RAYWHITE, DARKGRAY, 10) },
		{ "SettingsButton", ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 200, 400, 100}, "Settings", 80, RAYWHITE, DARKGRAY, 10) },
		{ "LoadGUIButton", ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 350, 400, 100}, "Load GUI", 60, RAYWHITE, DARKGRAY, 10) },
		{ "GUIname", ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 500, 400, 100}, "", 20, RAYWHITE, DARKGRAY, 10) }
	};

	while (getRoomID() == 0) {

		if (IsWindowResized()) {
			buttonMap.at("StartButton").rect = Rectangle{float(GetScreenWidth()) / 2 - 200, 50, 400, 100};
			buttonMap.at("SettingsButton").rect = Rectangle{ float(GetScreenWidth()) / 2 - 200, 200, 400, 100 };
			buttonMap.at("LoadGUIButton").rect = Rectangle{ float(GetScreenWidth()) / 2 - 200, 350, 400, 100 };
			buttonMap.at("GUIname").rect = Rectangle{ float(GetScreenWidth()) / 2 - 200, 500, 400, 100 };
		}

		if (GUIFilename == "")
			buttonMap.at("GUIname").text = "Enter Filename";
		else
			buttonMap.at("GUIname").text = GUIFilename;

		ClearBackground(BLACK);
		BeginDrawing();
		Functions.drawButtonMap(&buttonMap);
		EndDrawing();

		Functions.updateButtonStates(&buttonMap);

		for (int i = 0; i < buttonMap.size(); i++) {
			auto it = std::next(buttonMap.begin(), i);
			if (it->second.state == 2) {
				if (it->first == "StartButton") {
					Room.setRoomID(1);
					break;
				}
				if (it->first == "SettingsButton") {
					Room.setRoomID(2);
					break;
				}
				if (it->first == "LoadGUIButton" && GUIFilename != "") {
					if(!FileSystem.importFromFile(GUIFilename)) {
						GUIFilename = "";
						Rectangle rect = Rectangle{(float(GetScreenWidth()) / 2.0) - 200, (float(GetScreenHeight()) / 2.0) - 50, 400, 100};
						BeginDrawing();
						Functions.drawButtonRect(rect, "Error: Invalid file", 20, GRAY, RED, 10);
						EndDrawing();
						WaitTime(2); // This is dumb, please fix
						continue;
					}
					Room.setRoomID(1);
					break;
				}
			}
			if (it->first == "GUIname" && it->second.state == 1) {
				char keyboardInput;
				keyboardInput = GetCharPressed();
				if (MeasureTextEx(GetFontDefault(), GUIFilename.c_str(), 20, 10).x < (it->second.rect.width - 20) && keyboardInput != 0)
					GUIFilename += (char(keyboardInput));

				if (IsKeyPressed(KEY_BACKSPACE) && GUIFilename.size() > 0)
					GUIFilename.pop_back();
				break;
			}
		}
		if (WindowShouldClose()) {
			return 0;
		}
	}
	return 1; // This should not happen
}

int RoomClass::settingsMenu() {

	std::unordered_map<std::string, ButtonClass> buttonMap{ // Eh array might be better, I don't know how much more memory this uses
		{ "FullscreenButton",	ButtonClass(Rectangle{ float(GetScreenWidth()) / 2 - 200, 50, 400, 100 }, "Fullscreen", 60, RAYWHITE, DARKGRAY, 10) },
		{ "BackButton",			ButtonClass(Rectangle{ float(GetScreenWidth()) / 2 - 200, 400, 400, 100 }, "Back", 80, RAYWHITE, DARKGRAY, 10) },
	};

	if (IsWindowResized()) {
		buttonMap.at("FullscreenButton").rect =	Rectangle{ float(GetScreenWidth()) / 2 - 200, 50, 400, 100 };
		buttonMap.at("BackButton").rect =		Rectangle{ float(GetScreenWidth()) / 2 - 200, 400, 400, 100 };
	}

	ClearBackground(BLACK);
	BeginDrawing();
	Functions.drawButtonMap(&buttonMap);
	Functions.updateButtonStates(&buttonMap);
	EndDrawing();

	for (int i = 0; i < buttonMap.size(); i++) {
		auto it = std::next(buttonMap.begin(), i);
		if (it->second.state == 2) {
			if (it->first == "FullscreenButton") {
				ToggleFullscreen();
				Window.height = GetScreenHeight();
				Window.width = GetScreenWidth();
				Window.updateWindow();
				break;
			}
			if (it->first == "BackButton") {
				Room.setRoomID(0);
				break;
			}
		}
	}

	return 0;
}

int RoomClass::mainRoom() {
	Camera2D camera = Functions.createCamera();

	Rectangle rectangles[2]{
		{ 0, 0, MainRoom.size.x + 10, MainRoom.size.y + 10 }, // xPos, yPos, RecWidth, RecHeight
		{ rectangles[0].x + 5, rectangles[0].y + 5, MainRoom.size.x, MainRoom.size.y }
	}; // Btw this is stupid and super hard to understand and I should just rewrite this but eh // Buttons are rewritten atleast

	std::unordered_map<std::string, ButtonClass> buttonMap{ // Eh array might be better, I don't know how much more memory this uses
		{ "BackButton",		ButtonClass(Rectangle{ 0, -100, 400, 100 }, "Back", 60, RAYWHITE, DARKGRAY, 10) },
		{ "xInput",			ButtonClass(Rectangle{ 400, -100, 400, 100 }, "", 40, RAYWHITE, DARKGRAY, 10) },
		{ "yInput",			ButtonClass(Rectangle{ 800, -100, 400, 100 }, "", 40, RAYWHITE, DARKGRAY, 10) },
		{ "UpdateButton",	ButtonClass(Rectangle{ 1200, -100, 400, 100 }, "Update", 80, RAYWHITE, DARKGRAY, 10) },
		{ "SaveButton",		ButtonClass(Rectangle{1600, -100, 400, 100 }, "Save", 80, RAYWHITE, DARKGRAY, 10) },
		{ "SaveName",		ButtonClass(Rectangle{1600, -200, 400, 100 }, "", 40, RAYWHITE, DARKGRAY, 10) },
		{ "InnerRect",		ButtonClass(rectangles[1], "", 0, RAYWHITE, RAYWHITE, 0) } // Should not be drawn // Hope this does not cause errors due to empty string, 0 in outline, 0 in text size, and stuff
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

	std::string xInput = "", yInput = "", GUISaveName = "";
	int keyboardInput = 0;
	std::string posString;

	while (getRoomID() == 1) {
		camera = Functions.updateCamera(camera, 2); // System to change the speed (in settings or keybind)
		ClearBackground(BLACK);
		BeginDrawing();
		BeginMode2D(camera);

		Functions.updateButtonStates(&buttonMap, camera);
		Functions.drawButtonRect(&buttonMap.at("BackButton"));
		Functions.drawButtonRect(&buttonMap.at("UpdateButton"));
		Functions.drawButtonRect(&buttonMap.at("SaveButton"));

		if (xInput == "") // Todo: Better implementation or just move to function to make it look good atleast
			buttonMap.at("xInput").text = "Enter width";
		else 
			buttonMap.at("xInput").text = xInput;
		Functions.drawButtonRect(&buttonMap.at("xInput"));

		if (yInput == "")
			buttonMap.at("yInput").text = "Enter height";
		else
			buttonMap.at("yInput").text = yInput;
		Functions.drawButtonRect(&buttonMap.at("yInput"));

		if (GUISaveName == "")
			buttonMap.at("SaveName").text = "Enter Save Name";
		else
			buttonMap.at("SaveName").text = GUISaveName;
		Functions.drawButtonRect(&buttonMap.at("SaveName"));

		for (int i = 0; i < buttonMap.size(); i++) {
			auto it = std::next(buttonMap.begin(), i);
			if (it->second.state != 0) {
				if (it->first == "BackButton" && it->second.state == 2) {
					setRoomID(0);
					Drag.clearMap();
					break;
				}

				if (it->first == "xInput" && it->second.state == 1) {
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), xInput.c_str(), 40, 10).x < (it->second.rect.width - 40) && keyboardInput != 0) // using 10 due to spacing, removing 40 due to outline // just make this a function already...
						xInput += (char(keyboardInput));

					if (IsKeyPressed(KEY_BACKSPACE) && xInput.size() > 0)
						xInput.pop_back();
					break;
				}

				if (it->first == "yInput" && it->second.state == 1) {
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), yInput.c_str(), 40, 10).x < (it->second.rect.width - 40) && keyboardInput != 0) // using 10 due to spacing, removing 40 due to outline
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
						currentUI = Functions.getUITextures(pageNum);
					}
					break;
				}

				if (it->first == "SaveName" && it->second.state == 1) {
					keyboardInput = GetCharPressed();
					if (MeasureTextEx(GetFontDefault(), GUISaveName.c_str(), 40, 10).x < (it->second.rect.width - 40) && keyboardInput != 0) // using 10 due to spacing, removing 40 due to outline
						GUISaveName += (char(keyboardInput));

					if (IsKeyPressed(KEY_BACKSPACE) && GUISaveName.size() > 0)
						GUISaveName.pop_back();
					break;
				}

				if (it->first == "SaveButton" && it->second.state == 2 && GUISaveName != "") {
					FileSystem.exportToFile(rectangles[1], GUISaveName);
				}

				if (it->first == "InnerRect" && it->second.state == 1) {
					posString = "X: " + std::to_string(int(GetScreenToWorld2D(GetMousePosition(), camera).x) - 5) + " Y: " + std::to_string(int(GetScreenToWorld2D(GetMousePosition(), camera).y) - 5); // "-5" due to the inner rectangle being offset by 5 from the outside of it
					EndMode2D();
					DrawText(posString.c_str(), 200, 0, 50, RAYWHITE);
					BeginMode2D(camera);
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


		DrawRectangleLinesEx(rectangles[0], 5, RAYWHITE);

		EndMode2D();
		Drag.update(currentUI, UIRects, rectangles[1], camera);
		Functions.drawUI(currentUI, UIRects, sizeof(UIRects) / sizeof(UIRects[0]), pageNum, triangles);
		EndDrawing();

		if (WindowShouldClose()) {
			std::string saveName = "Autosave";
			FileSystem.exportToFile(rectangles[1], saveName);
			return 1;
		}
	}

	return 0;
}