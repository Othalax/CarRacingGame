#include "State.h"

std::string State::player1car = "redCar";
std::string State::player2car = "blueCar";
std::vector<std::tuple<std::string, std::string, std::string, float>> State::maps = State::initMaps();
int State::currentMap = 0;

State::State(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string, 
                sf::Keyboard::Key> supportedKeys)
    : window(window), supportedKeys(supportedKeys), quit(false), nextState(nullptr)
{
    if (textures.empty()) {
        this->initTextures();
    }

	initFont();
}

const bool& State::getQuit() const {
    return this->quit;
}

std::unique_ptr<State> State::getNextState()
{
    return std::move(this->nextState);
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

std::vector<std::tuple<std::string, std::string, std::string, float>> State::initMaps()
{
    std::ifstream ifs("config/maps.ini");
    std::string mapName;
    std::string mapTexture;
    std::string mapConfig;
    float startingAngle;
    std::vector<std::tuple<std::string, std::string, std::string, float>> maps;

    if (ifs.is_open())
    {
        while (ifs >> mapName >> mapTexture >> mapConfig >> startingAngle)
        {
            maps.push_back(std::make_tuple(mapName, mapTexture, mapConfig, startingAngle));
        }
    }
    ifs.close();

    return maps;
}

void State::updateMousePos() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void State::update(const float& dt) {

}

void State::render(sf::RenderTarget& target) {

}