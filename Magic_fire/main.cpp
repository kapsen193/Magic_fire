#include "raylib.h"
#include <iostream>
using namespace std;
// перечисление игровых экранов
enum GameScreen {MENU,EDUCATION,SETTINGS};
int main()  
{	
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(1920, 1080, "Magic Fire Game");
	SetTargetFPS(60);
	GameScreen currentScreen = MENU;
	Rectangle playBtnRect = { 830,585,150,60 };
	Rectangle settingsBtnRect = { 790,635,200,60 };


	while (!WindowShouldClose()){
		Vector2 mousepoint = GetMousePosition();
		if (currentScreen == MENU) {
			if (CheckCollisionPointRec(mousepoint, playBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = EDUCATION;
			}
		}
		else if (currentScreen == EDUCATION) {
			if (IsKeyPressed(KEY_ESCAPE)) {
				currentScreen = MENU;
			}
		}



		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		if (currentScreen == MENU) {
			DrawText("FIRE MATH", 620, 400, 100, DARKPURPLE);
			DrawText("PLAY", 850, 600, 30, DARKGRAY);
			DrawText("SETTINGs", 810, 650, 30, DARKGRAY);
		}
		else if (currentScreen == EDUCATION) {
			DrawText("WRITE A MATH FUNCTION TO SHOT", 560, 400, 30, DARKPURPLE);
		}

		EndDrawing();
	}

	CloseWindow();



};