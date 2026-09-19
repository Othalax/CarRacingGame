#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Car.h"
#include "Map.h"
#include "State.h"

class GameState : public State {
  public:
    GameState(sf::RenderWindow* window,
              std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys);
    ~GameState() override = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState(GameState&&) = delete;
    GameState& operator=(GameState&&) = delete;

    void update(const float& dt) override;
    void render(sf::RenderTarget& /*target*/) override;

  private:
    Car player1;
    Car player2;

    Map track;
};

#endif // GAMESTATE_H