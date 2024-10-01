#include <iostream> 
#include <string> 
#include <windows.h> 
#undef min
#undef max
#include <time.h> 
#include <conio.h> 
#include <SFML/Audio.hpp> 

using namespace std;


struct snakePart
{
	int x, y;
	snakePart* next;
	snakePart* prev;
};

void setcur(int x, int y) // Позволяет производить отрисовку только игрового поля
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void printField(char** field, int fildHeight, int fieldWidth)
{
	for (int i = 0; i < fildHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
		{
			setcur(j, i);
			cout << field[i][j];
		}
		cout << endl;
	}

}

void clearField(char** field, int fildHeight, int fieldWidth)
{
	for (int i = 0; i < fildHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
			field[i][j] = ' ';

	}
}

void borderField(char** field, int fildHeight, int fieldWidth)
{

	for (int i = 0; i < fildHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
		{

			if (i == 0 && j != 0)
				field[i][j] = char(205);

			if (i == fildHeight - 1 && j != 0)
				field[i][j] = char(205);

			if (j == 0 && i != fildHeight - 1 && i != 0)
				field[i][j] = char(186);


			if (j == fieldWidth - 1 && i != fildHeight - 1 && i != 0)
				field[i][j] = char(186);

			if (i == 0 && j == 0)
				field[i][j] = char(201);

			if (i == 0 && j == fieldWidth - 1)
				field[i][j] = char(187);

			if (i == fildHeight - 1 && j == 0)
				field[i][j] = char(200);

			if (j == fieldWidth - 1 && i == fildHeight - 1)
				field[i][j] = char(188);
		}
	}
}

int main()
{

	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	// Подготовка консоли
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	MoveWindow(GetConsoleWindow(), 50, 50, 2000, 2000, true);


	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false; // Скрываем курсор
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	//Ввод звука
	sf::SoundBuffer buffer;
	buffer.loadFromFile("Coin.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);

	//Поле данных ввода
	int fieldWidth, fildHeight;
	char wallCollission;
	char lvlDif;
	int sleepTime;
	string Name;

	//Ввод
	do
	{

		cout << "Введите имя: "; //!!
		cin >> Name;

		cout << "Введите ширину поля: ";
		cin >> fieldWidth;

		cout << "Введите высоту поля: ";
		cin >> fildHeight;

		cout << "Включить коллизию стен (по умолчанию N)? (Y - Yup, N - Noup): ";
		cin >> wallCollission;

		cout << "Введите уровнь сложности? (H - hard, M - medium, E - Easy): ";
		cin >> lvlDif;

		system("cls");
	} while (fieldWidth <= 5 || fildHeight <= 5); // Условия корректности ввода размеров поля


	//Добавление музыки
	sf::Music music;

	music.openFromFile("Space-Raptors.wav");
	music.play();
	music.setVolume(75);

	music.setLoop(true);

	//Регулировка сложности 

	if (lvlDif == 'H')
		sleepTime = 20;

	else if (lvlDif == 'M')
		sleepTime = 75;

	else if (lvlDif == 'E')
		sleepTime = 250;

	else if (lvlDif == '?')
		sleepTime = 0;

	else
		sleepTime = 1;


	// Коцец ввода


	// Поле инициализации

	// Инициализация игрового поля
	char** field = new char* [fildHeight];

	for (int i = 0; i < fildHeight; i++)
		field[i] = new char[fieldWidth];


	clearField(field, fildHeight, fieldWidth); // Очистка поля от мусора

	// Инициализация змейки
	snakePart snake;
	snakePart* snakeHead = &snake;

	// Ставим голову в случайную точку
	snake.x = rand() % fieldWidth;
	snake.y = rand() % fildHeight;
	snake.next = NULL;
	snake.prev = NULL;

	// Змея жива?
	bool alive = true;
	int currentLength = 0;
	int appleX, appleY;

	// Инициализация для непрерывного движения змейки
	int isCodeExecuted = true;
	char PrevMove;


	int snakeXprev, snakeYprev;
	int length = 0;
	snakePart* snakeCurrent = snakeHead;

	//Позиция яблока
	appleX = rand() % (fieldWidth - 1);
	appleY = rand() % (fildHeight - 1);

	// Булевы знаечния для ограничения движения
	bool isWtapped = false;
	bool isAtapped = false;
	bool isStapped = false;
	bool isDtapped = false;

	// Игровой цикл
	while (alive)
	{
		snakeXprev = snake.x;
		snakeYprev = snake.y;


		Sleep(sleepTime);

		if (_kbhit())
		{
			switch (_getch()) {
			case 'w':
				if (!isStapped)
				{
					snake.y -= 1;
					if (snake.y < 0)
						snake.y = fildHeight - 1;

					isWtapped = true;
					isAtapped = false;
					isDtapped = false;
					PrevMove = 'w';
				}
				else
				{
					snake.y = (snake.y + 1) % fildHeight;
					isStapped = true;
					PrevMove = 's';
				}
				break;


			case 's':
				if (!isWtapped)
				{
					snake.y = (snake.y + 1) % fildHeight;
					isStapped = true;
					PrevMove = 's';
					isAtapped = false;
					isDtapped = false;

				}
				else
				{
					snake.y -= 1;
					if (snake.y < 0)
						snake.y = fildHeight - 1;

					PrevMove = 'w';
				}
				break;


			case 'd':
				if (!isAtapped)
				{
					snake.x = (snake.x + 1) % fieldWidth;
					PrevMove = 'd';
					isDtapped = true;
					isWtapped = false;
					isStapped = false;
				}
				else
				{
					snake.x -= 1;
					if (snake.x < 0)
						snake.x = fieldWidth - 1;

					PrevMove = 'a';
				}


				break;
			case 'a':

				if (!isDtapped)
				{
					snake.x -= 1;
					if (snake.x < 0)
						snake.x = fieldWidth - 1;

					PrevMove = 'a';

					isAtapped = true;
					isWtapped = false;
					isStapped = false;
				}
				else
				{
					snake.x = (snake.x + 1) % fieldWidth;
					PrevMove = 'd';
				}
				break;
			default:
				if (isWtapped)
				{
					snake.y -= 1;
					if (snake.y < 0)
						snake.y = fildHeight - 1;

					isWtapped = true;
					isAtapped = false;
					isDtapped = false;
					PrevMove = 'w';
				}
				if (isStapped)
				{
					snake.y = (snake.y + 1) % fildHeight;
					isStapped = true;
					PrevMove = 's';
					isAtapped = false;
					isDtapped = false;
				}
				if (isAtapped)
				{
					snake.x -= 1;
					if (snake.x < 0)
						snake.x = fieldWidth - 1;

					PrevMove = 'a';
				}
				if (isDtapped)
				{
					snake.x = (snake.x + 1) % fieldWidth;
					PrevMove = 'd';
					isDtapped = true;
					isWtapped = false;
					isStapped = false;
				}

				break;
			}
		}
		else if (isCodeExecuted)
		{
			PrevMove = 'd';
			isCodeExecuted = false;
		}
		else if (PrevMove == 'w')
		{
			snake.y -= 1;
			if (snake.y < 0)
				snake.y = fildHeight - 1;

		}
		else if (PrevMove == 's')
		{
			snake.y = (snake.y + 1) % fildHeight;
		}
		else if (PrevMove == 'd')
		{
			snake.x = (snake.x + 1) % fieldWidth;
		}
		else if (PrevMove == 'a')
		{
			snake.x -= 1;
			if (snake.x < 0)
				snake.x = fieldWidth - 1;

		}


		if (field[snake.y][snake.x] == 'o')
			alive = false;


		if ((field[snake.y][snake.x] == char(205) or field[snake.y][snake.x] == char(186)) && wallCollission == 'Y')
			alive = false;


		if (!alive)
			break;


		if (snake.x == appleX && snake.y == appleY)
		{
			do
			{
				appleX = rand() % fieldWidth;
				appleY = rand() % fildHeight;

				if (appleX == 0)
					appleX++;

				if (appleY == 0)
					appleY++;


			} while (field[appleY][appleX] != ' ');

			snakeCurrent = snakeHead;

			while (snakeCurrent->next != NULL)
				snakeCurrent = snakeCurrent->next;

			snakeCurrent->next = new snakePart();
			snakeCurrent->next->x = snakeXprev;
			snakeCurrent->next->y = snakeYprev;
			snakeCurrent->next->prev = snakeCurrent;
			snakeCurrent->next->next = NULL;
			length++;
			sound.play();
		}


		snakeCurrent = snakeHead;

		if (length >= 1)
		{
			while (snakeCurrent->next != NULL)
				snakeCurrent = snakeCurrent->next;


			while (snakeCurrent != snakeHead->next)
			{
				snakeCurrent->x = snakeCurrent->prev->x;
				snakeCurrent->y = snakeCurrent->prev->y;

				snakeCurrent = snakeCurrent->prev;
			}

			snakeCurrent->x = snakeXprev;
			snakeCurrent->y = snakeYprev;
		}


		snakeCurrent = snakeHead;

		currentLength = 0;

		clearField(field, fildHeight, fieldWidth);
		borderField(field, fildHeight, fieldWidth);


		field[appleY][appleX] = '*';



		while (snakeCurrent != nullptr)
		{
			if (currentLength == 0)
				field[snakeCurrent->y][snakeCurrent->x] = 'O';

			else
				field[snakeCurrent->y][snakeCurrent->x] = 'o';

			currentLength++;
			snakeCurrent = snakeCurrent->next;
		}




		SetConsoleOutputCP(866);
		SetConsoleCP(866);

		printField(field, fildHeight, fieldWidth);


		cout << "Name: " << Name << "\n\n\nScore: " << length << endl << endl << endl;

	}

	system("cls");

	cout << "Game Over, your score is: " << length;

	// Область удаления

	//Удаление списка
	if (length >=1)
	{

	snakeCurrent = snakeHead->next;

	while (snakeCurrent->next != nullptr)
	{
		snakeCurrent = snakeCurrent->next;
		delete snakeCurrent->prev;
	}
	delete snakeCurrent;

	}

	// Удаление массива
	for (int i = 0; i < fildHeight; i++)
		delete[] field[i];
	delete[] field;


	cout << "\nPress Enter to exit. ";


	cin.ignore();
	char LastThing = cin.get();


	return 0;
}
