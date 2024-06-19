#include "room.h"

#include "../Functions.h"
#include "../window.h"
#include "../RaylibFunctions.h"

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
	RaylibFunctions::drawButtonMap(&buttonMap);
	RaylibFunctions::updateButtonStates(&buttonMap);
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