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
	//кнопки
	Rectangle playBtnRect = { 860,620,200,50 };
	Rectangle settingsBtnRect = { 810,690,300, 50 };
	Rectangle exitBtnRect = { 375, 420,100, 30 };

	//шрифты и фоны
	Font titleFont = LoadFontEx("assets/Sheeppuff.otf", 100, 0 ,128);
	SetTextureFilter(titleFont.texture, TEXTURE_FILTER_ANISOTROPIC_16X);
	Font textFont = LoadFontEx("assets/Peppermint.otf",100,0,128);
	SetTextureFilter(textFont.texture, TEXTURE_FILTER_ANISOTROPIC_16X);
	Texture2D bg= LoadTexture("assets/bg2.png");
	Texture2D bgset = LoadTexture("assets/bgset.png");


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
			if (CheckCollisionPointRec(mousepoint, exitBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = MENU;
			}
		}



		BeginDrawing();
		ClearBackground(BLACK);
		
		if (currentScreen == MENU) {
			DrawTexturePro(
				bg,
				{ 0, 0, (float)bg.width, (float)bg.height },
				{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				{ 0, 0 },
				0.0f,
				WHITE);


			const char* title = "FIRE MATH";
			Vector2 titleSize = MeasureTextEx(titleFont, title, 100, 1);
			DrawTextEx(titleFont, title, { (1924 - titleSize.x) / 2.0f, 404 }, 100, 1, BLACK);
			DrawTextEx(titleFont, title, { (1920 - titleSize.x) / 2.0f, 400 }, 100, 1, { 230, 150, 50, 255 });

			const char* play = "PLAY";
			Vector2 playSize = MeasureTextEx(textFont, play, 50, 1);
			const char* set = "SETTINGS";
			Vector2 setSize = MeasureTextEx(textFont, set, 50, 1);

			Color playColor = { 200, 220, 255, 255 };
			Color setColor = { 200, 220, 255, 255 };
			if (CheckCollisionPointRec(mousepoint, playBtnRect)) {
				playColor = { 255, 200, 50, 255 };

			}
			else if (CheckCollisionPointRec(mousepoint, settingsBtnRect)) {
				setColor = { 255, 200, 50, 255 };
			}
			DrawTextEx(textFont, play, { (1920 - playSize.x) / 2.0f + 12, 650 }, 50, 1, playColor);
			DrawTextEx(textFont, set, { (1920 - setSize.x) / 2.0f + 12,700 }, 50, 1, setColor);
		}
		else if (currentScreen == EDUCATION) {
			DrawTexturePro(
				bg,
				{ 0, 0, (float)bg.width, (float)bg.height },
				{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				{ 0, 0 },
				0.0f,
				WHITE);
			DrawText("WRITE A MATH FUNCTION TO SHOT", 560, 400, 30, DARKPURPLE);
		}
		else if (currentScreen == SETTINGS) {
			DrawTexturePro(
				bgset,
				{ 0, 0, (float)bgset.width, (float)bgset.height },
				{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				{ 0, 0 },
				0.0f,
				WHITE);
			Color inkColor = { 80,50,30,255};
			Color exitColor = { 80,50,30,255 };
			if (CheckCollisionPointRec(mousepoint, exitBtnRect)) {
				exitColor = { 255, 120, 0, 255 };
			}
			DrawTextEx(textFont, "SETTINGS", { 555, 220 }, 40,1, inkColor);
			DrawTextEx(textFont, "INGREDIENTS:", { 375, 300 }, 30, 1, inkColor);
			DrawTextEx(textFont, "- VOLUME", { 375, 350 }, 30, 1, inkColor);
			DrawTextEx(textFont, "- GRAPHICS", { 375, 400 }, 30, 1, inkColor);
			DrawTextEx(textFont, "- EXIT", { 375, 450 }, 30, 1, exitColor);
			
		}

		EndDrawing();
	}
	UnloadTexture(bg);
	UnloadTexture(bgset);

	CloseWindow();



};