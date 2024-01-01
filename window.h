#pragma once
#include <string>

class ProgramWindowClass {
public:
	std::string title = "GUI creator for Raylib";
	int width;
	int height;

	ProgramWindowClass() {
		width = 1920;
		height = 1080;
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