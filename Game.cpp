#include "Game.h"
#include "Config.h"


Game::Game()
{
    this->initWindow();
    this->initKeybinds();
    this->initStates();
}

void Game::initWindow(){
    const auto& config = Config::instance();

    this->window.reset();
    this->window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(config.getWindowSize()), config.getWindowTitle());
    this->window->setFramerateLimit(config.getFramerate());
    this->window->setVerticalSyncEnabled(config.getVerticalSync());
    this->view = this->window->getDefaultView();
}

void Game::initKeybinds(){
    this->supportedKeys = Config::instance().getSupportedKeys();
}

void Game::initStates() {
    this->currentState = std::make_unique<MenuState>(this->window, this->supportedKeys);
}

void Game::updateDT(){
    this->dt=this->dtClock.restart().asSeconds();
}

void Game::updateEvents() {
    while (const std::optional event = this->window->pollEvent()) {
        if (event->is<sf::Event::Closed>())
            this->window->close();
        else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            float targetRatio = this->view.getSize().x / this->view.getSize().y;
            float windowRatio = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);

            float sizeX = 1.f;
            float sizeY = 1.f;
            float posX = 0.f;
            float posY = 0.f;

            if (windowRatio > targetRatio) {
                sizeX = targetRatio / windowRatio;
                posX = (1.f - sizeX) / 2.f;
            }
            else {
                sizeY = windowRatio / targetRatio;
                posY = (1.f - sizeY) / 2.f;
            }

            this->view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
            this->window->setView(this->view);
        }
    }
}

void Game::update()
{
    if (this->currentState == nullptr)
    {
        this->window->close();
        return;
    }

    this->currentState->update(this->dt);

    if (this->currentState->getQuit())
    {
        this->currentState = nullptr;
        return;
    }

    auto next = this->currentState->getNextState();
    if (next)
    {
        this->currentState = std::move(next);
    }
}

void Game::render(){
    const auto logicalSize = Config::instance().getLogicalSize();
    sf::View view(sf::FloatRect({ 0.f, 0.f }, logicalSize));

    this->window->setView(view);
    this->window->clear();

    if(this->currentState != nullptr){
        this->currentState->render(*this->window);
    }

    this->window->display();
}

void Game::run(){
    while (this->window->isOpen())
    {
        this->updateDT();
        this->updateEvents();
        this->update();
        this->render();
    }
}
