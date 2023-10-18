#pragma once
#include <string>

class WindowClass {
private:
	std::string title	= "GUI creator for Raylib";
	int width			= 1920;
	int height			= 1080;
public:
	std::string getTitle()	{ return title; }
	int getWidth()			{ return width; }
	int getHeight()			{ return height; }

	void setWidth(int arg)	{ width = arg; }
	void setHeight(int arg) { height = arg; }
};

extern WindowClass Window;