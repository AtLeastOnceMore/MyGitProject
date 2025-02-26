#include <iostream>
#include <ncurses.h>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>    // Для time()
#include <locale.h> // Для setlocale()
#include <queue>    // Для BFS

const int ROWS = 10;
const int MAZE_COLS = 15;

int maze[ROWS][MAZE_COLS];
int playerRow, playerCol;
int exitRow, exitCol;

// Проверка, есть ли путь от игрока до выхода (BFS)
bool isPathAvailable() {
    bool visited[ROWS][MAZE_COLS] = {false};
    std::queue<std::pair<int, int>> q;

    q.push({playerRow, playerCol});
    visited[playerRow][playerCol] = true;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int x = current.first;
        int y = current.second;

        if (x == exitRow && y == exitCol) {
            return true; // Путь найден
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < ROWS && ny >= 0 && ny < MAZE_COLS && !visited[nx][ny] && maze[nx][ny] != 1) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    return false; // Путь не найден
}

// Инициализация лабиринта
void initMaze() {
    srand(time(0)); // Инициализация генератора случайных чисел

    do {
        // Заполняем лабиринт стенами (1) и путями (0)
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < MAZE_COLS; ++j) {
                if (i == 0 || i == ROWS - 1 || j == 0 || j == MAZE_COLS - 1) {
                    maze[i][j] = 1; // Границы лабиринта
                } else {
                    maze[i][j] = rand() % 2; // Случайные стены и пути
                }
            }
        }

        // Убедимся, что игрок и выход находятся на свободных клетках
        playerRow = 1;
        playerCol = 1;
        maze[playerRow][playerCol] = 0; // Игрок

        exitRow = ROWS - 2;
        exitCol = MAZE_COLS - 2;
        maze[exitRow][exitCol] = 0; // Выход
    } while (!isPathAvailable()); // Повторяем, пока путь не будет доступен

    maze[exitRow][exitCol] = 3; // Помечаем выход
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
    if (maze[newRow][newCol] != 1) {
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
