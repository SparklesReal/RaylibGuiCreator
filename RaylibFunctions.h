#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

class RaylibFunctionsClass {
public:
	int drawTextRectCenter(Rectangle rect, std::string text, int size);
	int drawButtonRect(Rectangle rect, std::string text, int size, Color rectangleColor, Color outlineColor, int outlineThickness);
	Camera2D createCamera();
	Camera2D updateCamera(Camera2D camera);
	std::unordered_map<std::string, Texture2D> loadTextures();
	void unloadTextures(std::unordered_map<std::string, Texture2D> textureMap);
	std::unordered_map<std::string, Texture2D> reloadTextures(std::unordered_map<std::string, Texture2D> textureMap);
};

extern RaylibFunctionsClass Functions;