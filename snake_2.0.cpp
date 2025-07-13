#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <curses.h>

using namespace std;
using namespace std::chrono;

bool game_exit = false;

struct vector2d {
    int x = 0; 
    int y = 0;
};

void showMap(const vector2d &apple, const int &eaten_apple) {
    clear();
    move(2, 55);

    attrset(A_DIM | COLOR_PAIR(1)); //устанавливаются атрибуты для текста далее
    printw("Змейка\t\t");
    attrset(A_BOLD | COLOR_PAIR(5));
    printw("Количество собранных яблок\t\t");
    printw(to_string(eaten_apple).c_str());
    printw(" > ");

    //Border (Граница)
    attrset(A_DIM | COLOR_PAIR(1));
    for (auto& value : { 4, 27 }) {
        for (int x = 5; x < 112; x++) {
            move(value, x);
            printw("*");
        }
    }

    for (auto& value : { 5, 112 }) {
        for (int y = 4; y < 28; y++) {
            move(y, value);
            printw("*");
        }
    }

    move(apple.y, apple.x);
    attrset(A_BOLD | COLOR_PAIR(2));
    printw("@");


}

void moveSnake(vector2d& snake_head, vector2d& step, vector<vector2d>& tail, vector2d& apple, vector2d& rand_apple, int& eaten_apples) {
    
    attrset(A_BOLD | COLOR_PAIR(3)); // цвет змейки

    if (!tail.empty()) {
        for (const auto& part : tail) {
            move(part.x, part.y);
            printw("#");
        }
    }

    //Координаты головы змеи
    snake_head.x += step.x;
    snake_head.y += step.y;

    move(snake_head.x, snake_head.y);
    auto s = static_cast<char>(winch(stdscr));
    if (s == '*' || s == '#') {
        attrset(A_BOLD | COLOR_PAIR(4));
        move(13, 55);
        printw("Конец игры");
        move(14, 42);
        printw("Выход - < q >  Начать заново - < n >");
        // *** Цикл выбора игрового меню ***
        do {
            if (getch() == 'q') {
                // выход из игры
                game_exit = true;
                return;
            }
            if (getch() == 'n')
            {
                // рестарт задаём начальные координаты переменным
                snake_head = { 10,10 };
                step = { 0, 1 };
                tail.clear();
                apple = { 15,15 };
                eaten_apples = 0;
                return;
            }
        } while (true);
        // *** Конец цикла выбора игрового меню ***

    }
    
    if (s == '@') {
        eaten_apples++;
        tail.push_back({snake_head.x, snake_head.y});
        printw("$");

        do {
            apple.x = rand_apple.x;
            apple.y = rand_apple.y;
            move(apple.y, apple.x);
            auto s = static_cast<char>(winch(stdscr));
            
        } while (s == '#');
    }
    else {
        printw("$");
        //update tail snake
        if (!tail.empty()) {
            tail.erase(tail.begin());
            tail.push_back({ snake_head.x, snake_head.y });
        }
    }

}

void startingSettings() {


}

int main()
{
    system("chcp 1251");

    initscr();
    curs_set(0);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_MAGENTA ,COLOR_BLUE);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);
    init_pair(4, COLOR_RED, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);

    bkgd(COLOR_PAIR(1));

    vector2d snake_head{ 10, 10 };
    vector2d vector_step{ 0, 1 };
    vector2d apple{ 15, 15 };
    vector2d rand_apple;
    vector<vector2d> snake_tail;

    int frame_rate = 500;
    int eaten_apples = 0;

    //Генератор случайных чисел 
    long long seed = system_clock::now().time_since_epoch().count();
    default_random_engine rnd(static_cast<unsigned int>(seed));
    uniform_int_distribution<int> apple_x(6, 99);
    uniform_int_distribution<int> apple_y(5, 24);

    keypad(stdscr, true); //возможность использовать клавиатуру
    noecho(); //Отключает отображение нажимаемых клавиш;

    while (!game_exit) {
        rand_apple.x = apple_x(rnd);
        rand_apple.y = apple_y(rnd);

        showMap(apple, eaten_apples);
        moveSnake(snake_head, vector_step, snake_tail, apple, rand_apple, eaten_apples);
        timeout(frame_rate);

        switch (getch()) {
        case KEY_UP:
            if (vector_step.x == 0) {
                vector_step.x = -1;
                vector_step.y = 0;
                frame_rate = 170;
            
            }
            break;
        case KEY_DOWN:
            if (vector_step.x == 0) {
                vector_step.x = 1;
                vector_step.y = 0;
                frame_rate = 170;
            }
            break;
        case KEY_LEFT:
            if (vector_step.y == 0) {
                vector_step.y = -1;
                vector_step.x = 0;
                frame_rate = 170;

            }
            break;
        case KEY_RIGHT:
            if (vector_step.y == 0) {
                vector_step.y = 1;
                vector_step.x = 0;
                frame_rate = 170;
            }
            break;
        case 'q':
            game_exit = true;
            break;
        }
    }
    endwin(); 
}

