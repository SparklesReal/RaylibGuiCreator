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
	Texture2D* numToTexture(int num);

	int getAmountOfPages();
	std::vector<std::string> getUITextures(int pageNum);
	void drawUI(std::vector<std::string> UI, Rectangle UIRects[], size_t arraySize, int pageNum, Vector2 triangles[6]); // create a better order of the args // Remove arraySize use UIRects[5]
	Rectangle drawRightClickMenu(int textureNum, Vector2* texturePos, Camera2D* camera, std::vector<std::string> UI, Rectangle UIRects[5]); // just make UI global
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
	void setTextureMap(std::vector<std::pair<std::string, Vector2>>& map) { textureMap = map; }
	Texture2D* getTextureByNum(int num);
	float getScale(int i)											{ return scales[i]; }
	void setScaleArray(std::vector<float> scaleArray)				{ scales = scaleArray; }
	std::string getButtonTexture(int i)								{ return buttonTexture[i]; }
	void setButton(int texture, std::vector<std::string> UI, Rectangle UIRects[5]);
	void setButtonArray(std::vector<std::string> buttonTextureVector) { buttonTexture = buttonTextureVector; }
	
	void clearMap() {
		textureHeld.clear();
		scale = 0;
		textureMap.clear();
		scales.clear();
		buttonTexture.clear();
	}

	void removeElementByString(const std::string& element) { // This function was dumb from the start and should just be removed
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

	void removeElementByNum(const int num);
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
	void importFromFile();
};