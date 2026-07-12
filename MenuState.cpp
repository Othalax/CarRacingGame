#include "MenuState.h"
#include "SettingState.h"
#include "Config.h"

MenuState::MenuState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                        sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
        gamestate(
            Config::instance().getMenuButton("start").x,
            Config::instance().getMenuButton("start").y,
            Config::instance().getMenuButton("start").width,
            Config::instance().getMenuButton("start").height,
            Config::instance().getMenuButton("start").text,
            textures["baseButton"], textures["baseButtonClicked"], font),
        settings(
            Config::instance().getMenuButton("settings").x,
            Config::instance().getMenuButton("settings").y,
            Config::instance().getMenuButton("settings").width,
            Config::instance().getMenuButton("settings").height,
            Config::instance().getMenuButton("settings").text,
            textures["baseButton"], textures["baseButtonClicked"], font),
        exit(
            Config::instance().getMenuButton("exit").x,
            Config::instance().getMenuButton("exit").y,
            Config::instance().getMenuButton("exit").width,
            Config::instance().getMenuButton("exit").height,
            Config::instance().getMenuButton("exit").text,
            textures["baseButton"], textures["baseButtonClicked"], font),
	    background(textures[Config::instance().getMenuBackground()])
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
