#include "GameState.h"

GameState::GameState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string, 
                        sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>& states)
    : State(window, supportedKeys, states),
      player1(
            {
              {"forward", sf::Keyboard::Key::W},
              {"backward", sf::Keyboard::Key::S},
              {"left", sf::Keyboard::Key::A},               
              {"right", sf::Keyboard::Key::D}
          },
          textures[State::player1car.empty() ? "pinkCar" : State::player1car],
          400.f, 200.f, 0.f, 90.f, 30.f, 200.f
      ),
      player2(
          {
              {"forward", sf::Keyboard::Key::Up},
              {"backward", sf::Keyboard::Key::Down},
              {"left", sf::Keyboard::Key::Left},
              {"right", sf::Keyboard::Key::Right}
          },
          textures[State::player2car.empty() ? "blueCar" : State::player2car],
           00.f, 200.f, 0.f, 90.f, 30.f, 200.f
      )
{
    if (State::player1car.empty()) {
        State::player1car = "pinkCar";
    }
    if (State::player2car.empty()) {
        State::player2car = "blueCar";
    }
}

void GameState::update(const float& dt){
    this->player1.update(dt);
	this->player1.handleCollision(this->player2);
    this->player2.update(dt);
	this->player2.handleCollision(this->player1);
}

void GameState::render(sf::RenderTarget& target){
    this->player1.render(*this->window);
    this->player2.render(*this->window);
}
