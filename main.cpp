#include <iostream>
#include <tuple>
#include <random>

constexpr auto PLAYER = static_cast<unsigned char>(120);
constexpr auto WALL = static_cast<unsigned char>(219);
constexpr auto EXIT = 'O';
constexpr auto FREE = ' ';
enum DIR { NORTH, EAST, SOUTH, WEST };

constexpr uint16_t SIZE_R = 20;
constexpr uint16_t SIZE_C = 60;

using std::cout;
using std::endl;
using std::cin;

int withinBounds(int x, int y)
{
	if (x < 0 || x >= SIZE_R-1) return false;
	if (y < 0 || y >= SIZE_C-1) return false;
	return true;
}

std::tuple<uint16_t, uint16_t> placeExit(unsigned char maze[][SIZE_C])
{
	while (true)
	{
		uint16_t x = rand() % (SIZE_R-1);
		uint16_t y = rand() % (SIZE_C-1);
		cout << x << " " << y << endl;
		if (maze[x][y] == FREE)
		{
			maze[x][y] = EXIT; 
			return std::make_tuple(x, y);
		}
	}
}

void generate(unsigned char maze[][SIZE_C], uint16_t x, uint16_t y)
{
	maze[x][y] = FREE;
	int dirs[4] = {NORTH, EAST, SOUTH, WEST};
	for (int i = 0; i < 4; ++i)
	{
		int r = rand() & 3;
		int temp = dirs[r];
		dirs[r] = dirs[i];
		dirs[i] = temp;
	}
	for (int i = 0; i < 4; ++i)
	{
		int dx = 0, dy = 0;
		switch (dirs[i])
		{
			case NORTH: dy = -1; break;
			case SOUTH: dy = 1; break;
			case EAST: dx = 1; break;
			case WEST: dx = -1; break;
		}
		int x2 = x + (dx << 1);
		int y2 = y + (dy << 1);
		if (withinBounds(x2, y2))
		{
			if (maze[x2][y2] == WALL)
			{
				maze[x2 - dx][y2 - dy] = FREE;
				generate(maze, x2, y2);
			}
		}
	}
}

void printMaze(unsigned char arr[][SIZE_C])
{
	for (uint16_t r = 0; r < SIZE_R; r++)
	{
		for (uint16_t c = 0; c < SIZE_C; c++)
			cout << arr[r][c];
		cout << "\n";
	}
}

bool isValidPosition(unsigned char arr[][SIZE_C], std::tuple<uint16_t, uint16_t> pos)
{
	if (arr[std::get<0>(pos)][std::get<1>(pos)] == FREE || arr[std::get<0>(pos)][std::get<1>(pos)] == EXIT) return true;
	else return false;
}

bool finished(std::tuple<uint16_t, uint16_t> playerPos, std::tuple<uint16_t, uint16_t> exitPos)
{
	if (std::get<0>(playerPos) == std::get<0>(exitPos)&& std::get<1>(playerPos) == std::get<1>(exitPos))
	{
		cout << "You win!" << endl;
		return true;
	}
	else return false;
}

void movePlayer(unsigned char arr[][SIZE_C], std::tuple<uint16_t, uint16_t> &playerPos)
{
	char input = ' ';
	uint16_t r, c;
	cout << "\nInput: "; cin >> input;
	switch (input)
	{
		case 'w':
			r = std::get<0>(playerPos) - 1;
			c = std::get<1>(playerPos);
			break;
		case 'a':
			r = std::get<0>(playerPos);
			c = std::get<1>(playerPos) - 1;
			break;
		case 's':
			r = std::get<0>(playerPos) + 1;
			c = std::get<1>(playerPos);
			break;
		case 'd':
			r = std::get<0>(playerPos);
			c = std::get<1>(playerPos) + 1;
			break;
	}
	std::tuple<uint16_t, uint16_t> newPos = std::make_tuple(r, c);
	if (isValidPosition(arr, newPos))
	{
		arr[std::get<0>(playerPos)][std::get<1>(playerPos)] = FREE;
		arr[std::get<0>(newPos)][std::get<1>(newPos)] = PLAYER;
		playerPos = std::make_tuple(r, c);
	}
}

void initialize(unsigned char maze[][SIZE_C], std::tuple<uint16_t, uint16_t>& playerPos, std::tuple<uint16_t, uint16_t>& exitPos)
{
	srand(unsigned(time(nullptr)));
	playerPos = std::make_tuple(1, 1);
	std::fill(&maze[0][0], &maze[SIZE_R-1][SIZE_C-1], WALL);
	generate(maze, std::get<0>(playerPos), std::get<1>(playerPos));
	maze[1][1] = PLAYER;
	exitPos = placeExit(maze);
	printMaze(maze);
	cout << "w => move UP | s => move DOWN | a => move LEFT | d => move RIGHT" << endl;
}

int main(void)
{
	std::tuple<uint16_t, uint16_t> playerPos, exitPos;
	unsigned char maze[SIZE_R][SIZE_C];
	initialize(maze, playerPos, exitPos);
	do {
		movePlayer(maze, playerPos);
		printMaze(maze);
	} while (!finished(playerPos,exitPos));
	return 0;
}