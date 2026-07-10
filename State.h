#pragma once


#include "Car.h"

class State {
public:
    State(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
            sf::Keyboard::Key> supportedKeys);
    virtual ~State() = default;

	const bool& getQuit() const;
    std::unique_ptr<State> getNextState();
    void endState();
    void updateMousePos();
    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget& target);

protected:
    std::unique_ptr<sf::RenderWindow>& window;
    std::unordered_map<std::string, sf::Texture> textures;
	sf::Font font;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    static std::string player1car;
    static std::string player2car;
    static std::vector<std::tuple<std::string, std::string, std::string, float>> maps;
    static int currentMap;

    bool quit;
    std::unique_ptr<State> nextState;

    void initTextures();
    void initFont();
    static std::vector<std::tuple<std::string, std::string, std::string, float>> initMaps();
};
