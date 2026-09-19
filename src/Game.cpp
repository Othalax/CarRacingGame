#include "Game.h"

#include "Config.h"
#include "MenuState.h"

Game::Game() : dt(0.F) {
    this->init_window();
    this->init_keybinds();
    this->init_states();
}

void Game::init_window() {
    const auto& config = Config::instance();

    this->window.reset();
    this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(config.get_window_size()),
                                                      config.get_window_title());
    this->window->setFramerateLimit(config.get_framerate());
    this->window->setVerticalSyncEnabled(config.get_vertical_sync());
    this->view = this->window->getDefaultView();
}

void Game::init_keybinds() {
    this->supportedKeys = Config::instance().get_supported_keys();
}

void Game::init_states() {
    this->currentState = std::make_unique<MenuState>(this->window.get(), this->supportedKeys);
}

void Game::update_dt() {
    this->dt = this->dtClock.restart().asSeconds();
}

void Game::update_events() {
    while (const std::optional event = this->window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            this->window->close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            const float target_ratio = this->view.getSize().x / this->view.getSize().y;
            const float window_ratio =
                static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);

            float size_x = 1.F;
            float size_y = 1.F;
            float pos_x = 0.F;
            float pos_y = 0.F;

            if (window_ratio > target_ratio) {
                size_x = target_ratio / window_ratio;
                pos_x = (1.F - size_x) / 2.F;
            } else {
                size_y = window_ratio / target_ratio;
                pos_y = (1.F - size_y) / 2.F;
            }

            this->view.setViewport(sf::FloatRect({pos_x, pos_y}, {size_x, size_y}));
            this->window->setView(this->view);
        }
    }
}

void Game::update() {
    if (this->currentState == nullptr) {
        this->window->close();
        return;
    }

    this->currentState->update(this->dt);

    if (this->currentState->get_quit()) {
        this->currentState = nullptr;
        return;
    }

    auto next = this->currentState->get_next_state();
    if (next) {
        this->currentState = std::move(next);
    }
}

void Game::render() {
    const auto logical_size = Config::instance().get_logical_size();
    const sf::View view(sf::FloatRect({0.F, 0.F}, logical_size));

    this->window->setView(view);
    this->window->clear();

    if (this->currentState != nullptr) {
        this->currentState->render(*this->window);
    }

    this->window->display();
}

void Game::run() {
    while (this->window->isOpen()) {
        this->update_dt();
        this->update_events();
        this->update();
        this->render();
    }
}
