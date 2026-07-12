#include "GameState.h"
#include "WinningState.h"
#include "Config.h"

GameState::GameState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string, 
                        sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
      player1(
          Config::instance().getPlayerControls("player1"),
          textures[State::player1car.empty() ? Config::instance().getDefaultPlayer1Car() : State::player1car]
      ),
      player2(
          Config::instance().getPlayerControls("player2"),
          textures[State::player2car.empty() ? Config::instance().getDefaultPlayer2Car() : State::player2car]
      )
{
    const auto& currentMap = State::maps[State::currentMap];
    if (!this->track.load(currentMap.configPath, currentMap.texture)) {
        throw std::runtime_error("Failed to load map");
    }

    this->player1.setPosition(this->track.getPlayer1Spawn(), currentMap.startingAngle);
    this->player2.setPosition(this->track.getPlayer2Spawn(), currentMap.startingAngle);
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
