#pragma once

#include "GameState.h"
#include "MenuState.h"

class Game {
public:
    Game();
    virtual ~Game() = default;

    void endApp();
    void updateDT();
    void updateEvents();
    void update();
    void render();
    void run();

private:
    std::unique_ptr<sf::RenderWindow> window;
    sf::View view;

    sf::Clock dtClock;
    float dt;

    std::unique_ptr<State> currentState;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;

    void initWindow();
    void initStates();
    void initKeybinds();
};

