#define MAX_KEYS 512

#include "RaylibFunctions.h"

#include "window.h"
#include "Functions.h"
#include "menus/room.h"

#include <raymath.h>
#include <filesystem>
#include <iostream>
#include <fstream>

int RaylibFunctions::drawTextRectCenter(Rectangle rect, std::string text, int size, Color color) {
	DrawTextEx(
		GetFontDefault(),
		text.c_str(), 
		Vector2{ rect.x + (rect.width - MeasureTextEx(GetFontDefault(), text.c_str(), size, 10).x) / 2, // xPos + (width - textWidth) / 2
		rect.y + (rect.height - size) / 2},																// yPos + (height - size) / 2 ;note we remove the size / 2 because text prints from the middle of the position
		size, 
		10,
		color);
	return 0;
} // turn into a void function

void RaylibFunctions::drawButtonRect(Rectangle rect, std::string text, int size, Color rectangleColor, Color outlineColor, int outlineThickness) {
	DrawRectangleRec(rect, rectangleColor);
	DrawRectangleLinesEx(rect, outlineThickness, outlineColor);
	drawTextRectCenter(rect, text, size, BLACK);
}

void RaylibFunctions::drawButtonRect(ButtonClass *button) {
	drawButtonRect(button->rect, button->text, button->textSize, button->color, button->outlineColor, button->outlineThickness);
}

Camera2D RaylibFunctions::createCamera() {
	Camera2D camera{ 0 };
	camera.target = Vector2{ 0, -100 };
	camera.offset = Vector2{ 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return camera;
}

Camera2D RaylibFunctions::updateCamera(Camera2D camera, float speed) { // Todo: System seems to be laggy when going diagonal, try to debug
	if (IsKeyDown(KEY_R)) {
		camera.target = Vector2{ 0, -100 };
		camera.zoom = 1.0f;
	}

	if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W))
		camera.target = Vector2Add(camera.target, Vector2{ (-1.0f / camera.zoom / 2) * speed, (- 1.0f / camera.zoom / 2) * speed});
	else if (IsKeyDown(KEY_A) && IsKeyDown(KEY_S))
		camera.target = Vector2Add(camera.target, Vector2{ (-1.0f / camera.zoom / 2) * speed, (1.0f / camera.zoom / 2) * speed});
	else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_W))
		camera.target = Vector2Add(camera.target, Vector2{ (1.0f / camera.zoom / 2) * speed, (-1.0f / camera.zoom / 2) * speed });
	else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S))
		camera.target = Vector2Add(camera.target, Vector2{ (1.0f / camera.zoom / 2) * speed, (1.0f / camera.zoom / 2) * speed });

	else if (IsKeyDown(KEY_A))			camera.target = Vector2Add(camera.target, Vector2{ -1.0f / camera.zoom, 0.0f });
	else if (IsKeyDown(KEY_D))			camera.target = Vector2Add(camera.target, Vector2{ 1.0f / camera.zoom, 0.0f });
	else if (IsKeyDown(KEY_W))			camera.target = Vector2Add(camera.target, Vector2{ 0.0f, -1.0f / camera.zoom });
	else if (IsKeyDown(KEY_S))			camera.target = Vector2Add(camera.target, Vector2{ 0.0f, 1.0f / camera.zoom });
	if (GetMouseWheelMove() > 0)	camera.zoom += 0.05f;
	if (GetMouseWheelMove() < 0)	camera.zoom -= 0.05f;
	return camera;
}

std::unordered_map<std::string, Texture2D> RaylibFunctions::loadTextures() { //Todo load all files from folders aswell //Todo change to raylib system :skull:
	std::unordered_map<std::string, Texture2D> returnMap;

	std::filesystem::path path{ "./textures" };

	if (!std::filesystem::exists(path)) {
		std::cout << "Could not find textures folder, please create one! \n";
		return std::unordered_map<std::string, Texture2D>();
	}

	for (const auto & entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string filePath = entry.path().string();
			std::string fileName = entry.path().filename().string();
			size_t dotPos = fileName.find_last_of('.');

			if (dotPos != std::string::npos) {
				std::string fileType = fileName.substr(dotPos + 1);
				if (fileType == "png") {
					Texture2D texture = LoadTexture(filePath.c_str());
					returnMap.insert({ fileName.substr(0, dotPos) , texture });
				}
			}
		}
	}

	return returnMap;
}

Texture2D* RaylibFunctions::stringToTexture(std::string texture) {
	return &TextureMap.getTextureMap()->at(texture);
}

Texture2D* RaylibFunctions::numToTexture(int num) { // This is stupid I think cus why would I need it this should be at drag class
	auto it = TextureMap.getTextureMap()->begin();
	std::advance(it, num);
	if (it != TextureMap.getTextureMap()->end())
		return &it->second;
	return nullptr;
}

void RaylibFunctions::drawButtonMap(std::unordered_map<std::string, ButtonClass> *buttons) {
	for (int i = 0; i < buttons->size(); i++) {
		auto it = std::next(buttons->begin(), i);
		RaylibFunctions::drawButtonRect(&it->second);
	}
}

void RaylibFunctions::updateButtonStates(std::unordered_map<std::string, ButtonClass> *buttons) {
	for (int i = 0; i < buttons->size(); i++) {
		auto it = std::next(buttons->begin(), i);
		if (CheckCollisionPointRec(GetMousePosition(), it->second.rect)) {
			it->second.state = 1;
			if (IsMouseButtonPressed(0))
				it->second.state = 2;
		}
		else
			it->second.state = 0;
	}
}

void RaylibFunctions::updateButtonStates(std::unordered_map<std::string, ButtonClass>* buttons, Camera2D camera) {
	for (int i = 0; i < buttons->size(); i++) {
		auto it = std::next(buttons->begin(), i);
		if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), it->second.rect)) {
			it->second.state = 1;
			if (IsMouseButtonPressed(0))
				it->second.state = 2;
		}
		else
			it->second.state = 0;
	}
}

int RaylibFunctions::getAmountOfPages() {
	int pages = TextureMap.getTextureMap()->size() / 5;
	if (TextureMap.getTextureMap()->size() % 5 != 0)
		pages++;
	return pages;
}

std::vector<std::string> RaylibFunctions::getUITextures(int pageNum) {
	std::vector<std::string> UI; // Rename varible // why doe? // Sure UI it is then
	std::unordered_map<std::string, Texture2D>* textureMap = TextureMap.getTextureMap();
	for (int i = 5 * pageNum - 5; i < 5 * pageNum; i++) {
		auto it = std::next(textureMap->begin(), i);
		if (it == textureMap->end())
			break;
		UI.push_back(it->first);
	}
	return UI;
}

void RaylibFunctions::drawUI(std::vector<std::string> UI, Rectangle UIRects[], size_t arraySize, int pageNum, Vector2 triangles[6]) {
	for (int i = 0; i < arraySize; i++) {
		if (i == UI.size())
			break; 

		DrawRectangleLinesEx(UIRects[i], 1, RAYWHITE);
		if (i * pageNum < TextureMap.getTextureMap()->size()) {
			float scale;
			if (stringToTexture(UI[i])->height > stringToTexture(UI[i])->width) {
				scale = 200.0 / stringToTexture(UI[i])->height;
			}
			else {
				scale = 200.0 / stringToTexture(UI[i])->width;
			}
			DrawTextureEx(*stringToTexture(UI[i]), Vector2{ UIRects[i].x + 1, UIRects[i].y + 1}, 0, scale, WHITE);
		}
	}

	Rectangle belowUI = { 0, 1000, 200, 80 }; // xPos, yPos, RecWidth, RecHeight
	drawTextRectCenter(belowUI, std::to_string(pageNum) + "/" + std::to_string(getAmountOfPages()), 25, RAYWHITE);
	DrawTriangle(triangles[0], triangles[1], triangles[2], GRAY);
	DrawTriangle(triangles[3], triangles[4], triangles[5], GRAY);
}

Rectangle RaylibFunctions::drawRightClickMenu(int textureNum, Vector2* TexturePos, Camera2D* camera, std::vector<std::string> UI, Rectangle UIRects[5]) {
	Texture2D* texture = Drag.getTextureByNum(textureNum);
	if (texture == nullptr)
		return { 0,0,0,0 };
	float textureScale = Drag.getScale(textureNum);
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *camera);
	Rectangle menuRec{ TexturePos->x + (texture->width * textureScale) + 6, TexturePos->y + 6, 23, 48 }; // xPos, yPos, RecWidth, RecHeight
	Rectangle menuRecOutline{ TexturePos->x + (texture->width * textureScale) + 5, TexturePos->y + 5, 25, 50 };
	BeginMode2D(*camera);
	DrawRectangleLinesEx(menuRecOutline, 1, GRAY);
	DrawRectangleRec(menuRec, RAYWHITE);

	Rectangle deleteButton{ menuRec.x, menuRec.y, menuRec.width, 7.5f };
	DrawRectangleRec(deleteButton, LIGHTGRAY);
	DrawRectangleLinesEx(deleteButton, 0.5f, RED);
	DrawTextEx(GetFontDefault(), "Delete", { deleteButton.x + 0.5f, deleteButton.y + 0.5f}, 7, 0.5f, BLACK);

	Rectangle makeButtonButton{ menuRec.x, menuRec.y + deleteButton.height, menuRec.width, 7.5f};
	DrawRectangleRec(makeButtonButton, LIGHTGRAY);
	DrawRectangleLinesEx(makeButtonButton, 0.5f, RED);
	DrawTextEx(GetFontDefault(), "Button", { makeButtonButton.x + 0.5f, makeButtonButton.y + 0.5f }, 7, 0.5f, BLACK);
	EndMode2D();

	if (CheckCollisionPointRec(mousePos, deleteButton) && IsMouseButtonPressed(0)) {
		Drag.removeElementByNum(textureNum);
	}

	if (CheckCollisionPointRec(mousePos, makeButtonButton) && IsMouseButtonPressed(0)) {
		Drag.setButton(textureNum, UI, UIRects);
	}

	return menuRecOutline;
}

bool RaylibFunctions::allKeysReleased() {
	for (int i = 0; i < MAX_KEYS; i++) {
		if (IsKeyDown(i)) 
			return false;
	}
	return true;
}

Texture2D* DragSystem::getTextureByNum(int num) {
	auto it = Drag.getTextureMap()->begin();
	std::advance(it, num);
	if (it != Drag.getTextureMap()->end())
		return RaylibFunctions::stringToTexture(it->first);
	return nullptr;
}

// Gonna code grabby grab system. this is gonna take 5 months atleast :skull: //Oh god I hope this repo never becomes public
Rectangle rightClickUI{ 0,0,0,0 }; // This is stupid
int lastTexture = 0;
Vector2 lastTexturePos{ 0, 0 };
void DragSystem::update(std::vector<std::string> UI, Rectangle UIRects[5], Rectangle area, Camera2D camera) { // This might be super smart or stupid 

	BeginMode2D(camera);
	for (std::size_t i = 0; i < textureMap.size(); i++) {
		auto it = std::next(textureMap.begin(), i);
		DrawTextureEx(*RaylibFunctions::stringToTexture(it->first), { std::round(area.x + it->second.x), std::round(area.y + it->second.y) }, 0, scales[i], RAYWHITE);
	}
	EndMode2D();
	
	if (IsMouseButtonReleased(0) && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), area) && textureHeld != "") {
		Texture2D texture = *RaylibFunctions::stringToTexture(textureHeld);
		Vector2 mousePos = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), camera), { 5,5 }); // 5,5 due to outline // make it a var or something to not need comments
		mousePos.x = std::round(mousePos.x);
		mousePos.y = std::round(mousePos.y);
		Vector2 vector = Vector2Subtract({ area.width, area.height }, Vector2Add(mousePos, { float(texture.width * scale), float(texture.height * scale) }));
		if (vector.x >= 0 && vector.y >= 0) {
			textureMap.push_back(std::pair<std::string, Vector2>{textureHeld, mousePos});
			scales.push_back(scale);
			buttonTexture.push_back("N/A");
		}
	}

	if (IsMouseButtonReleased(0)) {
		textureHeld = "";
		scale = 1;
		std::cout << "test";
	}

	if (textureHeld == "" && IsMouseButtonPressed(0)) {
		for (int i = 0; i < 5; i++) {
			if (i >= UI.size())
				break;
			if (CheckCollisionPointRec(GetMousePosition(), UIRects[i]))
				textureHeld = UI[i];
		}
	}

	if (textureHeld == "" && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), rightClickUI)) {
		rightClickUI = RaylibFunctions::drawRightClickMenu(lastTexture, &lastTexturePos, &camera, UI, UIRects);
	}

	if (textureHeld != ""  || !CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), rightClickUI)) {
		rightClickUI = { 0,0,0,0 };
		lastTexture = 0;
		lastTexturePos = { 0, 0 };
	}

	if (textureHeld == "" && IsMouseButtonDown(1) && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), area) ) {
		for (int i = 0; i < textureMap.size(); i++) {
			auto it = std::next(textureMap.begin(), i);
			Texture2D* texture = Drag.getTextureByNum(i);
			float scale = getScale(i);
			if (CheckCollisionPointRec({ GetScreenToWorld2D(GetMousePosition(), camera).x , GetScreenToWorld2D(GetMousePosition(), camera).y },
										{ it->second.x + 5, it->second.y + 5, float(texture->width * scale), float(texture->height * scale) })) {  // I don't know why we do -5, I have been trying with +5 and that mistake costed me 2 hours and my motivation // well put it at the mouse pos not the rect pos, im dumb // * 1.25 idk why // Tip: don't take textures from the wrong texture map // This is the worst line of code with the most bugs and the most comments, please just delete this the pain I suffered alredy is enough
				rightClickUI = RaylibFunctions::drawRightClickMenu(i, &it->second, &camera, UI, UIRects);
				lastTexture = i;
				lastTexturePos = it->second;
				break;
			}
		}
	}

	if (textureHeld != "") {
		Texture2D* texture = RaylibFunctions::stringToTexture(textureHeld);
		DrawTextureEx(*texture, GetMousePosition(), 0, scale * camera.zoom, RAYWHITE);
		if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), area)) {
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
			Rectangle rect{std::round(mousePos.x), std::round(mousePos.y), texture->width * scale, texture->height * scale}; // xPos, yPos, RecWidth, RecHeight
			BeginMode2D(camera);
			DrawRectangleLinesEx(rect, 1, RAYWHITE);
			EndMode2D();
		}
	}

	if (IsKeyPressed(KEY_UP))
		scale += 0.5;

	if (IsKeyPressed(KEY_DOWN) && scale > 0.5)
		scale -= 0.5;
} // Wow that only took a day or two

void DragSystem::setButton(int texture, std::vector<std::string> UI, Rectangle UIRects[5]) {
	std::string textureHeld = "";
	while (textureHeld == "") {
		if (IsMouseButtonPressed(1)) {
			for (int i = 0; i < 5; i++) {
				if (i >= UI.size())
					break;
				if (CheckCollisionPointRec(GetMousePosition(), UIRects[i])) {
					textureHeld = UI[i];
					break;
				}
			}
		}
	}
	auto it = Drag.getTextureMap()->begin();
	std::advance(it, texture);
	buttonTexture[texture] = textureHeld;
}

void DragSystem::removeElementByNum(const int num) {
	auto it = Drag.getTextureMap()->begin();
	std::advance(it, num);
	if (it != Drag.getTextureMap()->end())
		textureMap.erase(it);
	if (num < scales.size())
		scales.erase(scales.begin() + num);
	if (num < buttonTexture.size())
		buttonTexture.erase(buttonTexture.begin() + num);
}