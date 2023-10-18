#pragma once

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