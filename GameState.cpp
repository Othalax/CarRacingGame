#include "GameState.h"
#include "WinningState.h"

GameState::GameState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string, 
                        sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
      player1(
            {
              {"forward", sf::Keyboard::Key::W},
              {"backward", sf::Keyboard::Key::S},
              {"left", sf::Keyboard::Key::A},               
              {"right", sf::Keyboard::Key::D}
          },
          textures[State::player1car.empty() ? "redCar" : State::player1car],
          0.f, 0.f, 0.f, 20.f, 30.f, 100.f
      ),
      player2(
          {
              {"forward", sf::Keyboard::Key::Up},
              {"backward", sf::Keyboard::Key::Down},
              {"left", sf::Keyboard::Key::Left},
              {"right", sf::Keyboard::Key::Right}
          },
          textures[State::player2car.empty() ? "blueCar" : State::player2car],
           0.f, 0.f, 0.f, 20.f, 30.f, 100.f
      )
{
    if (!this->track.load("config/map1.json", "textures/map1.png")) {
        throw std::runtime_error("Failed to load map");
    }

    this->player1.setPosition(this->track.getPlayer1Spawn()); 
    this->player2.setPosition(this->track.getPlayer2Spawn());
}

void GameState::update(const float& dt){
    this->player1.update(dt);
	this->player1.handleCollision(this->player2);
    this->player2.update(dt);
	this->player2.handleCollision(this->player1);

    for (const auto& wall : this->track.getWalls()) {
        this->player1.handleWallCollision(wall.vertices);
        this->player2.handleWallCollision(wall.vertices);
    }
    if (this->player1.checkWinning(this->track.getFinishLine().vertices)) {
        this->nextState = std::make_unique<WinningState>(this->window, supportedKeys, '1');
	}
    if (this->player2.checkWinning(this->track.getFinishLine().vertices)) {
        this->nextState = std::make_unique<WinningState>(this->window, supportedKeys, '2');
    }
}

void GameState::render(sf::RenderTarget& target) {
    this->window->draw(this->track, sf::RenderStates::Default);

    this->player1.render(*this->window);
    this->player2.render(*this->window);
}