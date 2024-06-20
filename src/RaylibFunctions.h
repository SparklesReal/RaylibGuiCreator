#pragma once

#include "menus/room.h"

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

//This file is a mess
namespace RaylibFunctions {
	Camera2D createCamera();
	Camera2D updateCamera(Camera2D camera, float speed);
	std::unordered_map<std::string, Texture2D> loadTextures();
	Texture2D* stringToTexture(std::string texture);

	int getAmountOfPages();
	std::vector<std::string> getUITextures(int pageNum);
	void drawUI(std::vector<std::string> UI, Rectangle UIRects[], size_t arraySize, int pageNum, Vector2 triangles[6]); // create a better order of the args // Remove arraySize use UIRects[5]
	Rectangle drawRightClickMenu(int textureNum, Vector2* texturePos, Camera2D* camera, std::vector<std::string> UI, Rectangle UIRects[5]); // just make UI global // Why did yoou not make it's own class?
	bool allKeysReleased();
}; // Split UI functions to new class?

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
		textureMap = RaylibFunctions::loadTextures();
	}
};
extern TextureMapClass TextureMap;