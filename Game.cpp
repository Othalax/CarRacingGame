#include "Game.h"


Game::Game()
{
    this->initWindow();
    this->initKeybinds();
    this->initStates();
}

Game::~Game()
{
    //dtor
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

    window.reset();
    window = std::make_unique<sf::RenderWindow>(windowBounds, title);
    window->setFramerateLimit(framerate);
    window->setVerticalSyncEnabled(verticalSync);
    view = window->getDefaultView();
}

void Game::initKeybinds(){
    std::ifstream ifs("config/supportedKeys.ini");
    std::string keyName;
    int id;

    if (ifs.is_open()) {
        while (ifs >> keyName >> id) {
           supportedKeys[keyName] = static_cast<sf::Keyboard::Key>(id);
        }
    }
    ifs.close();
}

void Game::endApp(){
    std::cout<<"GameEndApp"<<std::endl;
}

void Game::initStates(){
    states.push_back(std::make_unique<MenuState>(this->window.get(), supportedKeys, &states));

}

void Game::updateDT(){
    this->dt=this->dtClock.restart().asSeconds();
}

void Game::updateEvents(){
    while (const std::optional event = this->window->pollEvent()) {
        if (event->is<sf::Event::Closed>())
            this->window->close();
        else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            view.setSize({static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
            window->setView(view);
        }
    }
}

void Game::update(){
    this->updateEvents();

    if(!this->states.empty()){
        this->states.back()->update(this->dt);
    }
    else{
        this->endApp();
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
