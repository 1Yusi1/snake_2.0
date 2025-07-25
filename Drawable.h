#pragma once

class Drawable {
public:
    virtual void draw() = 0;
};

struct vector2d {
    int x = 0;
    int y = 0;
};