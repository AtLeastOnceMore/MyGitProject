#include <iostream>
#include <ncurses.h>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>    // Для time()
#include <locale.h> // Для setlocale()
#include <vector>   // Для хранения направлений

const int ROWS = 20;       // Размер лабиринта
const int MAZE_COLS = 30;  // Размер лабиринта

int maze[ROWS][MAZE_COLS];
int playerRow, playerCol;
int exitRow, exitCol;

// Направления для генерации лабиринта
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

// Проверка, находится ли клетка в пределах лабиринта
bool isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < MAZE_COLS;
}

// Рекурсивный алгоритм для генерации лабиринта
void generateMaze(int x, int y) {
    maze[x][y] = 0; // Открываем текущую клетку

    // Создаем случайный порядок направлений
    std::vector<int> directions = {0, 1, 2, 3};
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;
        std::swap(directions[i], directions[j]);
    }

    // Проходим по всем направлениям
    for (int dir : directions) {
        int nx = x + dx[dir] * 2;
        int ny = y + dy[dir] * 2;

        if (isValid(nx, ny) && maze[nx][ny] == 1) {
            maze[x + dx[dir]][y + dy[dir]] = 0; // Открываем стену между клетками
            generateMaze(nx, ny); // Рекурсивно продолжаем
        }
    }
}

// Инициализация лабиринта
void initMaze() {
    // Заполняем лабиринт стенами
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < MAZE_COLS; ++j) {
            maze[i][j] = 1;
        }
    }

    // Генерация лабиринта
    generateMaze(1, 1);

    // Убедимся, что игрок и выход находятся на свободных клетках
    playerRow = 1;
    playerCol = 1;
    maze[playerRow][playerCol] = 0;

    exitRow = ROWS - 2;
    exitCol = MAZE_COLS - 2;
    maze[exitRow][exitCol] = 0;

    // Открываем выход, если он закрыт
    if (maze[exitRow][exitCol] == 1) {
        maze[exitRow][exitCol] = 0;
    }

    // Помечаем выход
    maze[exitRow][exitCol] = 3;
}

// Отрисовка лабиринта
void drawMaze() {
    clear();
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < MAZE_COLS; ++j) {
            if (i == playerRow && j == playerCol) {
                printw("@"); // Игрок
            } else if (i == exitRow && j == exitCol) {
                printw("X"); // Выход
            } else if (maze[i][j] == 1) {
                printw("#"); // Стена
            } else {
                printw(" "); // Путь
            }
        }
        printw("\n");
    }
    refresh();
}

// Обработка ввода игрока
void handleInput(int input) {
    int newRow = playerRow;
    int newCol = playerCol;

    switch (input) {
        case 'w': newRow--; break; // Вверх
        case 's': newRow++; break; // Вниз
        case 'a': newCol--; break; // Влево
        case 'd': newCol++; break; // Вправо
        default: break;
    }

    // Проверка, можно ли переместиться в новую позицию
    if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < MAZE_COLS && maze[newRow][newCol] != 1) {
        playerRow = newRow;
        playerCol = newCol;
    }
}

int main() {
    setlocale(LC_ALL, ""); // Установка локали для корректного отображения текста
    initscr();                        // Инициализация ncurses
    cbreak();                         // Отключение буферизации ввода
    noecho();                         // Отключение отображения ввода
    keypad(stdscr, TRUE);             // Включение поддержки специальных клавиш

    srand(time(0)); // Инициализация генератора случайных чисел

    initMaze(); // Инициализация лабиринта

    bool gameOver = false;

    while (!gameOver) {
        drawMaze();

        // Проверка, достиг ли игрок выхода
        if (playerRow == exitRow && playerCol == exitCol) {
            printw("Поздравляем! Вы нашли выход!\n");
            refresh();
            gameOver = true;
            break;
        }

        printw("Use W, A, S, D to move. Exit is X. You are @.\n");
        int input = getch(); // Получаем ввод от игрока

        handleInput(input);
    }

    endwin(); // Завершение работы ncurses
    return 0;
}
