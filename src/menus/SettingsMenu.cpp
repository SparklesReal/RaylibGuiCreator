#include "room.h"

#include "../Functions.h"
#include "../Window.hpp"
#include "../RaylibFunctions.h"

#include <RaylibAdditions.hpp>

int RoomClass::settingsMenu() {
	std::unordered_map<std::string, RaylibAdditions::ButtonClass> buttonMap{ // Eh array might be better, I don't know how much more memory this uses
		{ "FullscreenButton",	RaylibAdditions::ButtonClass(Rectangle{ float(GetScreenWidth()) / 2 - 200, 50, 400, 100 }, "Fullscreen", 60, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
		{ "BackButton",			RaylibAdditions::ButtonClass(Rectangle{ float(GetScreenWidth()) / 2 - 200, 400, 400, 100 }, "Back", 80, RAYWHITE, DARKGRAY, BLACK, 10, 1) },
	};

	if (IsWindowResized()) {
		buttonMap.at("FullscreenButton").rect =	Rectangle{ float(GetScreenWidth()) / 2 - 200, 50, 400, 100 };
		buttonMap.at("BackButton").rect =		Rectangle{ float(GetScreenWidth()) / 2 - 200, 400, 400, 100 };
	}

	ClearBackground(BLACK);
	BeginDrawing();
    RaylibAdditions::drawButtons(&buttonMap);
    RaylibAdditions::updateButtonstates(&buttonMap);
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