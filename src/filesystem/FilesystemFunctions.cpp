#include "FilesystemFunction.hpp"

#include "../RaylibFunctions.h"
#include "../Functions.h"

#include <fstream>
#include <vector>
#include <string>
#include <iostream>


void FileSystemFunctions::exportToFile(Rectangle rec, std::string& filename) {
	std::ofstream file;
	file.open(filename + ".gui"); // Add system to not edit alredy existing files
	file << rec.width << ":" << rec.height << std::endl;
	file << "!!!0:\n";
	for (int i = 0; i < Drag.mapCount(); i++) {
		auto it = std::next(Drag.getTextureMap()->begin() , i);
		if (Drag.getButtonTexture(i) == "N/A")
			file << "\t" << i << " - " << it->first << " -- " << it->second.x << " --- " << it->second.y << " ---- " << "x" << Drag.getScale(i) << std::endl;
	}
	file << "!!!buttons:";
	for (int i = 0; i < Drag.mapCount(); i++) {
		auto it = std::next(Drag.getTextureMap()->begin(), i);
		if (Drag.getButtonTexture(i) != "N/A")
			file << "\t" << i << " - " << it->first << " -- " << Drag.getButtonTexture(i) << " --- " << it->second.x << " ---- " << it->second.y << " " << " ----- " << Drag.getScale(i) << std::endl;
	}
	file.close();
}

bool FileSystemFunctions::importFromFile(std::string& filename) { // move this/create a better loader in RaylibAdditions and include it here instead
	std::ifstream file;
	file.open(filename + ".gui");
	if (!file.is_open())
		return false;
	std::string line;
	int i = 0;
	int currentFrame = 0;
	Vector2 size{ 0,0 };
	std::vector<std::pair<std::string, Vector2>> map;
	std::vector<float> scaleVector;
	std::vector<std::string> buttonTexture;
	while (std::getline(file, line)) {
		i++;
		std::cout << line << std::endl;
		if (i == 1) {
			int index = line.find(":");
			std::string Xstring = line.substr(0, index);
			std::string Ystring = line.substr(index + 1, line.length());
			size = { std::stof(Xstring), std::stof(Ystring) };
			MainRoom.size = size;
			continue;
		}
		if (i == 3) {
			if (line != "!!!buttons:")
				continue;
			std::cout << "Error: File is empty" << std::endl;
			return false;
		}
		size_t index = line.find("!!!");
 		if (index != std::string::npos && index + 3 == 3) {
			std::string value = line.substr(index + 3, line.length());
			std::cout << value;
			value = value.substr(0, value.find(':'));
			if (NormalFunctions::stringIsInt(value)) {
				currentFrame = std::stoi(value); // use later when frame system is in place
				continue;
			}
			else if (value == "buttons") {
				// do stuff
				continue;
			} else {
				std::cout << "Error: value is not int nor buttons" << std::endl; // Yes I needed this while coding... 
				return false;
			}
		}
		std::string texture = line.substr(line.find(" - ") + 3, line.find(" -- ") - line.find(" - ") - 3);
		float textureX = stof(line.substr(line.find(" -- ") + 4, line.find(" --- ") - line.find(" -- ") - 4));
		float textureY = stof(line.substr(line.find(" --- ") + 5, line.find(" ---- ") - line.find(" --- ") - 5));
		float scale = stof(line.substr(line.find(" ---- ") + 7, line.length() - line.find(" ---- ") - 6));
		map.push_back(std::make_pair(texture, Vector2{ textureX, textureY }));
		scaleVector.push_back(scale);
		buttonTexture.push_back("N/A");
	}
	Drag.setTextureMap(map);
	Drag.setScaleArray(scaleVector);
	Drag.setButtonArray(buttonTexture);
	file.close();
	return true;
}