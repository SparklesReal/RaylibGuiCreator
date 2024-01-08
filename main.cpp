#include <raylib.h>
#include <raygui.h>
#include <iostream>
#include <unordered_map>

#include "window.h"
#include "room.h"
#include "RaylibFunctions.h"

ProgramWindowClass Window;
RoomClass Room;
MainRoomClass MainRoom;
RaylibFunctionsClass Functions;
TextureMapClass TextureMap;

int main() {
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	TextureMap.updateTextureMap(Functions.loadTextures());

	while (!WindowShouldClose()) {
		switch (Room.getRoomID()) { // Todo: make all rooms use the same system, preferably the case 1: system as then all varibles aren't created outside of the function
		case 0:
			Room.mainMenu();
			break;

		case 1:
			if (Room.mainRoom()) // this room is quirky and uses a different system
				return 0;
			break;

		case 2:
			Room.settingsMenu();
			break;

		default:
			std::cout << "Invalid RoomID aka the developer committed broken code or you were bored and changed the memory" << std::endl;
			break;

		}
	}

	return 0;
}