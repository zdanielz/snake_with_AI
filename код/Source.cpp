#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")

#include <windows.h>
#include <iostream>
#include <thread>
#include "Snake.h"
using namespace std;

int SnakeProc(int x_, int y_, int width, int height, int padding, int SizeOfPart, COLORREF mainColor, COLORREF secondColor, HDC dc, HDC dcCompatible, vector direction, HBITMAP hbm) {
	Snake snake = Snake(x_, y_);
	HBRUSH hBrush;
	bool game = true;

	while (game) { //Получаем сообщение из очереди
		hBrush = CreateSolidBrush(mainColor); //выбор цвета кисти (серый)
		SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства
		for (int y = 0; y < y_; y++) {
			for (int x = 0; x < x_; x++) {
				CreatRect(dcCompatible, x, y, SizeOfPart, width, height, padding, hbm);
			}
		} // создание поля квадратов
		DeleteObject(hBrush);
		hBrush = CreateSolidBrush(secondColor); //выбор цвета кисти (оранж)
		SelectObject(dcCompatible, hBrush); //выбор кисти для буферного контекста устройства
		snake.SnakeMakeMov(direction);
		for (int i = 0; i < snake.SizeOfSnake(); i++) {
			CreatRect(dcCompatible, snake.snake_mas[i], SizeOfPart, x_, y_, padding, hbm);
		}  // отрисовка змейки
		DeleteObject(hBrush);
		BitBlt(dc, 0, 0, width, height, dcCompatible, 0, 0, SRCCOPY);
		Sleep(100);
	}
	return 0;
}

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
	int x_, y_, width, height, SizeOfPart, padding;
	HDC dc, dcCompatible;
	HBITMAP hbm;
	vector direction = { -1, 0 };
	COLORREF mainColor = RGB(50, 50, 50), secondColor = RGB(226, 124, 62); //основной цвет поля, серый, вторичный цвет, оранж

	width = GetSystemMetrics(SM_CXSCREEN);   //ширина экрана
	height = GetSystemMetrics(SM_CYSCREEN);  //высота экрана

	SizeOfPart = 10;  //размер одного элемента поля
	padding = 1;     //отступ между частями поля

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

	while (((width % (SizeOfPart + padding)) != 0) & ((height % (SizeOfPart + padding)) != 0)) {
		if (SizeOfPart == 0) { break; }
		SizeOfPart -= 1;
	}
	if (SizeOfPart == 0) {
		while (((width % (SizeOfPart + padding)) != 0) & ((height % (SizeOfPart + padding)) != 0)) {
			if ((SizeOfPart == (SizeOfPart * 2))) { break; }
			SizeOfPart += 1;
		}
	} //подгон частей поля под ширину экрана

	thread SnakeThread(SnakeProc, x_, y_, width, height, padding, SizeOfPart, mainColor, secondColor, dc, dcCompatible, direction, hbm);
	SnakeThread.detach();

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
		break;
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}
