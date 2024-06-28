#pragma once
#include <unordered_map>
#include <raylib.h>
#include <string>
#include <vector>

#include "../RaylibFunctions.h"

class RoomClass {
private:
	int roomID = 0;

public:
	void setRoomID(int ID)	{ roomID = ID; }
	int	getRoomID()			{ return roomID; }

	int mainMenu();
	int settingsMenu();
	int mainRoom();
};
extern RoomClass Room;

class MainRoomClass {
private:
public:
	Vector2 size = Vector2();
	std::vector<DragSystem> Frames;
};
extern MainRoomClass MainRoom;