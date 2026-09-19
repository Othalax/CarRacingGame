#ifndef GAME_H
#define GAME_H

#include "State.h"

class Game {
  public:
    Game();
    virtual ~Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) = delete;

    void update_dt();
    void update_events();
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

    void init_window();
    void init_states();
    void init_keybinds();
};

#endif // GAME_H