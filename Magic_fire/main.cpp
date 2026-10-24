#include "raylib.h"
#include <iostream>
using namespace std;
int main()  
{
	InitWindow(1920, 1080, "Magic Fire Game");
	SetTargetFPS(60);
	while (!WindowShouldClose()){
		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawText("READY TO START", 800, 400, 30, DARKGRAY);
		DrawText("WRITE A MATH FUNCTION FOR A DICE ROLL", 550, 300, 30, DARKGRAY);

		EndDrawing();
	}

	CloseWindow();



};