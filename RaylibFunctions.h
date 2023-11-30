#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

class RaylibFunctionsClass {
public:
	int drawTextRectCenter(Rectangle rect, std::string text, int size, Color color);
	int drawButtonRect(Rectangle rect, std::string text, int size, Color rectangleColor, Color outlineColor, int outlineThickness);
	Camera2D createCamera();
	Camera2D updateCamera(Camera2D camera);
	std::unordered_map<std::string, Texture2D> loadTextures();
	Texture2D* stringToTexture(std::string texture);

	int getAmountOfPages();
	std::vector<std::string> getUITextures(int pageNum);
	void drawUI(std::vector<std::string> UI, Rectangle UIRects[], size_t arraySize, int pageNum, Vector2 triangles[6]); // create a better order of the args // Remove arraySize use UIRects[5]
}; // Split UI functions to new class?
extern RaylibFunctionsClass Functions;

class DragSystem {
private:
	std::string textureHeld;
	float scale;
	std::vector<std::pair<std::string, Vector2>> textureMap;
	std::vector<float> scales;
public:
	void update(std::vector<std::string> UI, Rectangle UIRects[5], Rectangle area, Camera2D camera);
};


class TextureMapClass {
private:
	std::unordered_map<std::string, Texture2D> textureMap;

public:
	void updateTextureMap(std::unordered_map<std::string, Texture2D> textureMapArg) { textureMap = textureMapArg; }
	std::unordered_map<std::string, Texture2D>* getTextureMap() { return &textureMap; }

	void unloadTextures() {
		for (auto& count : textureMap) { 
			UnloadTexture(count.second); 
		} 
		textureMap.clear(); 
	}

	void reloadTextures() {
		unloadTextures();
		textureMap = Functions.loadTextures();
	}
};

extern TextureMapClass TextureMap;