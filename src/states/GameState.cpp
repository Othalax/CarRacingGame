#include "GameState.h"

#include "Config.h"
#include "State.h"
#include "WinningState.h"

GameState::GameState(sf::RenderWindow* window,
                     std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys)
    : State(window, std::move(supportedKeys)),
      player1(Config::instance().get_player_controls("player1"),
              State::get_textures().at(State::player1car.empty()
                                           ? Config::instance().get_default_player1_car()
                                           : State::player1car)),
      player2(Config::instance().get_player_controls("player2"),
              State::get_textures().at(State::player2car.empty()
                                           ? Config::instance().get_default_player2_car()
                                           : State::player2car)) {
    const auto& current_map = State::maps.at(State::current_map);
    if (!this->track.load(current_map.configPath, current_map.texture)) {
        throw std::runtime_error("Failed to load map");
    }

    this->player1.set_position(this->track.get_player1_spawn(), current_map.startingAngle);
    this->player2.set_position(this->track.get_player2_spawn(), current_map.startingAngle);
}

void GameState::update(const float& dt) {
    this->player1.update(dt);
    this->player1.handle_collision(this->player2);
    this->player2.update(dt);
    this->player2.handle_collision(this->player1);

    for (const auto& wall : this->track.get_walls()) {
        this->player1.handle_wall_collision(wall.vertices);
        this->player2.handle_wall_collision(wall.vertices);
    }
    if (this->player1.check_winning(this->track.get_finish_line().vertices)) {
        State::set_next_state(
            std::make_unique<WinningState>(&State::get_window(), State::get_supported_keys(), '1'));
    }
    if (this->player2.check_winning(this->track.get_finish_line().vertices)) {
        State::set_next_state(
            std::make_unique<WinningState>(&State::get_window(), State::get_supported_keys(), '2'));
    }
}

void GameState::render(sf::RenderTarget& /*target*/) {
    State::get_window().draw(this->track, sf::RenderStates::Default);

    this->player1.render(State::get_window());
    this->player2.render(State::get_window());
}
