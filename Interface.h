#pragma once
#include "Drawable.h"
#include "string"
#include "curses.h"


class Interface : public Drawable {
public:
    Interface() {
    }

    void draw() override;

    void addApple() { eatenApples++; } //FIXME --delete

    void endGame();

private:
    int eatenApples = 0;   //FIXME -- Interface must not depend of eatenApple
};