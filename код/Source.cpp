#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")
#pragma comment(lib, "gdiplus.lib")
#define _CRT_SECURE_NO_WARNINGS

#include <nlohmann/json.hpp>
#include <windows.h>
#include <gdiplus.h>
#include <valarray>
#include <fstream>
#include <random>
#include <thread>
#include <time.h>
#include <vector>
#include "Snake.h"
using namespace std;

int SizeOfPart = 9;                    //размер одного элемента поля

COLORREF mainColor = RGB(55, 55, 55);       //основной цвет поля
COLORREF snakeColor = RGB(226, 124, 62);	 //вторичный цвет, оранж (чтобы random оставить пустым) 
COLORREF AppleCollor = RGB(255, 0, 0);		 //цвет яблока
COLORREF textColor = RGB(255, 210, 0);		 //цвет текста

COLORREF OldMainColor = RGB(87, 166, 74);	 //цвет фона в олдскул моде
COLORREF OldSecondColor = RGB(3, 6, 1);      //цвет остального в олдскул моде

HWND hMainWnd;

bool FullScreen = true;
bool OldSkoolMode = false;
bool MenuOn = false;
bool terminateGameProc = false;
bool SaveRun = false;
bool LoadRun = false;

int PunktMenuNum[4] = { 1, 2, 3 ,4};
int score = 0;
int padding = 1;
valarray<int> PunktMenu (PunktMenuNum, 4);

Apple apple;

Snake snake = Snake(GetSystemMetrics(SM_CXSCREEN) / (SizeOfPart + padding), GetSystemMetrics(SM_CYSCREEN) / (SizeOfPart + padding));
coord SnakeDirection = { 0, 0 }; //направление змейки
coord NULLcoord = { 0, 0 }; //нулевые координаты, нужны только для сравнения

int GameProc(int x_, int y_, int width, int height,
	HDC dc, HDC dcCompatible, HBITMAP hbm) {

	bool appleExist = false;
	bool game = true;

	HBRUSH hBrush;
	HFONT bigTextFont;

	char buf1[32];
	const Gdiplus::REAL scale = 0.7;
	const Gdiplus::REAL scaleLoader = 0.5;

	const int size = 4;
	POINT p[size];
	p[0] = {0, 0};
	p[1] = {width, 0};
	p[2] = {width, height};
	p[3] = {0, height};

	Gdiplus::Graphics graphics(dcCompatible);
	
	Gdiplus::Image SaveButtonImg(L"img/SaveButton.png");
	Gdiplus::Image SaveButtonActiveImg(L"img/SaveButtonActive.png");
	
	Gdiplus::Image LoadButtonImg(L"img/LoadButton.png");
	Gdiplus::Image LoadButtonActiveImg(L"img/LoadButtonActive.png");
	
	Gdiplus::Image SetingButtonImg(L"img/SetingButton.png");
	Gdiplus::Image SetingButtonActiveImg(L"img/SetingButtonActive.png");
	
	Gdiplus::Image ExitButtonImg(L"img/ExitButton.png");
	Gdiplus::Image ExitButtonActiveImg(L"img/ExitButtonActive.png");

	Gdiplus::Image Loader1(L"img/loader/LoadingWheel1.png");
	Gdiplus::Image Loader2(L"img/loader/LoadingWheel2.png");
	Gdiplus::Image Loader3(L"img/loader/LoadingWheel3.png");
	Gdiplus::Image Loader4(L"img/loader/LoadingWheel4.png");
	Gdiplus::Image Loader5(L"img/loader/LoadingWheel5.png");
	Gdiplus::Image Loader6(L"img/loader/LoadingWheel6.png");
	Gdiplus::Image Loader7(L"img/loader/LoadingWheel7.png");
	Gdiplus::Image Loader8(L"img/loader/LoadingWheel8.png");
	Gdiplus::Image Loader9(L"img/loader/LoadingWheel9.png");
	Gdiplus::Image Loader10(L"img/loader/LoadingWheel10.png");

	graphics.ScaleTransform(scale, scale);

	std::srand(time(NULL));

	while (game) {
		if (terminateGameProc) { goto EndGameProc; }
		if (!MenuOn) { snake.SnakeMakeMov(SnakeDirection); }
		
		if (!(SaveRun | LoadRun)) {
			if (OldSkoolMode) {
				hBrush = CreateSolidBrush(OldMainColor);
				SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства для отрисовки поля
				Polygon(dcCompatible, p, size);
			}
			else {
				hBrush = CreateSolidBrush(mainColor);
				SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства для отрисовки поля

				for (int y = 0; y < y_; y++) {  //отрисовка поля
					for (int x = 0; x < x_; x++) {
						CreatRect(dcCompatible, x, y, SizeOfPart, width, height, padding, hbm);
					}
				}
			} //выбор цвета для отрисовки поля

			DeleteObject(hBrush);
			if (!appleExist) {
				//создание яблока если его уже сьели
				apple = Apple(rand() % (x_), rand() % (y_));

				if (OldSkoolMode) { hBrush = CreateSolidBrush(RGB(0, 0, 0)); }
				else { hBrush = CreateSolidBrush(AppleCollor); }	//выбор цвета для отрисовки яблока
				SelectObject(dcCompatible, hBrush);
				CreatRect(dcCompatible, apple.GetAppleCoord(), SizeOfPart, x_, y_, padding, hbm); //отрисовка яблока
				DeleteObject(hBrush);

				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				appleExist = true;
			}
			else {
				//отрисовка яблока если оно уже существует
				if (OldSkoolMode) { hBrush = CreateSolidBrush(OldSecondColor); }
				else { hBrush = CreateSolidBrush(AppleCollor); }	//выбор цвета для отрисовки яблока
				SelectObject(dcCompatible, hBrush);
				CreatRect(dcCompatible, apple.GetAppleCoord(), SizeOfPart, x_, y_, padding, hbm); //отрисовка яблока
				DeleteObject(hBrush);
			}

			if (OldSkoolMode) { hBrush = CreateSolidBrush(OldSecondColor); } //выбор цвета для отрисовки змейки
			else { hBrush = CreateSolidBrush(snakeColor); }; //выбор цвета для отрисовки змейки

			SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства

			if ((snake.GetSnakeMas()[0].x == apple.GetAppleCoord().x) && (snake.GetSnakeMas()[0].y == apple.GetAppleCoord().y)) {
				apple.~Apple();
				snake.SneakEatApple();
				score++;
				appleExist = false;
			}

			if ((snake.SizeOfSnake() < 5) && !(SnakeDirection == NULLcoord)) { snake.SneakEatApple(); }

			for (int i = 0; i < snake.SizeOfSnake(); i++) {
				CreatRect(dcCompatible, snake.GetSnakeMas()[i], SizeOfPart, x_, y_, padding, hbm);
			}  // отрисовка змейки
			DeleteObject(hBrush);

			if (snake.GetSnakeMas()[0].x == -1 |
				snake.GetSnakeMas()[0].x == x_ |
				snake.GetSnakeMas()[0].y == -1 |
				snake.GetSnakeMas()[0].y == y_) {
				game = false;
			} //game over если зейка ударилась об край

			for (int i = 1; i < snake.SizeOfSnake(); i++) {
				if (snake.GetSnakeMas()[0] == snake.GetSnakeMas()[i]) { game = false; }
			} //game over если зейка ударилась об себя

			if (OldSkoolMode) { SetTextColor(dcCompatible, OldSecondColor); }							 // выбор цвета текста
			else { SetTextColor(dcCompatible, textColor); }												 //
																										 //
			sprintf(buf1, "SCORE : %i\x00", score);														 // отрисовка очков в правом верхнем углу
			DrawText(dcCompatible, buf1, -1, &tagRECT({ width - 100, 0, width, height }), DT_CENTER);    //	
		}
		if (MenuOn && !SaveRun && !LoadRun) {
			if (PunktMenu[0] == 1) 
				graphics.DrawImage(&SaveButtonActiveImg, int(width / 2 / scale - SaveButtonActiveImg.GetWidth() / 2 / scale), int(height / 2 - 200));
			else
				graphics.DrawImage(&SaveButtonImg, int(width / 2 / scale - SaveButtonImg.GetWidth() / 2 / scale), int(height / 2 - 200));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (PunktMenu[0] == 2)
				graphics.DrawImage(&LoadButtonActiveImg, int(width / 2 / scale - LoadButtonActiveImg.GetWidth() / 2 / scale), int(height / 2 - 120));
			else
				graphics.DrawImage(&LoadButtonImg, int(width / 2 / scale - LoadButtonImg.GetWidth() / 2 / scale), int(height / 2 - 120));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (PunktMenu[0] == 3)
				graphics.DrawImage(&SetingButtonActiveImg, int(width / 2 / scale - SetingButtonActiveImg.GetWidth() / 2 / scale), int(height / 2 - 40));
			else
				graphics.DrawImage(&SetingButtonImg, int(width / 2 / scale - SetingButtonImg.GetWidth() / 2 / scale), int(height / 2 - 40));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (PunktMenu[0] == 4)
				graphics.DrawImage(&ExitButtonActiveImg, int(width / 2 / scale - ExitButtonActiveImg.GetWidth() / 2 / scale), int(height / 2 + 40));
			else
				graphics.DrawImage(&ExitButtonImg, int(width / 2 / scale - ExitButtonImg.GetWidth() / 2 / scale), int(height / 2 + 40));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		if (SaveRun | LoadRun) {
			graphics.ScaleTransform(1 / scale, 1 / scale);
			graphics.ScaleTransform(scaleLoader, scaleLoader);

			int x_pos = width / 2 / scaleLoader - Loader1.GetWidth() / 3 / scaleLoader;
			int y_pos = height / 2;

			for (int i = 0; i < 2; i++) {
				
				graphics.DrawImage(&Loader1, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader2, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader3, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader4, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader5, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader6, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader7, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader8, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader9, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);

				graphics.DrawImage(&Loader10, x_pos, y_pos);
				BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
				Polygon(dcCompatible, p, size);
				Sleep(9);
			}
			graphics.ScaleTransform(1 / scaleLoader, 1 / scaleLoader);
			graphics.ScaleTransform(scale, scale);
		}
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);		//обновление экрана окна (копирование из буферного контекста окна в main контекст)
		Polygon(dcCompatible, p, size);
		
		Sleep(50);
	}

	hBrush = CreateSolidBrush(mainColor);
	SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства для отрисовки поля

	bigTextFont = CreateFont(35, 15, 0, 0, 4, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						 DEFAULT_PITCH | FF_DONTCARE, ("Arial")); //1 - высота шрифта, 2 - средняя ширина шрифта, 3 - угол наклона шрифта,
																  //4 - угол ориентации базисной линии, 5 - толщина шрифта, 6 - описатель
																  //параметра курсивнго шрифта, 7 - описатель параметра подчеркивания,
																  //8 - описатель параметра зачеркивания, 9 - идентификатор набора символов
																  //10 - точность вывода, 11 - точность отсечения, 12 - качество вывода
																  //13 - шаг между символами шрифта и семейство, 14 - имя шрифта
	SelectObject(dcCompatible, bigTextFont);

	DrawText(dcCompatible, "GAME OVER", -1, &tagRECT({ 0 - (width / 40), height / 2 - 50, width, height }), DT_CENTER);                 //
																																		//
	for (int i = 0; i < 10; i++) {																										// анимация
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);																	// окончания
		Sleep(100);																														// игры
	}																																	//
																																		//
	for (int i = 1; i < 3000; i++) {																									//
		DrawText(dcCompatible, "GAME OVER", -1, &tagRECT({ rand() % (width * 2) - width, rand() % height, width, height }), DT_LEFT);   //
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);																	//
		Sleep(3000 / (i * 3));																											//
	}																																	//

	MessageBox(NULL, "!GAME OVER!", NULL, MB_OK); //сообщение об окончании игры
	DeleteObject(hBrush);
	EndGameProc:
	PostMessage(hMainWnd, WM_QUIT, NULL, NULL);  //выход из цикла обработки сообщений в главном потоке
	return 0;	//выход из потока игры
}

LRESULT CALLBACK WndProc(
	HWND hwnd,                   //дескриптор окна
	UINT uMsg,					 //сообщение
	WPARAM wParam,				 //дополнительные параметры собщения
	LPARAM lParam				 //дополнительные параметры собщения
);

int WINAPI WinMain(
	HINSTANCE hInstance,		// дескриптор текущего экземпляра окна
	HINSTANCE hPrevInstance,		// дескриптор предыдущего экземпляра окна 
	LPSTR lpCmdLine,			// указатель на командную строку
	int nCmdShow 			// показывает состояние окна 
) {
	std::srand(time(NULL));         //обновление генератора случайных чисел
	int x_, y_, width, height; //переменые для ширины/высоты игрового поля, и для ширины/высоты экрана 
	
	HDC dc, dcCompatible;  //переменные для контекста устройства и совместимого буферного контекста устройства
	HBITMAP hbm;           //битмап для буферного контекста устройства
	
	ULONG_PTR tokenGDIplus;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	if(snakeColor != RGB(226, 124, 62))
		snakeColor = RGB(rand() % 255, rand() % 255, rand() % 255);    //вторичный цвет random

	width = GetSystemMetrics(SM_CXSCREEN);   //ширина экрана
	height = GetSystemMetrics(SM_CYSCREEN);  //высота экрана

	x_ = width / (SizeOfPart + padding);
	y_ = height / (SizeOfPart + padding); //размер поля в... каких то еденицах

	TCHAR szClassName[] = "Мой класс"; // строка с именем класса
	MSG msg; // создём экземпляр структуры MSG для обработки сообщений
	WNDCLASSEX wc; //содержит характеристики окна
	wc.cbSize = sizeof(wc); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окна
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню
	wc.lpszClassName = szClassName; // указатель на имя класса
	wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
	wc.hInstance = hInstance; // указатель на строку, содержащую имя меню, применяемого для класса
	if (!RegisterClassEx(&wc)) {
		// в случае отсутствия регистрации класса:
		MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
		return NULL; // возвращаем, следовательно, выходим из WinMain
	}
	// Функция, создающая окно:
	hMainWnd = CreateWindow(
		szClassName, // имя класса
		"Змейка", // имя окна
		WS_POPUP, // режимы отображения окна
		CW_USEDEFAULT, // позиция окна по оси х
		NULL, // позиция окна по оси у (раз дефолт в х, то писать не нужно)
		CW_USEDEFAULT, // ширина окна
		NULL, // высота окна (раз дефолт в ширине, то писать не нужно)
		(HWND)NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		HINSTANCE(hInstance), // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc

	if (!hMainWnd) {
		// в случае некорректного создания окна (неверные параметры и тп):
		MessageBox(NULL, "Не получилось создать окно!", "Ошибка", MB_OK);
		return NULL;
	}

	ShowWindow(hMainWnd, SW_SHOWMAXIMIZED); // отображаем окно
	UpdateWindow(hMainWnd); // обновляем окно

	dc = GetDC(hMainWnd); //получение контекста устройства монитора

	dcCompatible = CreateCompatibleDC(dc); //создание буферного контекста устройства
	hbm = CreateCompatibleBitmap(dc, width, height); //создание битмапа

	SelectObject(dcCompatible, hbm); //выбор битмапа для буферного контекста устройства
	SetBkMode(dcCompatible, TRANSPARENT); //делает beckground прозрачным
	gdiplusStartupInput.GdiplusVersion = 1;
	Gdiplus::GdiplusStartup(&tokenGDIplus, &gdiplusStartupInput, NULL);

	thread GameThread(GameProc, x_, y_, width, height, dc, dcCompatible, hbm);
	//запуск процесса игры

	while (GetMessage(&msg, NULL, 0, 0)) { //получаем сообщение
		TranslateMessage(&msg); //преобразуем сообщения
		DispatchMessage(&msg); //передаём сообщение соответствующей функции окна
	} //цикл обработки сообщений

	terminateGameProc = true;
	GameThread.join();

	snake.~Snake();				              //  УБОРКА МУСОРА  //
	DeleteObject(hbm);						  //  УБОРКА МУСОРА  //
	DeleteObject(dc);						  //  УБОРКА МУСОРА  //
	DeleteObject(dcCompatible);               //  УБОРКА МУСОРА  //
	Gdiplus::GdiplusShutdown(tokenGDIplus);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { //функця обработки сообщений
	switch (uMsg) {
	case WM_KEYDOWN: //обработка сообщений о нажатии клавиш
		if ((wParam == VK_LEFT) or (wParam == 0x41))       //стрелка влево или a
			{ if (SnakeDirection.x != 1 && !MenuOn) { SnakeDirection = { -1, 0 }; Sleep(50); }}
		
		if ((wParam == VK_RIGHT) or (wParam == 0x44))      //стрелка вправо или d
			{ if (SnakeDirection.x != -1 && !MenuOn) { SnakeDirection = { 1, 0 }; Sleep(50); }}
		
		if ((wParam == VK_UP) or (wParam == 0x57)) {	   //стрелка вверх или w
			if (SnakeDirection.y != -1 && !MenuOn) { SnakeDirection = { 0, 1 }; Sleep(50); }
			else if (MenuOn) { PunktMenu = PunktMenu.cshift(-1); }}

		if ((wParam == VK_DOWN) or (wParam == 0x53)) {		//стрелка вниз или s
			if (SnakeDirection.y != 1 && !MenuOn) { SnakeDirection = { 0, -1 }; Sleep(50); }
			else if (MenuOn) { PunktMenu = PunktMenu.cshift(1); }}

		switch (wParam) {
			case VK_RETURN: //если энтер нажат
				switch (PunktMenu[0]) {
					case 1:
						if (MenuOn) {
							SaveRun = true;
							{
								vector<int> x_vect;
								vector<int> y_vect;
								nlohmann::json save{};
								string save_s;

								for (int i = 0; i < snake.SizeOfSnake(); i++) {
									x_vect.push_back(snake.GetSnakeMas()[i].x);
									y_vect.push_back(snake.GetSnakeMas()[i].y);
								}
								save["directionX"] = snake.GetDataDirection().x;
								save["directionY"] = snake.GetDataDirection().y;
								save["SnakeX"] = x_vect;
								save["SnakeY"] = y_vect;
								save["Lenght"] = snake.SizeOfSnake();
								save["Score"] = score;
								save["AppleX"] = apple.GetAppleCoord().x;
								save["AppleY"] = apple.GetAppleCoord().y;
								save_s = save.dump();

								{
									ifstream file;
									file.open("save");
									if (!file)
										_wmkdir(L"save/");
									else {}
									file.close();
									file.clear();
								}

								{
									ifstream file;
									file.open("save/save.json");
									if (!file)
										_wrmdir(L"save / save.json");
									else {}
									file.close();
									file.clear();
								}

								{
									ofstream SaveOut;
									SaveOut.open("save/save.json");
									SaveOut << save_s;
									SaveOut.close();
									SaveOut.clear();
								}
							}
							Sleep(100);
							SaveRun = false;
						}
						break;
					case 2:
						if (MenuOn) {
							LoadRun = true;
							{
								vector<int> x_vect;
								vector<int> y_vect;
								vector<coord> snake_mas;
								nlohmann::json load{};
								string save_s;
								coord AppleCoord;

								{
									ifstream file;
									file.open("save/save.json");
									if (!file)
										break;
									file.close();
									file.clear();
								}

								{
									ifstream LoadIn;
									LoadIn.open("save/save.json");
									getline(LoadIn, save_s);
									LoadIn.close();
									LoadIn.clear();
								}
								load = nlohmann::json::parse(save_s);

								snake.SetDataDirection(coord{ load["directionX"].get<int>() , load["directionY"].get<int>() });
								snake.SetDataSnakeLenght(load["Lenght"].get<int>());
								score = load["Score"].get<int>();
								SnakeDirection = coord{ load["directionX"].get<int>() , load["directionY"].get<int>() };

								for (int i = 0; i < load["Lenght"].get<int>(); i++) {
									x_vect.push_back(load["SnakeX"][i].get<int>());
									y_vect.push_back(load["SnakeY"][i].get<int>());
								}

								for (int i = 0; i < load["Lenght"].get<int>(); i++) {
									snake_mas.push_back(coord{ x_vect[i], y_vect[i] });
								}

								snake.SetDataSnakeMas(snake_mas);

								AppleCoord = coord{ load["AppleX"].get<int>(), load["AppleY"].get<int>() };

								apple.SetCoordApple(AppleCoord);
							}

							Sleep(100);
							LoadRun = false;
						}
						break;
					case 3:
						PostQuitMessage(NULL);
						break;
					case 4:
						PostQuitMessage(NULL);
						
						break;
					default:
						break;
				}
				break;
			case VK_ESCAPE: //если нажата клавиша esc
				MenuOn = !MenuOn;
				break;
			case 0x4F :			//если О английская нажата то включается олдскул мод
				OldSkoolMode = !OldSkoolMode;
				break;
			case VK_SPACE: {     //если пробел нажат 
				if (!FullScreen) //Из оконного во весь экран
				{
					SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);  //Устанавливаем новые стили
					ShowWindow(hWnd, SW_SHOWMAXIMIZED);        //Окно во весь экран
					UpdateWindow(hMainWnd); // обновляем окно
					FullScreen = true;
				}
				else //Из всего экранна в оконное
				{
					SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
					ShowWindow(hWnd, SW_SHOWNORMAL);//Показываем обычное окно
					UpdateWindow(hMainWnd); // обновляем окно
					FullScreen = false;
				}
			}
			default:
				break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(NULL);     //выход из цикла обработки сообщений, если получено соответствующее сообщение
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}
