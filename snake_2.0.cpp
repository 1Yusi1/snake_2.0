#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <curses.h>

using namespace std;
using namespace std::chrono;

//шаблонный класс для рисования объектов

struct vector2d {
    int x = 0;
    int y = 0;
};

bool game_exit = false;

class Apple {
public:
    Apple() {}

    Apple(const vector2d startCords, int color) : cords(startCords), numberOfColorPair(color) {}

    void drawInTheMap() {
        move(cords.x, cords.y);
        attrset(A_BOLD | COLOR_PAIR(numberOfColorPair));
        printw("@");
    }

    vector2d getCords() {
        return cords;
    }

    void setCords(vector2d value) {
        cords.x = value.x;
        cords.y = value.y;
        //cords = value;
    }

    static vector2d getRandomCords() {
        default_random_engine rand(static_cast<unsigned int>(system_clock::now().time_since_epoch().count()));
        uniform_int_distribution<int> apple_x(4, 26);
        uniform_int_distribution<int> apple_y(5, 109);


        return {apple_x(rand), apple_y(rand)};
    }

    Apple(const Apple& _apple) {
        this->cords.x = _apple.cords.x;
        this->cords.y = _apple.cords.y;
        this->numberOfColorPair = _apple.numberOfColorPair;
    }

private:
    vector2d cords;
    int numberOfColorPair;

};

class Snake {
public:
    Snake() {}

    //rewrite (must work base)
    Snake(const vector2d startCords, const Apple& _apple) {
        head.x = startCords.x;
        head.y = startCords.y;
        this->apple = _apple;
        
    }
    void next() {
        attrset(A_BOLD | COLOR_PAIR(3));

        if (!tail.empty()) {
            for (const auto& part : tail) {
                move(part.x, part.y);
                printw("#");
            }
        }

        head.x += step.x; // operator +=
        head.y += step.y;

        move(head.x, head.y);
        auto s = static_cast<char>(winch(stdscr));

        if (s == '*' || s == '#') {
            attrset(A_BOLD | COLOR_PAIR(4));
            move(13, 55);
            printw("Конец игры");
            move(14, 42);
            printw("Выход - < q >  Начать заново - < n >");

            do {
                if (getch() == 'q') {
                    game_exit = true;
                    return;
                    
                }
                if (getch() == 'n') {
                    //начальные значения
                    head = { 10,10 };
                    step = { 0, 1 };
                    tail.clear();
                    apple.setCords({15, 15});
                    eaten_apples = 0;
                    
                }
            
            } while (true);

        }
        if (s == '@') {
            eaten_apples++;
            tail.push_back({ head.x, head.y });
            do {
                apple.setCords(Apple::getRandomCords());
                move(apple.getCords().x, apple.getCords().y);
                auto s = static_cast<char>(winch(stdscr));
            
            } while (s == '#');
        
        }
        else {
            printw("$");
            //update tail snake
            if (!tail.empty()) {
                tail.erase(tail.begin());
                tail.push_back({ head.x, head.y });
            }    
        }
        
    }

    int getEatenApples() { return eaten_apples; }

    vector2d getStep() {
        return this->step;
    }

    void setStep(vector2d value) {
        step.x = value.x;
        step.y = value.y;
    }

    Apple& getApple() {
        return apple;
    }

    void drawInTheMap() {
        attrset(A_BOLD | COLOR_PAIR(3));

        if (!tail.empty()) {
            for (const auto& part : tail) {
                move(part.x, part.y);
                printw("#");
            }
        } 

        move(head.x, head.y);
        printw("$");

    }

private:
    vector2d head = {10, 10};
    vector<vector2d> tail;
    vector2d step = { 0, 1 }; // x, y
    Apple apple;
    int eaten_apples = 0;

};

class Game {
public:
    Game(int numberOfPlayer) : snake({ 10, 10 }, Apple({ 10, 15 }, 2)) {
        applySettings();
    }

    void start() {  //стрелочки и бесконечный цикл
        while (!game_exit) {
            updateFrame();
            snake.next();
            timeout(170);
            switch (getch()) {
            case KEY_UP:
                if (snake.getStep().x == 0) {
                    snake.setStep({ -1, 0 });

                }
                break;
            case KEY_DOWN:
                if (snake.getStep().x == 0) {
                    snake.setStep({ 1, 0 });

                }
                break;
            case KEY_LEFT:
                if (snake.getStep().y == 0) {
                    snake.setStep({ 0, -1 });

                }
                break;
            case KEY_RIGHT:
                if (snake.getStep().y == 0) {
                    snake.setStep({ 0, 1 });
                }
                break;
            case 'q':
                game_exit = true;
                break;
            }
        }


    
    }

    void updateFrame() {
        clear();
        move(2, 55);

        attrset(A_DIM | COLOR_PAIR(1)); //устанавливаются атрибуты для текста далее
        printw("Змейка\t\t");
        attrset(A_BOLD | COLOR_PAIR(5));
        printw("Количество собранных яблок\t\t");
        printw(to_string(snake.getEatenApples()).c_str());
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

        snake.getApple().drawInTheMap();
    }

    ~Game() {
        endwin();
    }


private:
    void applySettings() {
        system("chcp 1251");

        initscr();
        curs_set(0);
        start_color();

        init_pair(1, COLOR_WHITE, COLOR_BLUE);  //background
        init_pair(2, COLOR_MAGENTA, COLOR_BLUE); //apple
        init_pair(3, COLOR_GREEN, COLOR_BLUE);  //snake
        init_pair(4, COLOR_RED, COLOR_BLUE);    //endgame
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);  //text

        bkgd(COLOR_PAIR(1));

        keypad(stdscr, true); //возможность использовать клавиатуру
        noecho();
    }
    Snake snake;
    //int colorBkgd;  //потом попробовать
};

int main()
{
    Game game(1);
    game.start();


}

