#include "Functions.h"

bool NormalFunctions::stringIsInt(std::string string) {
	if (string.length() == 0) return false;
	for (int i = 0; i < string.length(); i++) {
		if (isdigit(string[i]) == false) {
			return false;
		}
	}
	return true;
}

size_t NormalFunctions::findMutipleChar(std::string string, char charToFind, int amount) { // use .find instead just here to not destroy anything or if I make a improved ver
	size_t index = string.find_first_of(charToFind);
	for (int i = 1; i < amount; i++) {
		if (string.at(index + i) != charToFind)
			return std::string::npos;
	}
	return index + static_cast<size_t>(amount);
}
