#include "Interface.h"

void Interface::draw() {
    move(2, 55);
    attrset(A_DIM | COLOR_PAIR(1)); //устанавливаются атрибуты для текста далее
    printw("Змейка\t\t");
    attrset(A_BOLD | COLOR_PAIR(5));
    printw("Количество собранных яблок\t\t");
    printw(std::to_string(eatenApples).c_str());
    printw(" > ");
}

void Interface::endGame() {
    attrset(A_BOLD | COLOR_PAIR(4));
    move(13, 55);
    printw("Конец игры");
    move(14, 42);
    printw("Выход - < q >  Начать заново - < n >");
    eatenApples = 0;
}