#include "raylib.h"
#include "muParser.h"
#include <cstdlib>
using namespace std;
int main() {
	InitWindow(1920, 1080, "Tester");
	SetTargetFPS(60);

	char formula[256] = "\0";
	int letterCount = 0;
	bool isTyping = false;

	Vector2 goblinPos = { 1200,400 };
	float goblinRadius = 40.0f;
	bool goblinAlive = true;
	int score = 0;

	Vector2 startPos = { 100,500 };
	Vector2 ballPos = startPos;
	bool isFlying = false;
	float currentX = 0;
	mu::Parser parser;
	double x_val = 0.0;
	double mathY = 0.0;
	parser.DefineVar("x", &x_val);
	while (!WindowShouldClose()) {

		// Если нажали R — перезапускаем гоблина
		if (IsKeyPressed(KEY_R)) {
			goblinAlive = true;
			//  случайное место для гоблина по X и У (от 800 до 1700)
			goblinPos.x = (float)GetRandomValue(800, 1700);
			goblinPos.y = (float)GetRandomValue(200, 800);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			isTyping = true;
			letterCount = 0;
			formula[0] = '\0';
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

				if (letterCount > 0) {
					try {
						parser.SetExpr(formula);
						isFlying = true;
						currentX = 0;
						ballPos = startPos;
					}
					catch (mu::Parser::exception_type& e) {
						TraceLog(LOG_WARNING, "Ошибка в формуле: %s", e.GetMsg().c_str());
						isFlying = false;
					}
				}
			
			}
		}
			if (isFlying) {
				currentX += 5.0f;
				x_val = currentX;

				try {
					mathY = parser.Eval();
				}
				catch (mu::Parser::exception_type& e) {
					
					isFlying = false;
					TraceLog(LOG_WARNING, "Eval Error: %s", e.GetMsg().c_str());
				}
				ballPos.x = startPos.x + currentX;
				ballPos.y = startPos.y - mathY;

				// --- ПРОВЕРКА ПОПАДАНИЯ ---
			    // Если гоблин жив и шарик коснулся его
				if (goblinAlive && CheckCollisionCircles(ballPos, 10.0f, goblinPos, goblinRadius)) {
					goblinAlive = false; // Гоблин убит
					isFlying = false; // Шарик останавливается
					score += 100; // + очки
				}
			
				// Если улетел за экран — останавливаем
				if (ballPos.x > 1920 || ballPos.y < 0) {
					isFlying = false;
					}
			}
				
			
		
		BeginDrawing();
		DrawRectangle(100, 500, 600, 60, LIGHTGRAY);
		DrawRectangleLines(100, 500, 600, 60, DARKGRAY);

		if (isTyping) {
			DrawText(TextFormat("%s_", formula), 110, 510, 30, BLACK);
		}else {
			if (letterCount == 0) {
				DrawText("Click and type formula here...", 110, 510, 20, GRAY);
			}else {
				DrawText(TextFormat("%s_", formula), 110, 510, 30, BLACK);
			}
		
		}
		if (isFlying) {
			DrawCircleV(ballPos, 10, ORANGE);
		}

		// Рисуем гоблина, если он жив
		if (goblinAlive) {
			DrawCircleV(goblinPos, goblinRadius, GREEN);
			DrawText("GOBLIN", goblinPos.x - 40, goblinPos.y - goblinRadius - 30, 20, DARKGREEN);
		}
		else {
			// Если гоблин убит, пишем об этом
			DrawText("GOBLIN DEFEATED!", goblinPos.x - 100, goblinPos.y, 30, RED);
		}

		// Рисуем счет
		DrawText(TextFormat("SCORE: %i", score), 20, 30, 30, BLACK);
		

		ClearBackground(RAYWHITE);
		DrawText("Hello, Sandbox!", 250, 280, 30, DARKGRAY);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
