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
	Rectangle drawRightClickMenu(std::string textureString, Vector2* texturePos, Camera2D* camera, std::vector<std::string> UI, Rectangle UIRects[5]); // just make UI global
}; // Split UI functions to new class?
extern RaylibFunctionsClass Functions;

class DragSystem {
private:
	std::string textureHeld;
	float scale = 0;
	std::vector<std::pair<std::string, Vector2>> textureMap;
	std::vector<float> scales;
	std::vector<std::string> buttonTexture;
public:
	void update(std::vector<std::string> UI, Rectangle UIRects[5], Rectangle area, Camera2D camera);
	int mapCount()													{ return textureMap.size(); }
	std::vector<std::pair<std::string, Vector2>>* getTextureMap()	{ return &textureMap; }
	float* getScale(int i)											{ return &scales[i]; }
	std::string* getButtonTexture(int i)							{ return &buttonTexture[i]; }
	void setButton(std::string texture, std::vector<std::string> UI, Rectangle UIRects[5]);
	
	void clearMap() {
		textureHeld.clear();
		scale = 0;
		textureMap.clear();
		scales.clear();
		buttonTexture.clear();
	}

	void removeElementByString(const std::string& element) {
		auto it = std::remove_if(textureMap.begin(), textureMap.end(),
			[&](const auto& pair) {
				return pair.first == element;
			});
		size_t index = std::distance(textureMap.begin(), it);
		textureMap.erase(it, textureMap.end());
		if (index < scales.size()) {
			scales.erase(scales.begin() + index);
		}
		if (index < buttonTexture.size()) {
			buttonTexture.erase(buttonTexture.begin() + index);
		}
	} // move to RaylibFunctions.cpp
};
extern DragSystem Drag;

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

class FileSystem {
private:
public:
	void exportToFile(Rectangle rec);
	Vector2 importFromFile();
};