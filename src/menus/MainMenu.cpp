#include "room.h"

#include "../RaylibFunctions.h"
#include "../window.h"
#include "../Functions.h"
#include "../filesystem/FilesystemFunction.hpp"

#include <raylib.h>
#include <iostream>
#include <vector>
#include <unordered_map>

int RoomClass::mainMenu() {

	std::unordered_map<std::string, RaylibAdditions::ButtonClass> buttonMap { // Eh array might be better, I don't know how much more memory this uses
		{ "StartButton", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 50, 400, 100}, "Start", 80, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "SettingsButton", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 200, 400, 100}, "Settings", 80, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "LoadGUIButton", RaylibAdditions::ButtonClass(Rectangle{float(GetScreenWidth()) / 2 - 200, 350, 400, 100}, "Load GUI", 60, RAYWHITE, DARKGRAY, BLACK, 10, 1) }
	};

	while (getRoomID() == 0) {

		if (IsWindowResized()) {
			buttonMap.at("StartButton").rect = Rectangle{float(GetScreenWidth()) / 2 - 200, 50, 400, 100};
			buttonMap.at("SettingsButton").rect = Rectangle{ float(GetScreenWidth()) / 2 - 200, 200, 400, 100 };
			buttonMap.at("LoadGUIButton").rect = Rectangle{ float(GetScreenWidth()) / 2 - 200, 350, 400, 100 };
		}

		ClearBackground(BLACK);
		BeginDrawing();
		RaylibAdditions::drawButtons(&buttonMap);
		EndDrawing();

		RaylibAdditions::updateButtonstates(&buttonMap);

		for (int i = 0; i < buttonMap.size(); i++) {
			auto it = std::next(buttonMap.begin(), i);
			if (it->second.state == 2) {
				if (it->first == "StartButton") {
					MainRoom.Frames.push_back(DragSystem());
					Room.setRoomID(1);
					break;
				}
				if (it->first == "SettingsButton") {
					Room.setRoomID(2);
					break;
				}
				if (it->first == "LoadGUIButton") {
					std::string path = FileSystemFunctions::getSaveLocation();
					std::pair<std::vector<RaylibAdditions::FrameClass>, std::vector<std::vector<std::string>>> loadedThing = FileSystemFunctions::importFromFile(path);
					if (loadedThing.first.size() == 0) {
						Rectangle rect = Rectangle{(float(GetScreenWidth()) / 2.0f) - 200.0f, (float(GetScreenHeight()) / 2.0f) - 50.0f, 400.0f, 100.0f};
						BeginDrawing();
                        std::string error = "Error loading file";
						RaylibAdditions::drawRectWOutlineWText(rect, 10, GRAY, RED, error, 20, BLACK);
						EndDrawing();
						WaitTime(2); // This is dumb, please fix
						continue; 
					}
					
					// make this conversion in the loading functions instead of here, idk why I did it here // yes im writing this will still working on it here
					for (int j = 0; j < loadedThing.first.size(); j++) {
						DragSystem newDrag = DragSystem();

						std::vector<std::pair<std::string, Vector2>> textureList;
						for (int k = 0; k < loadedThing.second.at(j).size(); k++) 
							textureList.push_back({loadedThing.second.at(j).at(k), loadedThing.first.at(j).texturePos.at(k)});
						newDrag.setTextureMap(textureList);

						newDrag.setScaleArray(loadedThing.first.at(j).textureScales);
						MainRoom.Frames.push_back(newDrag);
					}
					
					Room.setRoomID(1);
					break;
				}
			}
		}
		if (WindowShouldClose()) {
			return 1;
		}
	}
	return 0;
}
