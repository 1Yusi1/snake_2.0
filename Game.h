#pragma once
#include "Apple.h"
#include "Snake.h"
#include "Border.h"
#include "Interface.h"
#include "DisplayManager.h"


#include <vector>
#include <curses.h>

class Game {
public:
    Game(int numberOfPlayer);

    void updateFrame();

    //rename
    void updateStatus(const char s, Snake& snake);

    void start() { 
        handleKeyPress();
    }


    ~Game() {
        endwin();
    }


private:
    void applySettings();

    void arrowControl(Snake& snake, int ch);

    void WASDControl(Snake& snake, int ch);

    void reset() {
        for (auto& x : snakes) {
            x.reset();
        }

    }

    //Обработка нажатий на кнопку
    void handleKeyPress();

    std::vector<Snake> snakes;
    Apple apple;
    Border border;
    Interface interface;
    std::vector<Drawable*> elements;
    DisplayManager displayManager;
};