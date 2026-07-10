#pragma once

#include "State.h"
#include "Button.h"

class SettingState
    : public State
{
    public:
        SettingState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                        sf::Keyboard::Key> supportedKeys);
        virtual ~SettingState() = default;

        void updateButtons();
        void update(const float& dt);
        void render(sf::RenderTarget& target);
    private:
		sf::Sprite background;

        std::unordered_map<std::string, Button> buttons;
        std::vector<std::string> carTypes;

        sf::Sprite player1view;
        sf::Sprite player2view;
        
		sf::Text mapText;
        sf::Text mapName;

        void initButtons();
        void initCarTypes();
};
