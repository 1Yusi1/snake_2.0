#include "Game.h"
#include "Config.h"


bool game_exit = 0;

Game::Game(int numberOfPlayer) : apple({ 10, 15 }, 2), border({ 5, 112 }, { 4, 28 }) {
    applySettings();
    Config config;

    snakes.emplace_back(Snake({ 10, 10 }, { 0, 1 }));
    if (numberOfPlayer == 2) {
        snakes.emplace_back(Snake({ 10, 102 }, { 0, -1 }));
    }

    elements.emplace_back(&apple);
    elements.emplace_back(&border);
    elements.emplace_back(&interface);
    for (auto& x : snakes) {
        elements.emplace_back(&x);  
    }

}

void Game::updateFrame() {
    clear();
    for (const auto& x : elements) {
        x->draw();
    }
}

void Game::updateStatus(const char s, Snake& snake) {
    if (s == '#' || s == '*') {
        interface.endGame();
        do {
            int ch = getch();
            if (ch == 'q') {
                game_exit = true;
                return;

            }
            if (ch == 'n') {
                reset();
                return;
            }

        } while (true);
        return;
    }
    if (s == '@') {
        snake.eatApple();
        interface.addApple();
        apple.placeInNewLocation();
        return;
    }
    else {
        snake.stepForward();
    }
    snake.addStep();
}

void Game::applySettings() {
    system("chcp 1251"); //enabling russian symbols

    initscr();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);  //background
    init_pair(2, COLOR_MAGENTA, COLOR_BLUE); //apple
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  //snake
    init_pair(4, COLOR_RED, COLOR_BLUE);    //endgame
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);  //text

    bkgd(COLOR_PAIR(1));

    keypad(stdscr, true); //возможность использовать клавиатуру
    noecho();
    timeout(150);
}

void Game::arrowControl(Snake& snake, int ch) {
    switch (ch) {
    case KEY_UP:
        if (snake.getStep().x == 0) {
            snake.setStep({ -1, 0 });

        }
        break;
    case KEY_DOWN:
        if (snake.getStep().x == 0) {
            snake.setStep({ 1, 0 });

        }
        break;
    case KEY_LEFT:
        if (snake.getStep().y == 0) {
            snake.setStep({ 0, -1 });

        }
        break;
    case KEY_RIGHT:
        if (snake.getStep().y == 0) {
            snake.setStep({ 0, 1 });
        }
        break;
    case 'q':
        game_exit = true;
        break;
    }
}

void Game::WASDControl(Snake& snake, int ch) {
    switch (ch) {
    case 'w':
        if (snake.getStep().x == 0) {
            snake.setStep({ -1, 0 });

        }
        break;
    case 's':
        if (snake.getStep().x == 0) {
            snake.setStep({ 1, 0 });

        }
        break;
    case 'a':
        if (snake.getStep().y == 0) {
            snake.setStep({ 0, -1 });

        }
        break;
    case 'd':
        if (snake.getStep().y == 0) {
            snake.setStep({ 0, 1 });
        }
        break;
    case 'q':
        game_exit = true;
        break;
    }
}

//Обработка нажатий на кнопку
void Game::handleKeyPress() {
    std::vector<int> arrow = { KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT };
    std::vector<int> WASD = { 'w', 'a', 's', 'd' };
    while (!game_exit) {
        displayManager.drawAll(elements);
        for (auto& x : snakes) {
            updateStatus(x.checkNext(), x);
        }

        int ch = getch();
        if (std::find(WASD.begin(), WASD.end(), ch) != WASD.end() && snakes.size() == 2) {
            WASDControl(snakes[0], ch);
        }
        if (std::find(arrow.begin(), arrow.end(), ch) != arrow.end()) {
            arrowControl(snakes[1], ch);
        }

    }
}