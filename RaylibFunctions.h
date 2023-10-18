#pragma once

#include <raylib.h>
#include <string>

class RaylibFunctionsClass {
public:
	int drawTextRectCenter(Rectangle rect, std::string text, int size);
	int drawButtonRect(Rectangle rect, std::string text, int size, Color rectangleColor, Color outlineColor, int outlineThickness);
	Camera2D createCamera();
	Camera2D updateCamera(Camera2D camera);
};