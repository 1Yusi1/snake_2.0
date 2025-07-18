#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <curses.h>

using namespace std;
using namespace std::chrono;

//интерфейс для рисования элементов окна
class Drawable {
public:
    virtual void draw() = 0;
};

struct vector2d {
    int x = 0;
    int y = 0;
};

bool game_exit = false;

class Apple : public Drawable{
public:
    Apple() {}

    Apple(const vector2d& startCords, int color) : cords(startCords), numberOfColorPair(color) {}

    void draw() override {
        move(cords.x, cords.y);
        attrset(A_BOLD | COLOR_PAIR(numberOfColorPair));
        printw("@");
    }


    void setCords(const vector2d& value) {
        cords = value;
    }

    void placeInNewLocation() {
        char s = ' ';
        do {
            cords = getRandomCords();
            move(cords.x, cords.y);
            auto s = static_cast<char>(winch(stdscr));

        } while (s == '#');
        
    }

    Apple(const Apple& _apple) {
        this->cords.x = _apple.cords.x;
        this->cords.y = _apple.cords.y;
        this->numberOfColorPair = _apple.numberOfColorPair;
    }

private:
    static vector2d getRandomCords() {
        default_random_engine rand(static_cast<unsigned int>(system_clock::now().time_since_epoch().count()));
        uniform_int_distribution<int> apple_x(5, 26);
        uniform_int_distribution<int> apple_y(6, 109);


        return { apple_x(rand), apple_y(rand) };
    }


    vector2d cords;
    int numberOfColorPair;

};

class Snake : public Drawable {
public:
    Snake() {}

    Snake(const vector2d& startCords) : head(startCords) {
        
    }

    void draw() override {
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

    char checkNext() {
        move(head.x + step.x, head.y + step.y);
        auto s = static_cast<char>(winch(stdscr));

        return s;
        
    }

    void stepForward() {
        if (!tail.empty()) {
            tail.erase(tail.begin());
            tail.push_back({ head.x, head.y });
        }
    }

    void addStep() {
        head.x += step.x;
        head.y += step.y;
    }

    int getEatenApples() { return eaten_apples; }

    void eatApple() { 
        eaten_apples++; 
        tail.push_back({ head.x, head.y });
    
    }

    vector2d getStep() {
        return step;
    }

    void setStep(const vector2d& value) {
        step.x = value.x;
        step.y = value.y;
    }

    void reset() {
        head = { 10, 10 };
        tail.clear();
        eaten_apples = 0;
        step = { 0, 1 };
    }

private:
    vector2d head = {10, 10};
    vector<vector2d> tail;
    vector2d step = { 0, 1 }; // 1-vertical, 2-horizontal
    int eaten_apples = 0;

};

class Border : public Drawable {
public:
    Border() {}
    Border(int x1, int x2, int y1, int y2) : x({ x1, x2 }), y({ y1, y2 }) {}
    Border(pair<int, int> _x, pair<int, int> _y) : x(_x), y(_y) {}

    void draw() override {
        attrset(A_DIM | COLOR_PAIR(1));
        for (auto& value : { y.first, y.second }) {
            for (int _x = x.first; _x < x.second; _x++) {
                move(value, _x);
                printw("*");
            }
        }

        for (auto& value : { x.first, x.second }) {
            for (int _y = y.first; _y < y.second; _y++) {
                move(_y, value);
                printw("*");
            }
        }
    }



private:
    pair<int, int> x;
    pair<int, int> y;
};

class Interface  : public Drawable  {
public:
    Interface() {
    }

    void draw() override {
        move(2, 55);
        attrset(A_DIM | COLOR_PAIR(1)); //устанавливаются атрибуты для текста далее
        printw("Змейка\t\t");
        attrset(A_BOLD | COLOR_PAIR(5));
        printw("Количество собранных яблок\t\t");
        printw(to_string(eatenApples).c_str());
        printw(" > ");
    }

    void endGame() {
        attrset(A_BOLD | COLOR_PAIR(4));
        move(13, 55);
        printw("Конец игры");
        move(14, 42);
        printw("Выход - < q >  Начать заново - < n >");
    }
private:
    int eatenApples = 0;
    
};

class Game {
public:
    Game(int numberOfPlayer) : snake({ 10, 10 }), apple({ 10, 15 }, 2), border({ 5, 112 }, { 4, 28 }) {
        applySettings();
        elements.emplace_back(&snake);
        elements.emplace_back(&apple);
        elements.emplace_back(&border);
        elements.emplace_back(&interface);

    }

    void updateFrame() {
        clear();

        for (const auto& x : elements) {
            x->draw();

        }

    }

    //rename
    void updateStatus(const char s) {
        if (s == '#' || s == '*') {
            interface.endGame();
            do {
                if (getch() == 'q') {
                    game_exit = true;
                    return;

                }
                if (getch() == 'n') {
                    snake.reset();
                    return;
                }

            } while (true);
            return;   
        }
        if (s == '@') {
            snake.eatApple();
            apple.placeInNewLocation();
            return;
        }
        else {
            snake.stepForward();
        } 
        snake.addStep();
    }

    void start() {  //стрелочки и бесконечный цикл
        while (!game_exit) {
            updateFrame();
            updateStatus(snake.checkNext());
            timeout(150);
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


    ~Game() {
        endwin();
    }


private:
    void applySettings() {
        system("chcp 1251"); //enabling russian symbols

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
    Apple apple;
    Border border;
    Interface interface;
    vector<Drawable*> elements;

};

int main()
{
    Game game(1);
    game.start();

}

