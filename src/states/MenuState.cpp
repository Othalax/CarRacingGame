#include "MenuState.h"

#include "Config.h"
#include "GameState.h"
#include "SettingState.h"
#include "State.h"

MenuState::MenuState(sf::RenderWindow* window,
                     std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys)
    : State(window, std::move(supportedKeys)),
      gamestate(Config::instance().get_menu_button("start").x,
                Config::instance().get_menu_button("start").y,
                Config::instance().get_menu_button("start").width,
                Config::instance().get_menu_button("start").height,
                Config::instance().get_menu_button("start").text,
                &State::get_textures().at("baseButton"),
                &State::get_textures().at("baseButtonClicked"), State::get_font()),
      settings(Config::instance().get_menu_button("settings").x,
               Config::instance().get_menu_button("settings").y,
               Config::instance().get_menu_button("settings").width,
               Config::instance().get_menu_button("settings").height,
               Config::instance().get_menu_button("settings").text,
               &State::get_textures().at("baseButton"),
               &State::get_textures().at("baseButtonClicked"), State::get_font()),
      exit(Config::instance().get_menu_button("exit").x,
           Config::instance().get_menu_button("exit").y,
           Config::instance().get_menu_button("exit").width,
           Config::instance().get_menu_button("exit").height,
           Config::instance().get_menu_button("exit").text, &State::get_textures().at("baseButton"),
           &State::get_textures().at("baseButtonClicked"), State::get_font()),
      background(State::get_textures().at(Config::instance().get_menu_background())) {}

void MenuState::update_buttons() {
    if (this->exit.is_pressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        this->end_state();
    }

    if (this->gamestate.is_pressed()) {
        State::set_next_state(
            std::make_unique<GameState>(&State::get_window(), State::get_supported_keys()));
    }

    if (this->settings.is_pressed()) {
        State::set_next_state(
            std::make_unique<SettingState>(&State::get_window(), State::get_supported_keys()));
    }
}

void MenuState::update(const float& /*dt*/) {
    update_mouse_pos();
    this->gamestate.update(State::get_mouse_pos_view());
    this->exit.update(State::get_mouse_pos_view());
    this->settings.update(State::get_mouse_pos_view());
    update_buttons();
}

void MenuState::render(sf::RenderTarget& target) {
    State::get_window().draw(this->background, sf::RenderStates::Default);

    this->gamestate.render(target);
    this->exit.render(target);
    this->settings.render(target);
}
