#pragma once
#include "Drawable.h"
#include <iostream>
#include "curses.h"


class Border : public Drawable {
public:
    Border() {}

    Border(int x1, int x2, int y1, int y2) : x({ x1, x2 }), y({ y1, y2 }) {}

    Border(std::pair<int, int> _x, std::pair<int, int> _y) : x(_x), y(_y) {}

    void draw() override;

private:
    std::pair<int, int> x; //replace with vector2d //заменить на
    std::pair<int, int> y;
};