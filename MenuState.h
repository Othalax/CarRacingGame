#pragma once

#include "Game.h"
#include "State.h"
#include "Button.h"

class MenuState
    : public State
{
    public:
        MenuState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                    sf::Keyboard::Key> supportedKeys);
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

