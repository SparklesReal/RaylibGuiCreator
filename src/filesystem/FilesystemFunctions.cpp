#include "FilesystemFunction.hpp"

#include "../Functions.h"

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

void FileSystemFunctions::exportToFile(Rectangle rec, std::string& filename, std::vector<DragSystem>* Frames) {
	std::ofstream file;
	std::filesystem::path filePath = filename;

	if (filePath.extension() != ".gui")
		file.open(filename + ".gui");
	else
		file.open(filename);

	file << rec.width << ":" << rec.height << std::endl;

	for (int i = 0; i < Frames->size(); i++) {
		file << "[frame:" << i << "]\n";
		for (int j = 0; j < Frames->at(i).mapCount(); j++) {
			auto it = std::next(Frames->at(i).getTextureMap()->begin() , j);
			if (Frames->at(i).getButtonTexture(j) == "N/A")
				file << "\t" << j << " ----- " << it->first << " ----- " << it->second.x << " ----- " << it->second.y << " ----- " << "x" << Frames->at(i).getScale(j) << std::endl;
		}
	}

	file << "[buttons]";
	//for (int i = 0; i < Drag.mapCount(); i++) {
	//	auto it = std::next(Drag.getTextureMap()->begin(), i);
	//	if (Drag.getButtonTexture(i) != "N/A")
	//		file << "\t" << i << " ----- " << it->first << " ----- " << Drag.getButtonTexture(i) << " ----- " << it->second.x << " ----- " << it->second.y << " ----- " << " ----- " << Drag.getScale(i) << std::endl;
	//}
	file.close();
}

std::pair<std::vector<RaylibAdditions::FrameClass>, std::vector<std::vector<std::string>>> FileSystemFunctions::importFromFile(std::string& filename) { // move this/create a better loader in RaylibAdditions and include it here instead
	Vector2 size{ 0,0 };
	std::vector<std::vector<std::string>> textureName;
	std::vector<Vector2> texturePos;
	std::vector<float> scaleVector;
	std::vector<std::string> buttonTexture;

	RaylibAdditions::RoomClass room;
	RaylibAdditions::LoadedRoomClass loadedRoom = room.loadRoom("", filename);
	
	std::ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		return std::pair<std::vector<RaylibAdditions::FrameClass>, std::vector<std::vector<std::string>>>();
	}
	std::string line;
	int i = 0;
	int frame = 0;
	int lastFrame = 0;
	std::vector<std::string> textureNameCurrent;
	while (std::getline(file, line)) { 

		if (i == 0) {
			size.x = stof(line.substr(0, line.find(':')));
			size.y = stof(line.substr(line.find(':') + 1));
			i++;
			continue;
		}

		if (line.find("[") != std::string::npos && line.find("]") != std::string::npos && line.find("frame:") != std::string::npos) {
			frame = stoi(line.substr(line.find(":") + 1, line.length() - line.find(":") - 2));
		}

		if (frame != lastFrame) {
			textureName.push_back(textureNameCurrent);
			textureNameCurrent.clear();
		}

		if (line.find("-----") != std::string::npos) {
			std::string afterTextureSplit = line.substr(line.find("-----") + 6);
			std::string texture = afterTextureSplit.substr(0, afterTextureSplit.find("-----") - 1);
			textureNameCurrent.push_back(texture);
		}
		lastFrame = frame;
	}
	for (auto& currenTexture : textureNameCurrent) {
		if (!std::filesystem::exists("./textures/" + currenTexture + ".png"))
			return std::pair<std::vector<RaylibAdditions::FrameClass>, std::vector<std::vector<std::string>>>();
	}
	textureName.push_back(textureNameCurrent);
	textureNameCurrent.clear();

	std::vector<RaylibAdditions::FrameClass> frameList;
	i = 0;
	for (RaylibAdditions::FrameClass& frame : loadedRoom.frames) {
		frameList.push_back(RaylibAdditions::FrameClass());
		for (Vector2& frameTexturePos : frame.texturePos) {
			frameList.at(i).texturePos.push_back(frameTexturePos);
		}
		for (auto& frameTextureScale : frame.textureScales) {
			frameList.at(i).textureScales.push_back(frameTextureScale);
		}
		i++;
	}

	MainRoom.size = size;
	std::pair<std::vector<RaylibAdditions::FrameClass>, std::vector<std::vector<std::string>>> pairList = {frameList, textureName};
	file.close();
	return pairList;
}