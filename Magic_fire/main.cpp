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
	Rectangle playBtnRect = { 860,630,200,50 };
	Rectangle settingsBtnRect = {810,690,300, 50 };
	Rectangle backBtnRect = { 550, 430,120, 50 };

	Font titleFont = LoadFontEx("assets/Sheeppuff.otf", 100, 0 ,128);
	SetTextureFilter(titleFont.texture, TEXTURE_FILTER_ANISOTROPIC_16X);
	Font textFont = LoadFontEx("assets/Peppermint.otf",100,0,128);
	SetTextureFilter(textFont.texture, TEXTURE_FILTER_ANISOTROPIC_16X);
	Texture2D bg= LoadTexture("assets/bg1.png");
	//Texture2D bg1 = LoadTexture("assets");


	while (!WindowShouldClose()){
		Vector2 mousepoint = GetMousePosition();
		if (currentScreen == MENU) {
			if (CheckCollisionPointRec(mousepoint, playBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = EDUCATION;
		}
			if (CheckCollisionPointRec(mousepoint, settingsBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = SETTINGS;
			}
		}
		else if (currentScreen == EDUCATION) {
			if (IsKeyPressed(KEY_ESCAPE)) {
				currentScreen = MENU;
			}
		}
		else if (currentScreen == SETTINGS) {
			if (CheckCollisionPointRec(mousepoint, backBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = MENU;
			}
		}



		BeginDrawing();
		DrawTexturePro(
			bg,
			{ 0, 0, (float)bg.width, (float)bg.height }, 
			{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, 
			{ 0, 0 },
			0.0f,
			WHITE);
		
		if (currentScreen == MENU) {
			const char* title = "FIRE MATH";
			Vector2 titleSize = MeasureTextEx(titleFont, title, 100, 1);
			DrawTextEx(titleFont, title, { (1924 - titleSize.x) / 2.0f, 404 }, 100, 1, BLACK);
			DrawTextEx(titleFont, title, { (1920 - titleSize.x) / 2.0f, 400 }, 100, 1, { 230, 150, 50, 255 });


			
			const char* play = "PLAY";
			Vector2 playSize = MeasureTextEx(textFont, play, 50, 1);
			DrawTextEx(textFont, play, { (1920 - playSize.x) / 2.0f +12, 650},50, 1, {80, 60, 50, 255});
			const char* set = "SETTINGS";
			Vector2 setSize = MeasureTextEx(textFont, set, 50, 1);
			DrawTextEx(textFont, set, { (1920 - setSize.x) / 2.0f+12,700 },50, 1, { 80, 60, 50, 255 });
			
		}
		else if (currentScreen == EDUCATION) {
			DrawText("WRITE A MATH FUNCTION TO SHOT", 560, 400, 30, DARKPURPLE);
		}
		else if (currentScreen == SETTINGS) {
			DrawText("SETTINGS", 560, 400, 30, DARKPURPLE);
			DrawText("BACK", 560, 440, 30, DARKPURPLE);
		}

		EndDrawing();
	}
	UnloadTexture(bg);
	CloseWindow();



};