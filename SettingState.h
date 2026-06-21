#pragma once

#include "MenuState.h"
#include "Button.h"

class SettingState
    : public State
{
    public:
        SettingState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                        sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>& states);
        virtual ~SettingState() = default;

        void updateButtons();
        void update(const float& dt);
        void render(sf::RenderTarget& target);
    private:
        std::unordered_map<std::string, Button> buttons;
        std::vector<std::string> carTypes;

        sf::Sprite player1view;
        sf::Sprite player2view;

        void initButtons();
        void initCarTypes();
};
