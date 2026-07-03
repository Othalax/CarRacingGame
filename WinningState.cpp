#include "WinningState.h"
#include "MenuState.h"

WinningState::WinningState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
    sf::Keyboard::Key> supportedKeys, char winner)
    : State(window, supportedKeys),
    mainMenu(435.f, 150.f, 150.f, 50.f, "Main menu", textures["baseButton"], textures["baseButtonClicked"], font),
    playAgain(435.f, 250.f, 150.f, 50.f, "Play again", textures["baseButton"], textures["baseButtonClicked"], font),
	background(textures["winningBackground"]), winningText(font, "Player " + std::string(1, winner) + " wins!", 30),
	winner(winner)
{
    winningText.setFillColor(sf::Color::Black);
	winningText.setPosition(sf::Vector2f(340.f, 50.f));
}

void WinningState::updateButtons()
{
    if (this->mainMenu.isPressed())
    {
        this->nextState = std::make_unique<MenuState>(this->window, supportedKeys);
    }
    if (this->playAgain.isPressed())
    {
        this->nextState = std::make_unique<GameState>(this->window, supportedKeys);
    }
}

void WinningState::update(const float& dt)
{
    updateMousePos();
    this->mainMenu.update(this->mousePosView);
    this->playAgain.update(this->mousePosView);
    updateButtons();
}

void WinningState::render(sf::RenderTarget& target)
{
    this->window->draw(this->background, sf::RenderStates::Default);

	this->window->draw(this->winningText, sf::RenderStates::Default);
    this->mainMenu.render(target);
	this->playAgain.render(target);
}
