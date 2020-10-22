#pragma once
using namespace std;

const int START_SNAKE_SIZE = 5;

struct coord {
	int x;
	int y;
}; //просто структура вектора

bool operator==(coord& value1, coord& value2) {
	return (bool)(value1.x == value2.x) && (value1.y == value2.y);
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

void CreatRect(HDC dc, coord xy, int SizeOfPart, int width, int height, int padding, HBITMAP hbm) {
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
		vector<coord> snake_mas;

		Snake(int width_, int height_) {
			x = width_ / 2, y = height_ / 2, width = width_, height = height_;
			coord start;
			start.x = x;
			start.y = y;

			for (int i = 0; i < START_SNAKE_SIZE; i++) {
				snake_mas.push_back({ start.x, start.y - i });
				snake_lenght += 1;
			}
		}

		void SneakEatApple() {
			snake_lenght += 1;
			snake_mas.push_back(coord{});
		}

		void SnakeMakeMov(coord direction_) {
			direction = direction_;
			coord main_buf, second_buf;
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

		~Snake() {}
		
	private:
		coord direction = { 0 , 1 };
		int x, y, width, height;
		int snake_lenght = 0;
}; //класс змейки


class Apple {
public:
	coord coordAple;
	Apple(int x, int y) {
		coordAple.x = x;
		coordAple.y = y;
	};
	Apple() {};
	~Apple() {};
};
