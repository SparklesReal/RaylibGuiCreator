#pragma once
#include <raylib.h>

#include <GUI.hpp>
#include "../RaylibFunctions.h"
#include "../menus/room.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

namespace FileSystemFunctions {
	std::string setSaveLocation();
	std::string getSaveLocation();
	void exportToFile(Rectangle rec, std::string& filename, std::vector<DragSystem>* Frames);
	std::pair<std::vector<RaylibAdditions::FrameClass>, std::vector<std::vector<std::string>>> importFromFile(std::string& filename);
};