#include "Interface.h"

void Interface::draw() {
    move(2, 55);
    attrset(A_DIM | COLOR_PAIR(1)); //��������������� �������� ��� ������ �����
    printw("������\t\t");
    attrset(A_BOLD | COLOR_PAIR(5));
    printw("���������� ��������� �����\t\t");
    printw(std::to_string(eatenApples).c_str());
    printw(" > ");
}

void Interface::endGame() {
    attrset(A_BOLD | COLOR_PAIR(4));
    move(13, 55);
    printw("����� ����");
    move(14, 42);
    printw("����� - < q >  ������ ������ - < n >");
    eatenApples = 0;
}