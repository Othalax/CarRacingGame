#include "State.h"
#include "Config.h"

std::string State::player1car = Config::instance().getDefaultPlayer1Car();
std::string State::player2car = Config::instance().getDefaultPlayer2Car();
std::vector<MapEntry> State::maps = Config::instance().getMaps();
int State::currentMap = Config::instance().getDefaultCurrentMap();

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
    const auto& texturePaths = Config::instance().getTexturePaths();

    for (const auto& [textureName, texturePath] : texturePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(texturePath)) {
            textures[textureName] = std::move(texture);
        }
    }
}

void State::initFont()
{
    if (!this->font.openFromFile(Config::instance().getFontPath())) {
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
