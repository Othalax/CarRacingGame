#include "State.h"

std::string State::player1car = "pinkCar";
std::string State::player2car = "blueCar";

State::State(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states)
:states(states)
{
    this->window = window;
    this->supportedKeys = supportedKeys;
    initTextures();
}

State::~State()
{
    //dtor
}

void State::initTextures(){
    std::ifstream ifs("config/textures.ini");
    std::string textureName;
    std::string texturePath;

    if (ifs.is_open())
    {
        while (ifs >> textureName >> texturePath)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
                std::cerr << "Failed to load " << texturePath << std::endl;
            }
            textures[textureName] = std::move(texture);
        }
    }
    ifs.close();
}

void State::endState(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
        std::cout<<"StateEnd"<<std::endl;
    }
}

void State::updateMousePos(){
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void State::update(const float& dt){

}

void State::render(sf::RenderTarget& target){

}


