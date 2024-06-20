#include "FilesystemFunction.hpp"

#include "../RaylibFunctions.h"
#include "../Functions.h"

#include <RaylibAdditions.hpp>
#include <GUI.hpp>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <tinyfiledialogs.h>
#include <filesystem>

std::string FileSystemFunctions::setSaveLocation() {
	const char *filterPatterns[] = { "*.gui" };
	const char *saveFileName = tinyfd_saveFileDialog(
		"Save GUI",
		"",
		1,
		filterPatterns,
		NULL);

	if (saveFileName == NULL || strlen(saveFileName) == 0) {
        std::cout << "Save dialog was cancelled or an error occurred." << std::endl;
        return "";
    }

    return std::string(saveFileName);
}

std::string FileSystemFunctions::getSaveLocation() {
	const char *filterPatterns[] = { "*.gui" };
	const char *saveFileName = tinyfd_openFileDialog(
		"Load GUI",
		"",
		1,
		filterPatterns,
		NULL,
		false);

	if (saveFileName == NULL || strlen(saveFileName) == 0) {
        std::cout << "Save dialog was cancelled or an error occurred." << std::endl;
        return "";
    }

    return std::string(saveFileName);
}

void FileSystemFunctions::exportToFile(Rectangle rec, std::string& filename) {
	std::ofstream file;
	std::filesystem::path filePath = filename;

	if (filePath.extension() != ".gui")
		file.open(filename + ".gui");
	else
		file.open(filename);

	file << rec.width << ":" << rec.height << std::endl;
	file << "[frame:0]\n";
	for (int i = 0; i < Drag.mapCount(); i++) {
		auto it = std::next(Drag.getTextureMap()->begin() , i);
		if (Drag.getButtonTexture(i) == "N/A")
			file << "\t" << i << " ----- " << it->first << " ----- " << it->second.x << " ----- " << it->second.y << " ----- " << "x" << Drag.getScale(i) << std::endl;
	}
	file << "[buttons]";
	for (int i = 0; i < Drag.mapCount(); i++) {
		auto it = std::next(Drag.getTextureMap()->begin(), i);
		if (Drag.getButtonTexture(i) != "N/A")
			file << "\t" << i << " ----- " << it->first << " ----- " << Drag.getButtonTexture(i) << " ----- " << it->second.x << " ----- " << it->second.y << " ----- " << " ----- " << Drag.getScale(i) << std::endl;
	}
	file.close();
}

bool FileSystemFunctions::importFromFile(std::string& filename) { // move this/create a better loader in RaylibAdditions and include it here instead
	Vector2 size{ 0,0 };
	std::vector<std::string> textureName;
	std::vector<Vector2> texturePos;
	std::vector<float> scaleVector;
	std::vector<std::string> buttonTexture;

	RaylibAdditions::RoomClass room;
	RaylibAdditions::LoadedRoomClass loadedRoom = room.loadRoom("", filename);
	
	std::ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		return false;
	}
	std::string line;
	int i = 0;
	while (std::getline(file, line)) { 

		if (i == 0) {
			size.x = stof(line.substr(0, line.find(':')));
			size.y = stof(line.substr(line.find(':') + 1));
		}

		if (line.find("-----") != std::string::npos) {
			std::string afterTextureSplit = line.substr(line.find("-----") + 6);
			std::string texture = afterTextureSplit.substr(0, afterTextureSplit.find("-----") - 1);
			textureName.push_back(texture);
		}
		i++;
	}

	for (auto& frame : loadedRoom.frames) { // only 1 frame works for now
		for (auto& frameTexturePos : frame.texturePos) {
			texturePos.push_back(frameTexturePos);
		}
		for (auto& frameTextureScale : frame.textureScales) {
			scaleVector.push_back(frameTextureScale);
		}
	}

	buttonTexture.push_back("N/A");

	i = 0;
	std::vector<std::pair<std::string, Vector2>> map;
	for (auto &string : textureName) {
		map.push_back({string, texturePos[i]});
		i++;
	}

	MainRoom.size = size;
	Drag.setTextureMap(map);
	Drag.setScaleArray(scaleVector);
	Drag.setButtonArray(buttonTexture);
	file.close();
	return true;
}