#include "raylib.h"
#include <iostream>
#include "muParser.h"

using namespace std;
// перечисление игровых экранов
enum GameScreen {MENU,MODE_SELECT,SETTINGS,GAMEPLAY,DIALOGE_SCENE,SHOOTING_GUIDE};
int main()  
{	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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
	Texture2D bgdial = LoadTexture("assets/bgdial.png");
	Texture2D gamebg = LoadTexture("assets/gamebg.png");
	//прогресс обучения
	bool EducationCompleted = false;
	int dialoge_step = 0;
	const char* dialogeTexts[] = {
		"Hello, my young wizard",
		"Use fireball",
		"I have new mission for you",
	};
	//переменные для геймплея
	char formula[256] = "\0";
	int letterCount = 0;
	bool isTyping = false;

	Vector2 goblinPos = { 1200,400 };
	float goblinScale = 0.13f;
	float ballScale = 0.25f;
	bool goblinAlive = true;
	int score = 0;

	Texture2D goblinTex = LoadTexture("assets/gob1.png");
	if (goblinTex.id == 0) {
		TraceLog(LOG_ERROR, "ОШИБКА: Гоблин не загружен!");
	}

	Texture2D fireball = LoadTexture("assets/fireball_spritelist.png");
	int currentFrame = 0;
	float frameTimer = 0.0f;
	const int MAX_frames = 4; //4 кадра
	const float Frame_speed = 0.1f; // скорость анимации 10 кадров в секунду

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
	float currentBallRadius = 0.0f;
	float actualGoblinRadius = 0.0f;
	Vector2 trail[500]; 
	int trailCount = 0;

	int currentlevel = 1;// переменная уровня


	while (!WindowShouldClose()){
		Vector2 mousepoint = GetMousePosition();
		if (currentScreen == MENU) {
			if (CheckCollisionPointRec(mousepoint, playBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				if (EducationCompleted == false) {
					currentScreen = GAMEPLAY;
					goblinAlive = true;
					score = 0;
					
				}
				
					
		}
			if (CheckCollisionPointRec(mousepoint, settingsBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = SETTINGS;
			}
		}
		else if (currentScreen == MODE_SELECT) {
			if (IsKeyPressed(KEY_ESCAPE)) {
				currentScreen = MENU;
			}
		}
		else if (currentScreen == SETTINGS) {
			if (CheckCollisionPointRec(mousepoint, exitBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = MENU;
			}
		}
		else if (currentScreen == GAMEPLAY) {
			if (IsKeyPressed(KEY_R)) {
				goblinAlive = true;
				//  случайное место для гоблина по X и У (от 800 до 1700)
				goblinPos.x = (float)GetRandomValue(800, 1700);
				goblinPos.y = (float)GetRandomValue(200, 800);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(),{100,900,600,60})) {
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

			if (isTyping) {

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
						trailCount = 0; // обнуляем след
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
				//След из точек чтобы посмотреть траекторию шара
				if (trailCount < 500) {
					trail[trailCount] = ballPos;
					trailCount++;
				}
				//Анимация кадров
				frameTimer += GetFrameTime();
				if (frameTimer >= Frame_speed) {
					frameTimer = 0.0f;
					currentFrame++;
					if (currentFrame >= MAX_frames) currentFrame = 0;
				}


				// --- ПРОВЕРКА ПОПАДАНИЯ ---
				// Если гоблин жив и шарик коснулся его
				currentBallRadius = (fireball.height * ballScale) / 2.0f;
				actualGoblinRadius = (goblinTex.height * goblinScale) / 2.0f;
				if (goblinAlive && CheckCollisionCircles(ballPos, currentBallRadius, goblinPos, actualGoblinRadius)) {
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
		else if (currentScreen == DIALOGE_SCENE) {
			DrawTexturePro(
				bgdial,
				{ 0, 0, (float)bgdial.width, (float)bgdial.height },
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
		else if (currentScreen == GAMEPLAY) {
			DrawTexturePro(
				gamebg,
				{ 0, 0, (float)gamebg.width, (float)gamebg.height },
				{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				{ 0, 0 },
				0.0f,
				WHITE);
			DrawRectangle(100, 900, 600, 60, LIGHTGRAY);
			DrawRectangleLines(100, 900, 600, 60, DARKGRAY);
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
				DrawText(TextFormat("%s_", formula), 110, 920, 30, BLACK);
				if (letterCount == 0) {
					DrawText("Type formula and press ENTER (Press Q to clear)", 110, 860, 20, GRAY);
				}
			}
			else {
				if (letterCount == 0) {
					DrawText("Click and type formula here...", 110, 920, 20, GRAY);
				}
				else {
					DrawText(TextFormat("%s_", formula), 110, 920, 30, BLACK);
				}

			}

			for (int i = 0;i < trailCount;i += 3) { //+3 чтоюы не слипались
				float alpha = (float)i / (float)trailCount; // чем дальше от шара точка тем тускнее она становится эффект затухания
				DrawCircleV({ trail[i].x, trail[i].y - 5.0f }, 4.0f, Fade(ORANGE, alpha));
			}

			if (isFlying) {
				float scale = 0.2f;//размер шара
				float frameWidth = (float)fireball.width / MAX_frames; //вырезает нужный кадр из списка спрайтов
				Rectangle sourceRec = { currentFrame * frameWidth,0,frameWidth,(float)fireball.height };
				DrawTexturePro(fireball, sourceRec,
					{ ballPos.x, ballPos.y, frameWidth * ballScale, (float)fireball.height * ballScale },
					{ (frameWidth * ballScale) / 2.0f, ((float)fireball.height * ballScale) / 2.0f }, // Центр
					0.0f,
					WHITE
				);
			}

			// Рисуем гоблина, если он жив
			if (goblinAlive) {
				DrawTexturePro(
					goblinTex,
					{ 0, 0, (float)goblinTex.width, (float)goblinTex.height }, // Берем всю картинку
					{ goblinPos.x, goblinPos.y, (float)goblinTex.width  * goblinScale, (float)goblinTex.height * goblinScale }, // Куда и какого размера рисовать
					{ (goblinTex.width * goblinScale) / 2.0f, (goblinTex.height * goblinScale) / 2.0f }, // Точка вращения (теперь это центр картинки)
					0.0f,
					WHITE
				);

				// надпись над ним
				DrawText("GOBLIN", goblinPos.x - 35, goblinPos.y - 70, 20, DARKGREEN);

			}
			else {
				// Если гоблин убит, пишем об этом
				DrawText("GOBLIN DEFEATED!", goblinPos.x - 100, goblinPos.y, 30, RED);
			}

			// Рисуем счет
			DrawText(TextFormat("SCORE: %i", score), 20, 30, 30, BLACK);

			
		}
		// ВРЕМЕННЫЙ КОД ДЛЯ ОТЛАДКИ ХИТБОКСОВ
		/*if (isFlying) {
			// Рисуем красный контур хитбокса шара
			DrawCircleLines(ballPos.x, ballPos.y, currentBallRadius, RED);
		}
		if (goblinAlive) {
			// Рисуем синий контур хитбокса гоблина
			DrawCircleLines(goblinPos.x, goblinPos.y, actualGoblinRadius, BLUE);
		}*/
		EndDrawing();
	}
	UnloadTexture(bg);
	UnloadTexture(bgset);
	UnloadTexture(bgdial);
	UnloadTexture(gamebg);
	UnloadTexture(goblinTex);
	UnloadTexture(fireball);


	CloseWindow();



};