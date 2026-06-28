#pragma once

#include "Game.h"
#include "GameState.h"
#include "Button.h"
#include "SettingState.h"

class MenuState
    : public State
{
    public:
        MenuState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                    sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>& states);
        virtual ~MenuState() = default;

        void updateButtons();
        void update(const float& dt);
        void render(sf::RenderTarget& target);

    private:
        Button gamestate;
        Button settings;
        Button exit;

		sf::Sprite background;
};

