#pragma once
using namespace std;

const int START_SNAKE_SIZE = 1; //стартовый разер змейки

struct coord {
	int x;
	int y;
}; //просто структура координат

bool operator==(coord& value1, coord& value2) {
	return (bool)(value1.x == value2.x) && (value1.y == value2.y);
}  //перегрузка == для сравнения коорднат

void CreatRect(HDC dc, int nx, int ny, int SizeOfPart, int width, int height, int padding, HBITMAP hbm) {
	const int size = 4;
	POINT p[size];

	p[0] = { nx + (padding + nx * SizeOfPart), ny + (padding + ny * SizeOfPart) };
	p[1] = { nx + SizeOfPart + (padding + nx * SizeOfPart), ny + (padding + ny * SizeOfPart) };
	p[2] = { nx + SizeOfPart + (padding + nx * SizeOfPart), ny + SizeOfPart + (padding + ny * SizeOfPart) };
	p[3] = { nx + (padding + nx * SizeOfPart), ny + SizeOfPart + (padding + ny * SizeOfPart) };

	Polygon(dc, p, size);
} //функция создающяя квадраты, принимает координаты в виде значений х и у

void CreatRect(HDC dc, coord xy, int SizeOfPart, int width, int height, int padding, HBITMAP hbm) {
	const int size = 4;
	POINT p[size];

	p[0] = { xy.x + (padding + xy.x * SizeOfPart), xy.y + (padding + xy.y * SizeOfPart) };
	p[1] = { xy.x + SizeOfPart + (padding + xy.x * SizeOfPart), xy.y + (padding + xy.y * SizeOfPart) };
	p[2] = { xy.x + SizeOfPart + (padding + xy.x * SizeOfPart), xy.y + SizeOfPart + (padding + xy.y * SizeOfPart) };
	p[3] = { xy.x + (padding + xy.x * SizeOfPart), xy.y + SizeOfPart + (padding + xy.y * SizeOfPart) };

	Polygon(dc, p, size);
} //функция создающяя квадраты, принимает координаты в виде структуры coord

class Snake {
	public:
		Snake(int width_, int height_) {
			x = width_ / 2, y = height_ / 2;
			coord start; // стартовое положение головы змейки
			start.x = x;
			start.y = y;

			for (int i = 0; i < START_SNAKE_SIZE; i++) {
				snake_mas.push_back({ start.x, start.y - i });
				snake_lenght += 1;
			}
		} //конструктор змейки

		void SneakEatApple() {
			snake_lenght += 1;
			snake_mas.push_back(coord{});
		} //функция съедания яблока

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
		} //функция движения змейки

		int SizeOfSnake() {
			return snake_lenght;
		} //функция для определения размера змейки

		vector<coord> GetSnakeMas() {
			return snake_mas;
		}

		coord GetDataDirection() { return direction; }
		void SetDataDirection(coord direction_) { direction = direction_; }
		void SetDataSnakeLenght(int lenght) { snake_lenght = lenght; }
		void SetDataSnakeMas(vector<coord> SnakeMas) { snake_mas = SnakeMas; }

		~Snake() {}
		
	private:
		vector<coord> snake_mas; //вектор, содержащий координаты каждого элемента тела змейки
		coord direction = { 0 , 1 }; //направление змейки
		int x, y; //размеры поля
		int snake_lenght = 0;  //длинна змейки
}; //класс змейки

class Apple {
	public:
		Apple(int x, int y) {
			coordApple.x = x;
			coordApple.y = y;
		};
		coord GetAppleCoord() { return coordApple; }
		void SetCoordApple(coord coordApple_) { coordApple = coordApple_; }
		Apple() {};       //конструктор для случаев когда это квантовое яблоко (оно вроде есть, но где?)
		~Apple() {};
	private:
		coord coordApple;  //координаты яблока
}; //класс яблока, мальенький как само яблокло
