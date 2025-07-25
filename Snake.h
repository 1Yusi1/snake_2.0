#pragma once
#include "Drawable.h"
#include <curses.h>
#include <vector>

class Snake : public Drawable {
public:
    Snake() {}

    Snake(const vector2d& startCords) : head(startCords) {}

    Snake(const vector2d& startCords, const vector2d& startStep) : head(startCords), step(startStep) {}

    void draw() override;

    char checkNext();

    void stepForward();

    void reset();

    void reset(const vector2d& defaultHead, const vector2d& defaultStep);

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
 


private:
    vector2d head = { 10, 10 };
    std::vector<vector2d> tail;
    vector2d step = { 0, 1 }; // 1-vertical, 2-horizontal
    int eaten_apples = 0;

};