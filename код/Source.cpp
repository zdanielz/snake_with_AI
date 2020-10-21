#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")

#include <windows.h>
#include <iostream>
#include <random>
#include <thread>
#include <time.h>
#include "Snake.h"
using namespace std;

const int SizeOfPart = 10;  //размер одного элемента поля
const int padding = 1;     //отступ между частями поля

Snake snake = Snake(GetSystemMetrics(SM_CXSCREEN) / (SizeOfPart + padding), GetSystemMetrics(SM_CYSCREEN) / (SizeOfPart + padding));
coord SnakeDirection = { -1, 0 }; //направление змейки

int GameProc(int x_, int y_, int width, int height, int padding, int SizeOfPart, COLORREF mainColor, COLORREF AppleCollor, COLORREF snakeColor, HDC dc, HDC dcCompatible, HBITMAP hbm) {
	HBRUSH hBrush;
	bool game = true;
	bool appleExist = false;
	Apple apple;

	while (game) {
		hBrush = CreateSolidBrush(mainColor);
		SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства
		for (int y = 0; y < y_; y++) {
			for (int x = 0; x < x_; x++) {
				CreatRect(dcCompatible, x, y, SizeOfPart, width, height, padding, hbm);
			}
		} // создание поля квадратов
		DeleteObject(hBrush);
		if (!appleExist) {
			apple = Apple(rand() % x_, rand() % y_);
			hBrush = CreateSolidBrush(AppleCollor);
			SelectObject(dcCompatible, hBrush);
			CreatRect(dcCompatible, apple.coordAple, SizeOfPart, x_, y_, padding, hbm); //отрисовка яблока
			DeleteObject(hBrush);
			appleExist = true;
		}
		else {
			hBrush = CreateSolidBrush(AppleCollor);
			SelectObject(dcCompatible, hBrush);
			CreatRect(dcCompatible, apple.coordAple, SizeOfPart, x_, y_, padding, hbm); //отрисовка яблока
			DeleteObject(hBrush);
		}
		hBrush = CreateSolidBrush(snakeColor);
		SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства
		snake.SnakeMakeMov(SnakeDirection);
		if (snake.snake_mas[0] == apple.coordAple) {
			apple.~Apple();
			snake.SneakEatApple();
			appleExist = false;
		}
		for (int i = 0; i < snake.SizeOfSnake(); i++) {
			CreatRect(dcCompatible, snake.snake_mas[i], SizeOfPart, x_, y_, padding, hbm);
		}  // отрисовка змейки
		DeleteObject(hBrush);
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
		Sleep(50);
	}
	snake.~Snake();
	return 0;
}

bool FullScreen = false;

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	HINSTANCE hInstance,		// дескриптор текущего экземпляра окна
	HINSTANCE hPrevInstance,		// дескриптор предыдущего экземпляра окна 
	LPSTR lpCmdLine,			// указатель на командную строку
	int nCmdShow 			// показывает состояние окна 
)
{
	srand(time(NULL));
	int x_, y_, width, height;
	HDC dc, dcCompatible;
	HBITMAP hbm;
	COLORREF mainColor = RGB(50, 50, 50), snakeColor = RGB(rand() % 255, rand() % 255, rand() % 255);//secondColor = RGB(226, 124, 62); //основной цвет поля, серый. вторичный цвет, оранж / random
	COLORREF appleCollor = RGB(255, 0, 0); //цвет яблока
	width = GetSystemMetrics(SM_CXSCREEN);   //ширина экрана
	height = GetSystemMetrics(SM_CYSCREEN);  //высота экрана

	TCHAR szClassName[] = "Мой класс"; // строка с именем класса
	HWND hMainWnd; // создаём дескриптор будущего окошка
	MSG msg; // создём экземпляр структуры MSG для обработки сообщений
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	wc.cbSize = sizeof(wc); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
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
	// Функция, создающая окошко:
	hMainWnd = CreateWindow(
		szClassName, // имя класса
		"Змейка", // имя окошка (то что сверху)
		WS_OVERLAPPEDWINDOW | WS_POPUP, // режимы отображения окошка
		CW_USEDEFAULT, // позиция окошка по оси х
		NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
		CW_USEDEFAULT, // ширина окошка
		NULL, // высота окошка (раз дефолт в ширине, то писать не нужно)
		(HWND)NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		HINSTANCE(hInstance), // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc

	if (!hMainWnd) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, "Не получилось создать окно!", "Ошибка", MB_OK);
		return NULL;
	}
	ShowWindow(hMainWnd, SW_MAXIMIZE); // отображаем окошко
	UpdateWindow(hMainWnd); // обновляем окошко

	dc = GetDC(hMainWnd); //получение контекста устройства монитора

	dcCompatible = CreateCompatibleDC(dc); //создание буферного контекста устройства
	hbm = CreateCompatibleBitmap(dc, width, height); //создание битмапа

	SelectObject(dcCompatible, hbm); //выбор битмапа для буферного контекста устройства

	int c = 0; //проссто итератор
	x_ = width / (SizeOfPart + padding);
	y_ = height / (SizeOfPart + padding); //размер поля в... каких то еденицах

	thread GameThread(GameProc, x_, y_, width, height, padding, SizeOfPart, mainColor, appleCollor, snakeColor, dc, dcCompatible, hbm);
	GameThread.detach();

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg); //Преобразуем сообщения
		DispatchMessage(&msg); //Передаём сообщение соответствующей функции окна
	}

	DeleteObject(dcCompatible);               //  УБОРКА МУСОРА  //

	return 0; // возвращаем код выхода из приложения
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		switch (wParam) {
			case VK_LEFT:
				if (SnakeDirection.x != 1) { SnakeDirection = { -1, 0 }; }
				break;
			case VK_RIGHT:
				if (SnakeDirection.x != -1) { SnakeDirection = { 1, 0 }; };
				break;
			case VK_UP:
				if (SnakeDirection.y != -1) { SnakeDirection = { 0, 1 }; };
				break;
			case VK_DOWN:
				if (SnakeDirection.y != 1) { SnakeDirection = { 0, -1 }; };
				break;
			case VK_SPACE: {
				if (!FullScreen)//Из оконного во весь экран
				{
					SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);//Устанавливаем новые стили
					ShowWindow(hWnd, SW_SHOWMAXIMIZED);//Окно во весь экран
					FullScreen = true;
				}
				else//Из всего эранна в оконное
				{
					SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
					ShowWindow(hWnd, SW_SHOWDEFAULT);//Показываем обычное окно
					FullScreen = false;
				}
			}
			default:
				break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}
