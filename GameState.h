#pragma once

#include "State.h"


class GameState : public State
{
    public:
        GameState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                    sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>& states);
        virtual ~GameState() = default;

        void update(const float& dt);
        void render(sf::RenderTarget& target);

    private:
        Car player1;
        Car player2;
};

