#include "Functions.h"

#include <string>

bool FunctionClass::stringIsInt(std::string string) {
	for (int i = 0; i < string.length(); i++) {
		if (isdigit(string[i]) == false) {
			return false;
		}
	}
	return true;
}