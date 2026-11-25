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
	Texture2D goblinTex = LoadTexture("gob1.png");
	if (goblinTex.id == 0) {
		TraceLog(LOG_ERROR, "ОШИБКА: Гоблин не загружен!");
	}
	Vector2 startPos = { 100,500 };
	Vector2 ballPos = startPos;
	bool isFlying = false;
	float currentX = 0;
	mu::Parser parser;
	double x_val = 0.0;
	double mathY = 0.0;
	float flashTimer = 0.0f;
	bool justActivated = false;
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

		// Если нажали E и ввод выключен — включаем и ставим флаг
		if (IsKeyPressed(KEY_Q) && !isTyping) {
			isTyping = true;
			letterCount = 0;
			formula[0] = '\0';
			justActivated = true;
		}

		if (isTyping ) {
			
			int key = GetCharPressed();
			while (key > 0) {
				if ((key >= 32) && (key <= 125) && (letterCount < 255)) {
					// Если мы только что активировали ввод клавишей E, пропускаем эту букву
					if (justActivated && (key == 'q' || key == 'Q')) {
						justActivated = false; // Сбрасываем флаг
					}
					else {
						formula[letterCount] = (char)key;
						formula[letterCount + 1] = '\0';
						letterCount++;
					}
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
				justActivated = false;
				
				if (letterCount > 0) {
					try {
						parser.SetExpr(formula);
						isFlying = true;
						flashTimer = 0.3f; // Вспышка длится 0.3 секунды
						currentX = 0;
						ballPos = startPos;
						letterCount = 0;
						formula[0] = '\0';
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
			// Уменьшаем таймер вспышки каждый кадр
				if (flashTimer > 0.0f) {
					flashTimer -= GetFrameTime();
					if (flashTimer < 0.0f) flashTimer = 0.0f; // Защита от минусового времени
				}
		
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangle(100, 500, 600, 60, LIGHTGRAY);
		DrawRectangleLines(100, 500, 600, 60, DARKGRAY);
		// 1. Рисуем книгу (точка старта startPos)
		DrawRectangle(startPos.x - 40, startPos.y - 20, 80, 60, DARKBROWN);
		DrawRectangle(startPos.x - 35, startPos.y - 15, 70, 50, BEIGE);
		DrawLine(startPos.x, startPos.y - 15, startPos.x, startPos.y + 35, LIGHTGRAY);
		DrawCircle(startPos.x, startPos.y - 25, 5, ORANGE);
		DrawCircle(startPos.x - 10, startPos.y - 35, 3, YELLOW);
		DrawCircle(startPos.x + 15, startPos.y - 30, 4, ORANGE); // Точка, откуда вылетает шар

		// Рисуем вспышку, если таймер активен
		if (flashTimer > 0.0f) {
			float alpha = flashTimer / 0.3f;
			DrawCircle(startPos.x, startPos.y, 60, Fade(YELLOW, alpha));
			DrawCircle(startPos.x, startPos.y, 30, Fade(ORANGE, alpha));
		}

		if (isTyping) {
			DrawText(TextFormat("%s_", formula), 110, 510, 30, BLACK);
			if (letterCount == 0) {
				DrawText("Type formula and press ENTER (Press Q to clear)", 110, 460, 20, GRAY);
			}
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
			float scale = 0.15f; // Уменьшение в 3 раза
			DrawTexturePro(
				goblinTex,
				{ 0, 0, (float)goblinTex.width, (float)goblinTex.height }, // Берем всю картинку
				{ goblinPos.x, goblinPos.y, (float)goblinTex.width * scale, (float)goblinTex.height * scale }, // Куда и какого размера рисовать
				{ (goblinTex.width * scale) / 2.0f, (goblinTex.height * scale) / 2.0f }, // Точка вращения (теперь это центр картинки)
				0.0f,
				WHITE
			);

			// надпись над ним
			DrawText("GOBLIN", goblinPos.x- 35, goblinPos.y-70, 20, DARKGREEN);
		
		}
		else {
			// Если гоблин убит, пишем об этом
			DrawText("GOBLIN DEFEATED!", goblinPos.x - 100, goblinPos.y, 30, RED);
		}

		// Рисуем счет
		DrawText(TextFormat("SCORE: %i", score), 20, 30, 30, BLACK);
		

		
		DrawText("Hello, Sandbox!", 250, 280, 30, DARKGRAY);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
