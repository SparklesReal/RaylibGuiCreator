#pragma once
#include <string>

class ProgramWindowClass {
public:
	std::string title;
	int width;
	int height;

	ProgramWindowClass(int w, int h, std::string t) : width(w), height(h), title(t) {
		InitWindow(width, height, title.c_str());
	};

	~ProgramWindowClass() {
		CloseWindow();
	}

	void updateWindow() {
		SetWindowTitle(title.c_str());
		SetWindowSize(width, height);
	}
};

extern ProgramWindowClass Window;