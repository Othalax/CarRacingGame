#include "WinningState.h"

#include "Config.h"
#include "GameState.h"
#include "MenuState.h"
#include "State.h"

WinningState::WinningState(sf::RenderWindow* window,
                           std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys,
                           char winner)
    : State(window, std::move(supportedKeys)),
      mainMenu(Config::instance().get_winning_button("mainMenu").x,
               Config::instance().get_winning_button("mainMenu").y,
               Config::instance().get_winning_button("mainMenu").width,
               Config::instance().get_winning_button("mainMenu").height,
               Config::instance().get_winning_button("mainMenu").text,
               &State::get_textures().at("baseButton"),
               &State::get_textures().at("baseButtonClicked"), State::get_font()),
      playAgain(Config::instance().get_winning_button("playAgain").x,
                Config::instance().get_winning_button("playAgain").y,
                Config::instance().get_winning_button("playAgain").width,
                Config::instance().get_winning_button("playAgain").height,
                Config::instance().get_winning_button("playAgain").text,
                &State::get_textures().at("baseButton"),
                &State::get_textures().at("baseButtonClicked"), State::get_font()),
      background(State::get_textures().at(Config::instance().get_winning_background())),
      winningText(State::get_font(), "Player " + std::string(1, winner) + " wins!",
                  Config::instance().get_winning_text().fontSize),
      scoresText(State::get_font(), "", Config::instance().get_winning_scores_text().fontSize) {
    const auto winning_text_layout = Config::instance().get_winning_text();
    const auto scores_text_layout = Config::instance().get_winning_scores_text();

    winningText.setFillColor(sf::Color::Black);
    winningText.setPosition(sf::Vector2f(winning_text_layout.x, winning_text_layout.y));
    scoresText.setFillColor(sf::Color::Black);
    scoresText.setPosition(sf::Vector2f(scores_text_layout.x, scores_text_layout.y));
    init_scores(winner);
}

void WinningState::init_scores(char winner) {
    auto& config = Config::instance();
    int player1score = config.get_player1_score();
    int player2score = config.get_player2_score();

    if (winner == '1') {
        player1score += 1;
    } else if (winner == '2') {
        player2score += 1;
    }

    std::string message;

    if (player1score > player2score) {
        message = "Player 1: " + std::to_string(player1score) + '\n' +
                  "Player 2: " + std::to_string(player2score);
    } else {
        message = "Player 2: " + std::to_string(player2score) + '\n' +
                  "Player 1: " + std::to_string(player1score);
    }

    this->scoresText.setString(message);
    config.set_scores(player1score, player2score);
}

void WinningState::update_buttons() {
    if (this->mainMenu.is_pressed()) {
        State::set_next_state(
            std::make_unique<MenuState>(&State::get_window(), State::get_supported_keys()));
    }
    if (this->playAgain.is_pressed()) {
        State::set_next_state(
            std::make_unique<GameState>(&State::get_window(), State::get_supported_keys()));
    }
}

void WinningState::update(const float& /*dt*/) {
    update_mouse_pos();
    this->mainMenu.update(State::get_mouse_pos_view());
    this->playAgain.update(State::get_mouse_pos_view());
    update_buttons();
}

void WinningState::render(sf::RenderTarget& target) {
    State::get_window().draw(this->background, sf::RenderStates::Default);

    State::get_window().draw(this->winningText, sf::RenderStates::Default);
    State::get_window().draw(this->scoresText, sf::RenderStates::Default);
    this->mainMenu.render(target);
    this->playAgain.render(target);
}
