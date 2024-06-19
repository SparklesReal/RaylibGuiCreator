#include "room.h"

#include "../RaylibFunctions.h"
#include "../window.h"
#include "../Functions.h"
#include "../filesystem/FilesystemFunction.hpp"

#include <raylib.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <RaylibAdditions.hpp>

int RoomClass::mainMenu() {

	std::string GUIFilename = "";

	std::unordered_map<std::string, RaylibAdditions::ButtonClass> buttonMap { // Eh array might be better, I don't know how much more memory this uses
		{ "StartButton", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 50, 400, 100}, "Start", 80, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "SettingsButton", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 200, 400, 100}, "Settings", 80, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "LoadGUIButton", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 350, 400, 100}, "Load GUI", 60, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "GUIname", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 500, 400, 100}, "", 20, RAYWHITE, DARKGRAY, BLACK, 10, 1) }
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
		RaylibAdditions::drawButtons(&buttonMap);
		EndDrawing();

		RaylibAdditions::updateButtonstates(&buttonMap);

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
					if(!FileSystemFunctions::importFromFile(GUIFilename)) {
						GUIFilename = "";
						Rectangle rect = Rectangle{(float(GetScreenWidth()) / 2.0) - 200, (float(GetScreenHeight()) / 2.0) - 50, 400, 100};
						BeginDrawing();
                        std::string error = "Error: Invalid file";
						RaylibAdditions::drawRectWOutlineWText(rect, 20, GRAY, RED, error, 10, BLACK);
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
			return 1;
		}
	}
	return 0;
}
