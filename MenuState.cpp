#include "MenuState.h"
#include "SettingState.h"

MenuState::MenuState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                        sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
        gamestate(245.f, 50.f, 150.f, 50.f, "Start", textures["baseButton"], textures["baseButtonClicked"], font),
        settings(245.f, 150.f, 150.f, 50.f, "Settings", textures["baseButton"], textures["baseButtonClicked"], font),
        exit(245.f, 250.f, 150.f, 50.f, "Exit", textures["baseButton"], textures["baseButtonClicked"], font),
	    background(textures["menuBackground"])
{
}

void MenuState::updateButtons()
{
    if(this->exit.isPressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        this->endState();
    }

    if(this->gamestate.isPressed())
    {
        this->nextState = std::make_unique<GameState>(this->window, supportedKeys);
    }

    if(this->settings.isPressed())
    {
        this->nextState = std::make_unique<SettingState>(this->window, supportedKeys);
    }
}

void MenuState::update(const float& dt)
{
    updateMousePos();
    this->gamestate.update(this->mousePosView);
    this->exit.update(this->mousePosView);
    this->settings.update(this->mousePosView);
    updateButtons();
}

void MenuState::render(sf::RenderTarget& target)
{
    this->window->draw(this->background, sf::RenderStates::Default);

    this->gamestate.render(target);
    this->exit.render(target);
    this->settings.render(target);
}
