#include "Border.h"

void Border::draw() {
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