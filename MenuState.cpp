#include "MenuState.h"

MenuState::MenuState(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states)
    : State(window, supportedKeys, states)
{
    this->gamestate = new Button(window, 150.f, 150.f, 150.f, 50.f, "Start", textures["baseButton"], textures["baseButtonClicked"]);
    this->exit = new Button(window, 150.f, 250.f, 150.f, 50.f, "Exit", textures["baseButton"], textures["baseButtonClicked"]);
    this->settings = new Button(window, 150.f, 350.f, 150.f, 50.f, "Settings", textures["baseButton"], textures["baseButtonClicked"]);
    this->wiwiwi.setFillColor(sf::Color(100, 100, 100));
    this->wiwiwi.setPosition({100.0f, 100.0f});
    this->wiwiwi.setSize({200.0f, 200.0f});
}

MenuState::~MenuState()
{
    delete this->gamestate;
    delete this->exit;
}

void MenuState::updateButtons()
{
    if(this->exit->isPressed())
    {
        window->close();
    }

    if(this->gamestate->isPressed())
    {
        this->states->push_back(std::make_unique<GameState>(this->window, supportedKeys, states));
    }

    if(this->settings->isPressed())
    {
        this->states->push_back(std::make_unique<SettingState>(this->window, supportedKeys, states));
    }
}

void MenuState::update(const float& dt)
{
    updateMousePos();
    this->gamestate->update(this->mousePosView);
    this->exit->update(this->mousePosView);
    this->settings->update(this->mousePosView);
    updateButtons();
}

void MenuState::render(sf::RenderTarget& target)
{
    target.draw(this->wiwiwi);
    this->gamestate->render(target);
    this->exit->render(target);
    this->settings->render(target);
}
