#include <raylib.h>
#include <iostream>
#include <unordered_map>

#include "window.h"
#include "menus/room.h"
#include "RaylibFunctions.h"

ProgramWindowClass Window (1920, 1080, "GUI creator for Raylib");
RoomClass Room;
MainRoomClass MainRoom;
TextureMapClass TextureMap;

int main() {
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	TextureMap.updateTextureMap(RaylibFunctions::loadTextures());

	while (!WindowShouldClose()) {
		switch (Room.getRoomID()) { // Todo: make all rooms use the same system, preferably the case 1: system as then all varibles aren't created outside of the function
		case 0:
			if (Room.mainMenu()) // this room is also quirky and uses a different system, wait that means it is the starndad now waow
				return 0;
			break;

		case 1:
			if (Room.mainRoom())
				return 0;
			break;

		case 2:
			Room.settingsMenu(); // this room is quirky and uses a different system
			break;

		default:
			std::cout << "Invalid RoomID aka the developer committed broken code or you were bored and changed the memory" << std::endl;
			break;

		}
	}

	return 0;
}