#pragma once
using namespace std;

struct vector {
	int x;
	int y;
}; //просто структура вектора

vector* addElement(vector *mas, int &size) {
	size = size - 1;
	vector *temp = new vector[size + 1];
	for (int i = 0; i < size + 1; i++) { temp[i] = mas[i]; }
	temp[size + 1] = mas[size];
	delete[] mas;
	size += 1;

	return temp;
}

void CreatRect(HDC dc, int nx, int ny, int SizeOfPart, int width, int height, int padding, HBITMAP hbm) {
	const int size = 4;
	POINT p[size];

	p[0] = { nx + (padding * nx + nx * SizeOfPart), ny + (padding * ny + ny * SizeOfPart) };
	p[1] = { nx + SizeOfPart + (padding * nx + nx * SizeOfPart), ny + (padding * ny + ny * SizeOfPart) };
	p[2] = { nx + SizeOfPart + (padding * nx + nx * SizeOfPart), ny + SizeOfPart + (padding * ny + ny * SizeOfPart) };
	p[3] = { nx + (padding * nx + nx * SizeOfPart), ny + SizeOfPart + (padding * ny + ny * SizeOfPart) };

	Polygon(dc, p, size);
} //функция создающяя квадраты, принимает координаты

void CreatRect(HDC dc, vector xy, int SizeOfPart, int width, int height, int padding, HBITMAP hbm) {
	const int size = 4;
	POINT p[size];

	p[0] = { xy.x + (padding * xy.x + xy.x * SizeOfPart), xy.y + (padding * xy.y + xy.y * SizeOfPart) };
	p[1] = { xy.x + SizeOfPart + (padding * xy.x + xy.x * SizeOfPart), xy.y + (padding * xy.y + xy.y * SizeOfPart) };
	p[2] = { xy.x + SizeOfPart + (padding * xy.x + xy.x * SizeOfPart), xy.y + SizeOfPart + (padding * xy.y + xy.y * SizeOfPart) };
	p[3] = { xy.x + (padding * xy.x + xy.x * SizeOfPart), xy.y + SizeOfPart + (padding * xy.y + xy.y * SizeOfPart) };

	Polygon(dc, p, size);
} //функция создающяя квадраты, принимает векторы

class Snake {

	public:
		HDC BuferDC;
		HBITMAP hbm;
		vector direction = {0 , 1};
		COLORREF MainColor;
		int x, y, width, height, padding, SizeOfPart;
		const int START_SNAKE_SIZE = 5;
		int snake_lenght = START_SNAKE_SIZE;
		vector* snake_mas = new vector[START_SNAKE_SIZE]; //масив точек с их координатами, змейка


		Snake(int width_, int height_) {
			x = width_ / 2, y = height_ / 2, width = width_, height = height_;
			vector start;
			start.x = x;
			start.y = y;

			for (int i = 0; i < START_SNAKE_SIZE; i++) {
				snake_mas[i] = { start.x, start.y - i };
			}
		}

		void SneakEatApple() {
			snake_mas = addElement(snake_mas, snake_lenght);
		}

		void SnakeMakeMov(vector direction_) {
			direction = direction_;
			vector main_buf, second_buf;
			main_buf.x = snake_mas[0].x;
			main_buf.y = snake_mas[0].y;
			snake_mas[0].x += direction.x;
			snake_mas[0].y -= direction.y;
			for (int i = 1; i < snake_lenght; i++) {
				second_buf.x = snake_mas[i].x;
				second_buf.y = snake_mas[i].y;
				snake_mas[i].x = main_buf.x;
				snake_mas[i].y = main_buf.y;
				main_buf.x = second_buf.x;
				main_buf.y = second_buf.y;
			}
		}

		int SizeOfSnake() {
			return snake_lenght;
		}

		~Snake() {
			delete[] snake_mas;
		}
}; //класс змейки
