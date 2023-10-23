#pragma once
#include <unordered_map>
#include <raylib.h>
#include <string>

class RoomClass {
private:
	int roomID = 0;

public:
	void setRoomID(int ID)	{ roomID = ID; }
	int	getRoomID()			{ return roomID; }

	int mainMenu();
	int settingsMenu();
	int mainRoom(std::unordered_map<std::string, Texture2D> textureMap);
};

extern RoomClass Room;