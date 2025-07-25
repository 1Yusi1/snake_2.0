#include "Apple.h"

void Apple::draw() {
    move(cords.x, cords.y);
    attrset(A_BOLD | COLOR_PAIR(numberOfColorPair));
    printw("@");
}

void Apple::placeInNewLocation() {
    char s = ' ';
    do {
        cords = getRandomCords();
        move(cords.x, cords.y);
        auto s = static_cast<char>(winch(stdscr));

    } while (s == '#');

}

 vector2d Apple::getRandomCords() {
    std::default_random_engine rand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> apple_x(5, 26);
    std::uniform_int_distribution<int> apple_y(6, 109);


    return { apple_x(rand), apple_y(rand) };
}