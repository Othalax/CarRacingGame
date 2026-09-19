#include "State.h"

#include "Config.h"

#include <iostream>

std::string State::player1car;
std::string State::player2car;
std::vector<MapEntry> State::maps = {};
int State::current_map = 0;

State::State(sf::RenderWindow* window,
             std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys)
    : window(window), supportedKeys(std::move(supportedKeys)), nextState(nullptr) {

    if (State::maps.empty()) {
        State::player1car = Config::instance().get_default_player1_car();
        State::player2car = Config::instance().get_default_player2_car();
        State::maps = Config::instance().get_maps();
        State::current_map = Config::instance().get_default_current_map();
    }

    if (textures.empty()) {
        this->init_textures();
    }

    init_font();
}

const bool& State::get_quit() const {
    return this->quit;
}

std::unique_ptr<State> State::get_next_state() {
    return std::move(this->nextState);
}

void State::end_state() {
    this->quit = true;
}

sf::RenderWindow& State::get_window() const {
    return *window;
}

std::unordered_map<std::string, sf::Texture>& State::get_textures() {
    return textures;
}

const std::unordered_map<std::string, sf::Texture>& State::get_textures() const {
    return textures;
}

sf::Font& State::get_font() {
    return font;
}

const sf::Font& State::get_font() const {
    return font;
}

const std::unordered_map<std::string, sf::Keyboard::Key>& State::get_supported_keys() const {
    return supportedKeys;
}

const sf::Vector2i& State::get_mouse_pos_screen() const {
    return mousePosScreen;
}

const sf::Vector2i& State::get_mouse_pos_window() const {
    return mousePosWindow;
}

const sf::Vector2f& State::get_mouse_pos_view() const {
    return mousePosView;
}

void State::set_next_state(std::unique_ptr<State> state) {
    nextState = std::move(state);
}

void State::init_textures() {
    const auto& texture_paths = Config::instance().get_texture_paths();

    for (const auto& [textureName, texturePath] : texture_paths) {
        sf::Texture texture;
        if (texture.loadFromFile(texturePath)) {
            textures[textureName] = std::move(texture);
        }
    }
}

void State::init_font() {
    if (!this->font.openFromFile(Config::instance().get_font_path())) {
        std::cout << "Error loading font\n";
    }
}

void State::update_mouse_pos() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void State::update(const float& dt) {}

void State::render(sf::RenderTarget& target) {}
