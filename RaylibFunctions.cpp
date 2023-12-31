#include "RaylibFunctions.h"
#include "window.h"

#include <raymath.h>
#include <filesystem>
#include <iostream>
#include <fstream>

int RaylibFunctionsClass::drawTextRectCenter(Rectangle rect, std::string text, int size, Color color) {
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

int RaylibFunctionsClass::drawButtonRect(Rectangle rect, std::string text, int size, Color rectangleColor, Color outlineColor, int outlineThickness) {
	DrawRectangleRec(rect, rectangleColor);
	DrawRectangleLinesEx(rect, outlineThickness, outlineColor);
	drawTextRectCenter(rect, text, size, BLACK);
	return 0;
} // turn into a void function

Camera2D RaylibFunctionsClass::createCamera() {
	Camera2D camera{ 0 };
	camera.target = Vector2{ 0, -100 };
	camera.offset = Vector2{ 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return camera;
}

Camera2D RaylibFunctionsClass::updateCamera(Camera2D camera) { // Todo: System seems to be laggy when going diagonal, try to debug
	if (IsKeyDown(KEY_R)) {
		camera.target = Vector2{ 0, -100 };
		camera.zoom = 1.0f;
	}

	if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W))
		camera.target = Vector2Add(camera.target, Vector2{ -1.0f / camera.zoom / 2, -1.0f / camera.zoom / 2});
	else if (IsKeyDown(KEY_A) && IsKeyDown(KEY_S))
		camera.target = Vector2Add(camera.target, Vector2{ -1.0f / camera.zoom / 2, 1.0f / camera.zoom / 2 });
	else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_W))
		camera.target = Vector2Add(camera.target, Vector2{ 1.0f / camera.zoom / 2, -1.0f / camera.zoom / 2 });
	else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S))
		camera.target = Vector2Add(camera.target, Vector2{ 1.0f / camera.zoom / 2, 1.0f / camera.zoom / 2 });

	else if (IsKeyDown(KEY_A))			camera.target = Vector2Add(camera.target, Vector2{ -1.0f / camera.zoom, 0.0f });
	else if (IsKeyDown(KEY_D))			camera.target = Vector2Add(camera.target, Vector2{ 1.0f / camera.zoom, 0.0f });
	else if (IsKeyDown(KEY_W))			camera.target = Vector2Add(camera.target, Vector2{ 0.0f, -1.0f / camera.zoom });
	else if (IsKeyDown(KEY_S))			camera.target = Vector2Add(camera.target, Vector2{ 0.0f, 1.0f / camera.zoom });
	if (GetMouseWheelMove() > 0)	camera.zoom += 0.05f;
	if (GetMouseWheelMove() < 0)	camera.zoom -= 0.05f;
	return camera;
}

std::unordered_map<std::string, Texture2D> RaylibFunctionsClass::loadTextures() { //Todo load all files from folders aswell //Todo change to raylib system :skull:
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

Texture2D* RaylibFunctionsClass::stringToTexture(std::string texture) {
	return &TextureMap.getTextureMap()->at(texture);
}

int RaylibFunctionsClass::getAmountOfPages() {
	int pages = TextureMap.getTextureMap()->size() / 5;
	if (TextureMap.getTextureMap()->size() % 5 != 0)
		pages++;
	return pages;
}

std::vector<std::string> RaylibFunctionsClass::getUITextures(int pageNum) {
	std::vector<std::string> CurrentUI; // Rename varible
	std::unordered_map<std::string, Texture2D>* textureMap = TextureMap.getTextureMap();
	for (int i = 5 * pageNum - 5; i < 5 * pageNum; i++) {
		auto it = std::next(textureMap->begin(), i);
		if (it == textureMap->end())
			break;
		CurrentUI.push_back(it->first);
	}
	return CurrentUI;
}

void RaylibFunctionsClass::drawUI(std::vector<std::string> UI, Rectangle UIRects[], size_t arraySize, int pageNum, Vector2 triangles[6]) {
	for (int i = 0; i < arraySize; i++) {
		if (i == UI.size())
			break; 

		DrawRectangleLinesEx(UIRects[i], 1, RAYWHITE);
		if (i * pageNum < TextureMap.getTextureMap()->size()) {
			int scale;
			if (stringToTexture(UI[i])->height > stringToTexture(UI[i])->width) {
				scale = 200 / stringToTexture(UI[i])->height;
			}
			else {
				scale = 200 / stringToTexture(UI[i])->width;
			}
			DrawTextureEx(*stringToTexture(UI[i]), Vector2{ UIRects[i].x + 1, UIRects[i].y + 1}, 0, scale, WHITE);
		}
	}

	Rectangle belowUI = { 0, 1000, 200, 80 }; // xPos, yPos, RecWidth, RecHeight
	drawTextRectCenter(belowUI, std::to_string(pageNum) + "/" + std::to_string(getAmountOfPages()), 25, RAYWHITE);
	DrawTriangle(triangles[0], triangles[1], triangles[2], GRAY);
	DrawTriangle(triangles[3], triangles[4], triangles[5], GRAY);
}

Rectangle RaylibFunctionsClass::drawRightClickMenu(std::string textureString, Vector2* TexturePos, Camera2D* camera, std::vector<std::string> UI, Rectangle UIRects[5]) {
	Texture2D* texture = stringToTexture(textureString);
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *camera);
	Rectangle menuRec{ TexturePos->x + texture->width + 6, TexturePos->y + 6, 23, 48 }; // xPos, yPos, RecWidth, RecHeight
	Rectangle menuRecOutline{ TexturePos->x + texture->width + 5, TexturePos->y + 5, 25, 50 };
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
		Drag.removeElementByString(textureString);
	}

	if (CheckCollisionPointRec(mousePos, makeButtonButton) && IsMouseButtonPressed(0)) {
		Drag.setButton(textureString, UI, UIRects);
	}

	return menuRecOutline;
}

// Gonna code grabby grab system. this is gonna take 5 months atleast :skull: //Oh god I hope this repo never becomes public
Rectangle rightClickUI{ 0,0,0,0 }; // This is stupid
std::string lastTexture = "";
Vector2 lastTexturePos{ 0, 0 };
void DragSystem::update(std::vector<std::string> UI, Rectangle UIRects[5], Rectangle area, Camera2D camera) { // This might be super smart or stupid 
	if (IsMouseButtonReleased(0) && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), area) && textureHeld != "") {
		Texture2D texture = *Functions.stringToTexture(textureHeld);
		Vector2 mousePos = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), camera), { 5,5 }); // 5,5 due to outline // make it a var or something to not need comments
		mousePos.x = std::round(mousePos.x);
		mousePos.y = std::round(mousePos.y);
		Vector2 vector = Vector2Subtract({ area.width, area.height }, Vector2Add(mousePos, { float(texture.width * scale), float(texture.height * scale) }));
		if (vector.x > 0 && vector.y > 0) {
			textureMap.push_back(std::pair<std::string, Vector2>{textureHeld, mousePos});
			scales.push_back(scale);
			buttonTexture.push_back("N/A");
		}
	}

	if (IsMouseButtonUp(0)) {
		textureHeld = "";
		scale = 1;
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
		rightClickUI = Functions.drawRightClickMenu(lastTexture, &lastTexturePos, &camera, UI, UIRects);
	}

	if (textureHeld != ""  || !CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), rightClickUI)) {
		rightClickUI = { 0,0,0,0 };
		lastTexture = "";
		lastTexturePos = { 0, 0 };
	}

	if (textureHeld == "" && IsMouseButtonDown(1) && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), area) ) {
		for (int i = 0; i < textureMap.size(); i++) {
			auto it = std::next(textureMap.begin(), i);
			Texture2D* texture = Functions.stringToTexture(it->first);
			if (CheckCollisionPointRec({ GetScreenToWorld2D(GetMousePosition(), camera).x - 5, GetScreenToWorld2D(GetMousePosition(), camera).y - 5 }, // I don't know why we do -5, I have been trying with +5 and that mistake costed me 2 hours and my motivation
										{ it->second.x, it->second.y, float(texture->width), float(texture->height) })) {
				rightClickUI = Functions.drawRightClickMenu(it->first, &it->second, &camera, UI, UIRects);
				lastTexture = it->first;
				lastTexturePos = it->second;
				break;
			}
		}
	}

	if (textureHeld != "") {
		Texture2D* texture = Functions.stringToTexture(textureHeld);
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

	BeginMode2D(camera);
	for (std::size_t i = 0; i < textureMap.size(); i++) {
		auto it = std::next(textureMap.begin(), i);
		DrawTextureEx(*Functions.stringToTexture(it->first), { std::round(area.x + it->second.x), std::round(area.y + it->second.y)}, 0, scales[i], RAYWHITE);
	}
	EndMode2D();
} // Wow that only took a day or two

void DragSystem::setButton(std::string texture, std::vector<std::string> UI, Rectangle UIRects[5]) {
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
	auto it = std::find_if(Drag.getTextureMap()->begin(), Drag.getTextureMap()->end(),
		[&](const auto& pair) { return pair.first == texture; });
	size_t index = std::distance(textureMap.begin(), it);
	buttonTexture[index] = textureHeld;
}

void FileSystem::exportToFile(Rectangle rec) {
	std::ofstream file;
	file.open("Export.gui"); // Add system to not edit alredy existing files
	file << rec.width << ":" << rec.height << std::endl;
	file << "Frame0:\n";
	for (int i = 0; i < Drag.mapCount(); i++) {
		auto it = std::next(Drag.getTextureMap()->begin() , i);
		if (Drag.getButtonTexture(i)->c_str() == "N/A")
			file << "\t" << i << ". " << it->first << " - " << it->second.x << "," << it->second.y << " " << ": x" << Drag.getScale(i) << std::endl;
	}
	file << "---Buttons---";
	for (int i = 0; i < Drag.mapCount(); i++) {
		auto it = std::next(Drag.getTextureMap()->begin(), i);
		if (Drag.getButtonTexture(i)->c_str() != "N/A")
			file << "\t" << i << ". " << it->first << " " << Drag.getButtonTexture(i) << " - " << it->second.x << "," << it->second.y << " " << ": x" << Drag.getScale(i) << std::endl;
	}
	file.close();
}

Vector2 FileSystem::importFromFile() {
	std::ifstream file;
	file.open("Export.gui"); // Create something to select file
	std::string line;
	int i = 0;
	Vector2 size{ 0,0 };
	while (std::getline(file, line)) {
		i++;
		std::cout << line << std::endl;
		if (i == 1) {
			int index = line.find(":");
			std::string Xstring = line.substr(0, index);
			std::string Ystring = line.substr(index, line.length());
			size = { std::stof(Xstring), std::stof(Ystring) };
		}

	}
	file.close();
	return size;
}
