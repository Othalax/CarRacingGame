#include "State.h"

std::string State::player1car = "pinkCar";
std::string State::player2car = "blueCar";

State::State(sf::RenderWindow* window, std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states)
    : window(window), supportedKeys(supportedKeys), states(states), quit(false)
{
    if (textures.empty()) {
        this->initTextures();
    }
}

State::~State() {}

const bool& State::getQuit() const {
    return this->quit;
}

void State::endState() {
    this->quit = true;
}

void State::initTextures() {
    std::ifstream ifs("config/textures.ini");
    std::string textureName, texturePath;

    if (ifs.is_open()) {
        while (ifs >> textureName >> texturePath) {
            sf::Texture texture;
            if (texture.loadFromFile(texturePath)) {
                textures[textureName] = std::move(texture);
            }
        }
    }
    ifs.close();
}

void State::updateMousePos() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void State::update(const float& dt) {

}

void State::render(sf::RenderTarget& target) {

}