#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <time.h>

using namespace std;

#pragma region ENUMS AND STRUCTS
enum TYPES
{
	Fire,
	Water,
	Grass,
};

enum MODES
{
	Exit,
	Play,
	Settings,
	MainMenu,
};

enum CHARN
{
	upLeftC,                // ╔
	hor,                    // ═
	upT,                    // ╦
	upRightC,               // ╗
	ver,                    // ║
	leftT,                  // ╠
	cross,                  // ╬
	rightT,                 // ╣
	lowLeftC,               // ╚
	lowT,                   // ╩
	lowRightC,              // ╝
};

struct Pokemon
{
	string name;
	TYPES type;
	int hp;
	int damage;
	int level;
	float xp;
};

struct COORDS
{
	int x;
	int y;
};
#pragma endregion

HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

void Menu();
void GameController();
void Movement(char dir);
void Map();
Pokemon Fight(Pokemon pj, Pokemon wild);

char map[100][30];
COORDS playerPos = { 10, 10 };
char playerRot = '^';
char collision[3][5];

MODES Mode = MainMenu;

int main()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	srand(time(nullptr));

	do
	{

		switch (Mode)
		{

		case Exit:
			break;

		case Play:
			GameController();
			break;

		case Settings:
			break;

		case MainMenu:
			Mode = Play;
			break;

		default:
			break;
		}
	} while (Mode != Exit);
}

void GameController()
{
	Pokemon wildSquirtle = { "Squirtle", Water, 100, 3 };
	Pokemon wildBulbasaur = { "Bulbasaur", Grass, 150, 4 };
	Pokemon pjPk = { "Charmander", Fire, 60, 5, 7, 70.0f };
	pjPk.hp = 15 * pjPk.level;

	char move;

	do
	{

		system("cls");
		Map();
		for (int y = 0; y < 30; y++)
		{
			for (int x = 0; x < 100; x++)
			{
				cout << map[x][y];
			}
			cout << endl;
		}

		move = _getch();
		if (move == 'w' || move == 'a' || move == 's' || move == 'd')
		{
			Movement(move);
			int randomNumber = rand() % 5;

			switch (randomNumber)
			{
			case 1:
				pjPk = Fight(pjPk, wildSquirtle);
				break;

			case 2:
				pjPk = Fight(pjPk, wildBulbasaur);
				break;

			default:
				break;
			}
		}
	} while (true);
}

void Movement(char dir)
{
	switch (dir)
	{
	case 'w':
		playerPos.y -= 3;
		playerRot = '^';
		break;

	case 'a':
		playerPos.x -= 5;
		playerRot = '<';
		break;

	case 's':
		playerPos.y += 3;
		playerRot = 'v';
		break;

	case 'd':
		playerPos.x += 5;
		playerRot = '>';
		break;
	}
}

void Map()
{
	char wall[11] = { 201, 205, 203, 187, 186, 204, 206, 185, 200, 202, 188 };           // Cell Walls variables

	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			if (x == playerPos.x && y == playerPos.y)
			{
				map[x][y] = playerRot;
			}
			else if (y == 0)
			{
				map[x][y] = (x == 0 ? wall[upLeftC] : (x == 99 ? wall[upRightC] : wall[hor]));
			}
			else if (y == 29)
			{
				map[x][y] = x == 0 ? wall[lowLeftC] : x == 99 ? wall[lowRightC] : wall[hor];
			}
			else if (x == 0 || x == 99)
			{
				map[x][y] = wall[ver];
			}
			else
			{
				map[x][y] = ' ';
			}
		}
	}
}

Pokemon Fight(Pokemon pj, Pokemon wild)
{
	wild.level = rand() % 5 + 1;
	int pjDamage = pj.damage * pj.level;
	wild.hp = wild.hp + 5 * wild.level;
	wild.damage = wild.damage * wild.level;
	bool turn = true;

	do
	{
		system("cls");
		(turn ? wild.hp : pj.hp) -= (turn ? pjDamage : wild.damage);
		pj.xp += 0.05 * wild.level;
		cout << pj.name << "           " << wild.name << endl
			<< pj.hp << "               " << wild.hp << endl
			<< pj.level << "               " << wild.level << endl;

		turn = !turn;
		system("pause");
	} while (pj.hp > 0 && wild.hp > 0);

	if (pj.xp > pj.level * 10)
	{
		pj.level++;
		pj.hp += 15;
		cout << pj.name << " has grown to level " << pj.level << endl;
		system("pause");
	}
	return pj;
}