#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <random>
#include <thread>
#include <time.h>
#include <vector>
#include "Snake.h"
using namespace std;

const int SizeOfPart = 9;                   //размер одного элемента поля
const int padding = 1;                       //отступ между частями поля
COLORREF mainColor = RGB(50, 50, 50);        //основной цвет поля, серый
COLORREF snakeColor = RGB(rand() % 255, rand() % 255, rand() % 255);    //snakeColor = RGB(226, 124, 62);  //вторичный цвет, оранж / random
COLORREF AppleCollor = RGB(255, 0, 0);		 //цвет яблока
COLORREF textColor = RGB(255, 210, 0);		 //цвет текста

COLORREF OldMainColor = RGB(142, 179, 102);	 //цвет фона в олдскул моде
COLORREF OldSecondColor = RGB(23, 26, 7);   //цвет остального в олдскул моде

Snake snake = Snake(GetSystemMetrics(SM_CXSCREEN) / (SizeOfPart + padding), GetSystemMetrics(SM_CYSCREEN) / (SizeOfPart + padding)); //
coord SnakeDirection = { 0, 0 }; //направление змейки
coord NULLcoord = { 0, 0 }; //нулевые координаты, нужны только для сравнения


HWND hMainWnd;

bool FullScreen = true;
bool OldSkoolMode = false;

int GameProc(int x_, int y_, int width, int height, int padding, int SizeOfPart,
	HDC dc, HDC dcCompatible, HBITMAP hbm) {

	bool game = true;
	bool appleExist = false;

	Apple apple;
	HBRUSH hBrush;
	int score = 0;

	char buf1[32];

	const int size = 4;
	POINT p[size];
	p[0] = {0, 0};
	p[1] = {width, 0};
	p[2] = {width, height};
	p[3] = {0, height};

	srand(time(NULL));

	while (game) {
		if (OldSkoolMode) {
			hBrush = CreateSolidBrush(OldMainColor);
			SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства для отрисовки поля

			for (int y = 0; y < y_; y++) {  //отрисовка поля
				for (int x = 0; x < x_; x++) {
					CreatRect(dcCompatible, x, y, SizeOfPart, width, height, padding, hbm);
				}
			}

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
			CreatRect(dcCompatible, apple.coordAple, SizeOfPart, x_, y_, padding, hbm); //отрисовка яблока
			DeleteObject(hBrush);

			BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
			appleExist = true;
		}
		else {
			//отрисовка яблока если оно уже существует
			if (OldSkoolMode) { hBrush = CreateSolidBrush(OldSecondColor); }
			else { hBrush = CreateSolidBrush(AppleCollor); }	//выбор цвета для отрисовки яблока
			SelectObject(dcCompatible, hBrush);
			CreatRect(dcCompatible, apple.coordAple, SizeOfPart, x_, y_, padding, hbm); //отрисовка яблока
			DeleteObject(hBrush);
		}

		if (OldSkoolMode) { hBrush = CreateSolidBrush(OldSecondColor); } //выбор цвета для отрисовки змейки
		else { hBrush = CreateSolidBrush(snakeColor); }; //выбор цвета для отрисовки змейки
		
		SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства
		snake.SnakeMakeMov(SnakeDirection);
		if (snake.snake_mas[0] == apple.coordAple) {
			apple.~Apple();
			snake.SneakEatApple();
			score++;
			appleExist = false;
		}

		if ((snake.SizeOfSnake() < 5) && !(SnakeDirection == NULLcoord)) { snake.SneakEatApple(); }

		for (int i = 0; i < snake.SizeOfSnake(); i++) {
			CreatRect(dcCompatible, snake.snake_mas[i], SizeOfPart, x_, y_, padding, hbm);
		}  // отрисовка змейки
		DeleteObject(hBrush);

		if (snake.snake_mas[0].x == -1 |
			snake.snake_mas[0].x == x_ |
			snake.snake_mas[0].y == -1 |
			snake.snake_mas[0].y == y_ ) { game = false; } //game over если зейка ударилась об край

		for (int i = 1; i < snake.SizeOfSnake(); i++) {
			if (snake.snake_mas[0] == snake.snake_mas[i]) { game = false; }
		} //game over если зейка ударилась об себя

		if (OldSkoolMode) { SetTextColor(dcCompatible, OldSecondColor); }							 // выбор цвета текста
		else { SetTextColor(dcCompatible, textColor); }												 //
																									 //
		sprintf(buf1, "SCORE : %i\x00", score);														 // отрисовка очков в правом верхнем углу
		DrawText(dcCompatible, buf1, -1, &tagRECT({ width - 100, 0, width, height }), DT_CENTER);    //	
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY); //обновление экрана окна (копирование из буферного контекста окна в main контекст)
		Sleep(50);
	}

	HFONT bigTextFont = CreateFont(35, 15, 0, 0, 4, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
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
		DrawText(dcCompatible, "GAME OVER", -1, &tagRECT({ rand() % (width * 2) - width, rand() % height, width, height }), DT_LEFT); //
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);																	//
		Sleep(3000 / (i * 3));																											//
	}																																	//

	MessageBox(NULL, "!GAME OVER!", NULL, MB_OK); //сообщение об окончании игры
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
	srand(time(NULL));         //обновление генератора случайных чисел
	int x_, y_, width, height; //переменые для ширины/высоты игрового поля, и для ширины/высоты экрана 
	
	HDC dc, dcCompatible;  //переменные для контекста устройства и совместимого буферного контекста устройства
	HBITMAP hbm;           //битмап для буферного контекста устройства
	
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

	thread GameThread(GameProc, x_, y_, width, height, padding, SizeOfPart, dc, dcCompatible, hbm);
	//запуск процесса игры

	while (GetMessage(&msg, NULL, 0, 0)) { //получаем сообщение
		TranslateMessage(&msg); //преобразуем сообщения
		DispatchMessage(&msg); //передаём сообщение соответствующей функции окна
	} //цикл обработки сообщений

	GameThread.detach(); //не ожидаем завершения процесса игры, если он не завершен
	snake.~Snake();				              //  УБОРКА МУСОРА  //
	DeleteObject(hbm);						  //  УБОРКА МУСОРА  //
	DeleteObject(dc);						  //  УБОРКА МУСОРА  //
	DeleteObject(dcCompatible);               //  УБОРКА МУСОРА  //

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { //функця обработки сообщений
	switch (uMsg) {
	case WM_KEYDOWN: //обработка сообщений о нажатии клавиш
		if ((wParam == VK_LEFT) or (wParam == 0x41))       //стрелка влево или a
			{ if (SnakeDirection.x != 1) { SnakeDirection = { -1, 0 }; }}
		if ((wParam == VK_RIGHT) or (wParam == 0x44))      //стрелка вправо или d
			{ if (SnakeDirection.x != -1) { SnakeDirection = { 1, 0 }; }}
		if ((wParam == VK_UP) or (wParam == 0x57))			//стрелка вверх или w
			{ if (SnakeDirection.y != -1) { SnakeDirection = { 0, 1 }; }}
		if ((wParam == VK_DOWN) or (wParam == 0x53))		//стрелка вниз или s
			{ if (SnakeDirection.y != 1) { SnakeDirection = { 0, -1 }; }}

		switch (wParam) {
			case 0x4F :			//если О английская нажата то включается олдскул мод
				OldSkoolMode = !OldSkoolMode;
				break;
			case VK_SPACE: {    //пробел
				if (!FullScreen)//Из оконного во весь экран
				{
					SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);//Устанавливаем новые стили
					ShowWindow(hWnd, SW_SHOWMAXIMIZED);//Окно во весь экран
					FullScreen = true;
				}
				else //Из всего эранна в оконное
				{
					SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
					ShowWindow(hWnd, SW_SHOWNORMAL);//Показываем обычное окно
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
