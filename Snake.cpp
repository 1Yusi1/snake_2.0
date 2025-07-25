#include "Snake.h"

void Snake::draw() {
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

char Snake::checkNext() {
    move(head.x + step.x, head.y + step.y);
    auto s = static_cast<char>(winch(stdscr));

    return s;
}

void Snake::stepForward() {
    if (!tail.empty()) {
        tail.erase(tail.begin());
        tail.push_back({ head.x, head.y });
    }
}

void Snake::reset() {
    head = { 10, 10 };
    step = { 0, 1 };
    tail.clear();
    eaten_apples = 0;
}

void Snake::reset(const vector2d& defaultHead, const vector2d& defaultStep) {
    head = defaultHead;
    step = defaultStep;
    tail.clear();
    eaten_apples = 0;
}