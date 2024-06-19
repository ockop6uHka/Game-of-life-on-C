#include <ncurses.h> //дополнительная библиотека для отображения картинки
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //библиотека для регулирования скорости игры

#define GRIDHEIGHT 25 //размеры игрового поля
#define GRIDWIDTH 80

void printGrid(int gridOne[GRIDHEIGHT][GRIDWIDTH]); //вывод изображения
void determineState(int gridOne[GRIDHEIGHT][GRIDWIDTH]); //логика поведения клеток
void copyGrid(int source[GRIDHEIGHT][GRIDWIDTH], int dest[GRIDHEIGHT][GRIDWIDTH]); //копирование матриц
void initializeGridFromFile(int gridOne[GRIDHEIGHT][GRIDWIDTH]); //считывание файла
void getInputMode(char *mode);
void startGameLoop(int gridOne[GRIDHEIGHT][GRIDWIDTH], int update_speed); //отображение игрового поля
int stopGame(int gridOne[GRIDHEIGHT][GRIDWIDTH]); //условие выхоада из игры

int main(int argc, char *argv[]) {
    initscr(); //подключение библиотеки
    cbreak();  // Отключение буферизации ввода
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  // Отключение отображения вводимых символов
    curs_set(FALSE);        // Скрытие курсора

    int gridOne[GRIDHEIGHT][GRIDWIDTH] = {0}; //создаем матрицу и заполняем её ноликами

    initializeGridFromFile(gridOne);

    int update_speed = 200000;  // По умолчанию 0.2 секунды
    if (argc > 1) {
        update_speed = atoi(argv[1]) * 100000;
    }

    startGameLoop(gridOne, update_speed);

    endwin(); //закрытие библиотеки
    return 0;
}

void initializeGridFromFile(int gridOne[GRIDHEIGHT][GRIDWIDTH]) {
    int x = -2;
    int y = -2;
    while (x != -1 && y != 1) { //считывание координат и запись их в матрицу до значений -1 -1
        scanf("%d %d", &x, &y);
        gridOne[x][y] = 1;
        printGrid(gridOne);
        refresh(); //обновление экрана
    }
    printGrid(gridOne);
    refresh();
}

void startGameLoop(int gridOne[GRIDHEIGHT][GRIDWIDTH], int update_speed) {
    while (stopGame(gridOne)) { //игра заканчивается если живых клеток не остаётся
        clear();
        printGrid(gridOne); //генерация изображений если у нас игра не заканчивается
        refresh();
        determineState(gridOne);
        usleep(update_speed);  // Используем задержку в зависимости от скорости обновления
    }
}

void printGrid(int gridOne[GRIDHEIGHT][GRIDWIDTH]) {
    for (int a = 0; a < GRIDHEIGHT; a++) {
        for (int b = 0; b < GRIDWIDTH; b++) { //заполнение матрицы значениями клеток для их отображения
            if (gridOne[a][b] == 1) {
                mvprintw(a, b * 2, " O ");
            } else {
                mvprintw(a, b * 2, " . ");
            }
        }
    }
}

void copyGrid(int source[GRIDHEIGHT][GRIDWIDTH], int dest[GRIDHEIGHT][GRIDWIDTH]) {
    for (int a = 0; a < GRIDHEIGHT; a++) {
        for (int b = 0; b < GRIDWIDTH; b++) { //копирование одной матрицы в другую чтобы у нас клетки менялись сразу все
            dest[a][b] = source[a][b];
        }
    }
}

void determineState(int gridOne[GRIDHEIGHT][GRIDWIDTH]) {
    int gridTwo[GRIDHEIGHT][GRIDWIDTH];
    copyGrid(gridOne, gridTwo); //создаем дополнительную матрицу для копирования

    for (int a = 0; a < GRIDHEIGHT; a++) {
        for (int b = 0; b < GRIDWIDTH; b++) { //проходим каждую ячейку нашей первой матрицы
            int alive = 0;
            for (int c = -1; c <= 1; c++) {
                for (int d = -1; d <= 1; d++) { //считываем количество живых ячеек рядом с нашей точкой
                    if (!(c == 0 && d == 0)) {
                        int newA = (a + c + GRIDHEIGHT) % GRIDHEIGHT;
                        int newB = (b + d + GRIDWIDTH) % GRIDWIDTH; //дополнительная обработка на замкнутость
                        if (gridTwo[newA][newB] == 1) {
                            ++alive;
                        }
                    }
                }
            }
            if (alive < 2 || (gridOne[a][b] == 1 && alive > 3) || (gridOne[a][b] == 0 && alive == 2)) {
                gridOne[a][b] = 0; //условия для превращения клетки в мертвую
            } else {
                gridOne[a][b] = 1; 
            }
        }
    }
}

int stopGame(int gridOne[GRIDHEIGHT][GRIDWIDTH]) {
    int count_alive = 0;

    for (int a = 0; a < GRIDHEIGHT; a++) { //считывание количества живых клеток, если их 0, то заканчиваем игру
        for (int b = 0; b < GRIDWIDTH; b++) {
            if (gridOne[a][b] == 1) {
                count_alive++;
            }
        }
    }
    return count_alive;
}
