#ifndef GAME_H
#define GAME_H

#include <GameState.h>
#include <MenuState.h>

class Game {
private:
    std::unique_ptr<sf::RenderWindow> window;
    sf::View view;

    sf::Clock dtClock;
    float dt;

    std::vector<std::unique_ptr<State>> states;
    std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys;

    void initWindow();
    void initStates();
    void initKeybinds();

public:
    Game();
    virtual ~Game();

    void endApp();
    void updateDT();
    void updateEvents();
    void update();
    void render();
    void run();
};

#endif // GAME_H
