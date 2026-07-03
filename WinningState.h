#pragma once

#include "State.h"
#include "Button.h"

class WinningState :
    public State
{
public:
    WinningState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
        sf::Keyboard::Key> supportedKeys, char winner);
    virtual ~WinningState() = default;

    void updateButtons();
    void update(const float& dt);
    void render(sf::RenderTarget& target);

private:
    Button mainMenu;
    Button playAgain;

    sf::Sprite background;

	char winner;
    sf::Text winningText;
};

