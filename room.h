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
	int mainRoom();
};
extern RoomClass Room;

class MainRoomClass {
private:
public:
	Vector2 size = Vector2();
};
extern MainRoomClass MainRoom;

class ButtonClass {
private:
public:
	Rectangle rect;
	std::string text;
	int textSize;
	Color color;
	Color outlineColor;
	int outlineThickness;
	int state;

	ButtonClass(Rectangle ButtonRect, std::string ButtonText, int buttonTextSize, Color buttonColor, Color buttonOutlineColor, int buttonOutlineThickness, int buttonState = 0, bool buttonPressed = false) {
		rect = ButtonRect;
		text = ButtonText;
		textSize = buttonTextSize;
		color = buttonColor;
		outlineColor = buttonOutlineColor;
		outlineThickness = buttonOutlineThickness;
		state = buttonState;
	}
};