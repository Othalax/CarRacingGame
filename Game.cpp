#include "Game.h"


Game::Game()
{
    this->initWindow();
    this->initKeybinds();
    this->initStates();
}

void Game::initWindow(){
    std::ifstream ifs("config/window.ini");
    std::string title = "x";
    sf::VideoMode windowBounds({800, 600});
    int framerate = 120;
    bool verticalSync = false;

    if(ifs.is_open()){
        ifs >> title;
        ifs >> windowBounds.size.x >> windowBounds.size.y;
        ifs >> framerate;
        ifs >> verticalSync;
    }
    ifs.close();

    this->window.reset();
    this->window = std::make_unique<sf::RenderWindow>(windowBounds, title);
    this->window->setFramerateLimit(framerate);
    this->window->setVerticalSyncEnabled(verticalSync);
    this->view = this->window->getDefaultView();
}

void Game::initKeybinds(){
    std::ifstream ifs("config/supportedKeys.ini");
    std::string keyName;
    int id;

    if (ifs.is_open()) {
        while (ifs >> keyName >> id) {
           this->supportedKeys[keyName] = static_cast<sf::Keyboard::Key>(id);
        }
    }
    ifs.close();
}

void Game::initStates() {
    this->states.push_back(std::make_unique<MenuState>(this->window, this->supportedKeys, this->states));
}

void Game::updateDT(){
    this->dt=this->dtClock.restart().asSeconds();
}

void Game::updateEvents(){
    while (const std::optional event = this->window->pollEvent()) {
        if (event->is<sf::Event::Closed>())
            this->window->close();
        else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            this->view.setSize({static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
            this->window->setView(this->view);
        }
    }
}

void Game::update() {
    this->updateEvents();

    if (!this->states.empty()) {
        this->states.back()->update(this->dt);

        if (this->states.back()->getQuit()) {
            this->states.pop_back(); 
        }
    }
    else {
        this->window->close();
    }
}

void Game::render(){
    this->window->clear();

    if(!this->states.empty()){
        this->states.back()->render(*this->window);
    }

    this->window->display();
}

void Game::run(){
    while (this->window->isOpen())
    {
        this->updateDT();
        this->update();
        this->render();
    }
}
