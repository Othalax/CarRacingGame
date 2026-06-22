#include "State.h"

std::string State::player1car = "redCar";
std::string State::player2car = "blueCar";

State::State(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string, 
                sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>& states)
    : window(window), supportedKeys(supportedKeys), states(states), quit(false)
{
    if (textures.empty()) {
        this->initTextures();
    }

	initFont();
}

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

void State::initFont()
{
    if (!this->font.openFromFile("textures/fonts/NicoPaint-Monospaced.ttf")) {
        std::cout << "Error loading font\n";
    }
}

void State::updateMousePos() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void State::update(const float& dt) {

}

void State::render(sf::RenderTarget& target) {

}