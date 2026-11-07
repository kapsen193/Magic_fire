#include "raylib.h"
using namespace std;
int main() {
	InitWindow(1920, 1080, "Tester");
	SetTargetFPS(60);

	char formula[256] = "\0";
	int letterCount = 0;
	bool isTyping = false;

	while (!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			isTyping = true;
		}
		if (isTyping) {
			int key = GetCharPressed();
			while (key > 0) {
				if ((key >= 32) && (key <= 125) && (letterCount < 63)) {
					formula[letterCount] = (char)key;
					formula[letterCount + 1] = '\0';
					letterCount++;
				}
				key = GetCharPressed();
			}
			if (IsKeyPressed(KEY_BACKSPACE)) {
				letterCount--;
				if (letterCount < 0) letterCount = 0;
				formula[letterCount] = '\0';
			}
			
			if (IsKeyPressed(KEY_ENTER)) {
				isTyping = false;
			}
		}
		BeginDrawing();
		DrawRectangle(100, 500, 600, 60, LIGHTGRAY);
		DrawRectangleLines(100, 500, 600, 60, DARKGRAY);

		if (isTyping) {
			DrawText(TextFormat("%s_", formula), 110, 510, 30, BLACK);
		}
		else {
			DrawText(TextFormat("%s_", formula), 110, 510, 30, BLACK);
			if (letterCount == 0) {
				DrawText("Click and type formula here...", 110, 510, 20, GRAY);
			}
		
		}


		ClearBackground(RAYWHITE);
		DrawText("Hello, Sandbox!", 250, 280, 30, DARKGRAY);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
