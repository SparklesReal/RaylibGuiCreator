#include <raylib.h>

#include <string>

namespace FileSystemFunctions {
	void exportToFile(Rectangle rec, std::string& filename);
	bool importFromFile(std::string& filename);
};