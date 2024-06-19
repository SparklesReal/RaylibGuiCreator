#include <raylib.h>

#include <string>

namespace FileSystemFunctions {
	std::string setSaveLocation();
	std::string getSaveLocation();
	void exportToFile(Rectangle rec, std::string& filename);
	bool importFromFile(std::string& filename);
};