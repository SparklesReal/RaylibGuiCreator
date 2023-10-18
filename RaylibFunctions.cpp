#include "RaylibFunctions.h"

#include <raylib.h>
#include <raymath.h>

int RaylibFunctionsClass::drawTextRectCenter(Rectangle rect, std::string text, int size) {
	DrawTextEx(
		GetFontDefault(),
		text.c_str(), 
		Vector2{ rect.x + (rect.width - MeasureTextEx(GetFontDefault(), text.c_str(), size, 10).x) / 2, // xPos + (width - textWidth) / 2
		rect.y + (rect.height - size) / 2},																// yPos + (height - size) / 2 ;note we remove the size / 2 because text prints from the middle of the position
		size, 
		10,
		BLACK);
	return 0;
}

int RaylibFunctionsClass::drawButtonRect(Rectangle rect, std::string text, int size, Color rectangleColor, Color outlineColor, int outlineThickness) {
	DrawRectangleRec(rect, rectangleColor);
	DrawRectangleLinesEx(rect, outlineThickness, outlineColor);
	drawTextRectCenter(rect, text, size);
	return 0;
}

Camera2D RaylibFunctionsClass::createCamera() {
	Camera2D camera{ 0 };
	camera.target = Vector2{ 0, -100 };
	camera.offset = Vector2{ 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return camera;
}

Camera2D RaylibFunctionsClass::updateCamera(Camera2D camera) {
	if (IsKeyDown(KEY_R)) {
		camera.target = Vector2{ 0, -100 };
		camera.zoom = 1.0f;
	}

	if (IsKeyDown(KEY_A))			camera.target = Vector2Add(camera.target, Vector2{ -1.0f / camera.zoom, 0.0f });
	if (IsKeyDown(KEY_D))			camera.target = Vector2Add(camera.target, Vector2{ 1.0f / camera.zoom, 0.0f });
	if (IsKeyDown(KEY_W))			camera.target = Vector2Add(camera.target, Vector2{ 0.0f, -1.0f / camera.zoom });
	if (IsKeyDown(KEY_S))			camera.target = Vector2Add(camera.target, Vector2{ 0.0f, 1.0f / camera.zoom });
	if (GetMouseWheelMove() > 0)	camera.zoom += 0.05f;
	if (GetMouseWheelMove() < 0)	camera.zoom -= 0.05f;
	return camera;
}