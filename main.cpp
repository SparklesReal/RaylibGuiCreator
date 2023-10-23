#include <raylib.h>
#include <raygui.h>
#include <iostream>
#include <unordered_map>

#include "window.h"
#include "room.h"
#include "RaylibFunctions.h"

WindowClass Window;
RoomClass Room;
RaylibFunctionsClass Functions;

int main() {
	InitWindow(Window.getWidth(), Window.getHeight(), Window.getTitle().c_str());
	if (!IsWindowReady) {
		std::cout << "Failed to create window";
		return 1;
	}
	SetWindowState(FLAG_WINDOW_RESIZABLE); // Todo: move this window init and error handeling stuff to a init function

	std::unordered_map<std::string, Texture2D> textureMap = Functions.loadTextures(); // load textures

	while (!WindowShouldClose()) {
		switch (Room.getRoomID()) { // Todo: make all rooms use the same system, preferebly the case 1: system as then all varibles aren't created outside of the function
		case 0:
			Room.mainMenu();
			break;

		case 1:
			if (Room.mainRoom(textureMap)) // this room is quirky and uses a diffrent system
				return 0;
			break;

		case 2:
			Room.settingsMenu();
			break;

		default:
			std::cout << "Invalid RoomID aka the developer committed broken code or you where bored and changed the memory" << std::endl;
			break;

		}
	}

	return 0;
}