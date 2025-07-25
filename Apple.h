#pragma once
#include "Drawable.h"
#include <chrono>
#include <random>
#include <curses.h>


class Apple : public Drawable {
public:
    Apple() {}

    Apple(const vector2d& startCords, int color) : cords(startCords), numberOfColorPair(color) {}

    void draw() override;


    void setCords(const vector2d& value) {
        cords = value;
    }

    void placeInNewLocation();

    Apple(const Apple& _apple) = default;

private:
    static vector2d getRandomCords();

    vector2d cords;
    int numberOfColorPair;

};
