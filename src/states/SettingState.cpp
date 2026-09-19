#include "SettingState.h"

#include "Config.h"
#include "MenuState.h"
#include "State.h"

#include <algorithm>
#include <utility>

SettingState::SettingState(sf::RenderWindow* window,
                           std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys)
    : State(window, std::move(supportedKeys)), carTypes(Config::instance().get_car_types()),
      player1view(State::get_textures().at(State::player1car)),
      player2view(State::get_textures().at(State::player2car)),
      background(State::get_textures().at(Config::instance().get_settings_background())),
      mapText(State::get_font(), Config::instance().get_settings_map_text().text,
              Config::instance().get_settings_map_text().fontSize),
      mapName(State::get_font(), State::maps.at(State::current_map).name,
              Config::instance().get_settings_map_name().fontSize) {

    this->init_buttons();

    const auto player1_layout = Config::instance().get_settings_player1_view();
    const auto player2_layout = Config::instance().get_settings_player2_view();
    const auto map_text_layout = Config::instance().get_settings_map_text();
    const auto map_name_layout = Config::instance().get_settings_map_name();
    const float logical_width = Config::instance().get_logical_size().x;

    this->player1view.setPosition(sf::Vector2f(player1_layout.x, player1_layout.y));
    this->player1view.setScale(sf::Vector2f(player1_layout.scale, player1_layout.scale));
    this->player1view.rotate(sf::degrees(player1_layout.rotation));
    this->player2view.setPosition(sf::Vector2f(player2_layout.x, player2_layout.y));
    this->player2view.setScale(sf::Vector2f(player2_layout.scale, player2_layout.scale));
    this->player2view.rotate(sf::degrees(player2_layout.rotation));

    this->mapText.setPosition(sf::Vector2f(
        (logical_width - this->mapText.getLocalBounds().size.x) / 2.F, map_text_layout.y));
    this->mapName.setPosition(sf::Vector2f(
        (logical_width - this->mapName.getLocalBounds().size.x) / 2.F, map_name_layout.y));
}

void SettingState::init_buttons() {
    const auto& config = Config::instance();

    auto add_button = [this, &config](const std::string& name, const std::string& defaultTexture,
                                      const std::string& hoverTexture) {
        const auto layout = config.get_settings_button(name);
        this->buttons.try_emplace(name, layout.x, layout.y, layout.width, layout.height,
                                  layout.text, &State::get_textures().at(defaultTexture),
                                  &State::get_textures().at(hoverTexture), State::get_font());
    };

    add_button("menu", "baseButton", "baseButtonClicked");
    add_button("player1arrowright", "arrowButtonRight", "arrowButtonRightClicked");
    add_button("player1arrowleft", "arrowButtonLeft", "arrowButtonLeftClicked");
    add_button("player2arrowright", "arrowButtonRight", "arrowButtonRightClicked");
    add_button("player2arrowleft", "arrowButtonLeft", "arrowButtonLeftClicked");
    add_button("resetScore", "baseButton", "baseButtonClicked");
    add_button("maparrowright", "arrowButtonRight", "arrowButtonRightClicked");
    add_button("maparrowleft", "arrowButtonLeft", "arrowButtonLeftClicked");
}

void SettingState::change_car(std::string& currentCar, sf::Sprite& view, int direction,
                              const std::string& other) {
    auto it = std::ranges::find(this->carTypes, currentCar);
    auto it_other = std::ranges::find(this->carTypes, other);

    if (it != this->carTypes.end()) {
        const int current_index = static_cast<int>(std::distance(this->carTypes.begin(), it));
        const int current_index_other =
            static_cast<int>(std::distance(this->carTypes.begin(), it_other));

        int new_index = current_index + direction;

        if (new_index == current_index_other) {
            new_index += direction;
        }

        if (new_index >= 0 && std::cmp_less(new_index, this->carTypes.size())) {
            currentCar = this->carTypes.at(new_index);
        } else if (new_index < 0) {
            currentCar = this->carTypes.back();
            if (currentCar == other) {
                currentCar = this->carTypes.at(this->carTypes.size() - 2);
            }
        } else if (std::cmp_greater_equal(new_index, this->carTypes.size())) {
            currentCar = this->carTypes.front();
            if (currentCar == other) {
                currentCar = this->carTypes.at(1);
            }
        }
        view.setTexture(State::get_textures().at(currentCar));
    }
}

void SettingState::change_map(int direction) {
    const int new_index = State::current_map + direction;

    if (new_index >= 0 && std::cmp_less(new_index, State::maps.size())) {
        State::current_map = new_index;
    } else if (new_index < 0) {

        State::current_map = static_cast<int>(State::maps.size()) - 1;
    } else if (std::cmp_greater_equal(new_index, State::maps.size())) {
        State::current_map = 0;
    }

    this->mapName.setString(State::maps.at(State::current_map).name);
    this->mapName.setPosition(sf::Vector2f(
        (Config::instance().get_logical_size().x - this->mapName.getLocalBounds().size.x) / 2.F,
        Config::instance().get_settings_map_name().y));
}

void SettingState::update_buttons() {
    if (this->buttons.at("menu").is_pressed()) {
        State::set_next_state(
            std::make_unique<MenuState>(&State::get_window(), State::get_supported_keys()));
        return;
    }

    if (this->buttons.at("resetScore").is_pressed()) {
        Config::instance().reset_scores();
    } else if (this->buttons.at("player1arrowright").is_pressed()) {
        change_car(State::player1car, this->player1view, 1, State::player2car);
    } else if (this->buttons.at("player1arrowleft").is_pressed()) {
        change_car(State::player1car, this->player1view, -1, State::player2car);
    } else if (this->buttons.at("player2arrowright").is_pressed()) {
        change_car(State::player2car, this->player2view, 1, State::player1car);
    } else if (this->buttons.at("player2arrowleft").is_pressed()) {
        change_car(State::player2car, this->player2view, -1, State::player1car);
    } else if (this->buttons.at("maparrowright").is_pressed()) {
        change_map(1);
    } else if (this->buttons.at("maparrowleft").is_pressed()) {
        change_map(-1);
    }
}

void SettingState::update(const float& /*dt*/) {
    update_mouse_pos();
    for (auto& itr : this->buttons) {
        itr.second.update(State::get_mouse_pos_view());
    }
    update_buttons();
}

void SettingState::render(sf::RenderTarget& target) {
    target.draw(this->background, sf::RenderStates::Default);

    for (auto& itr : this->buttons) {
        itr.second.render(target);
    }

    target.draw(this->player1view);
    target.draw(this->player2view);

    target.draw(this->mapText);
    target.draw(this->mapName);
}