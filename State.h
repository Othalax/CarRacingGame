#pragma once


#include "Car.h"

class State {
public:
    State(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
            sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>& states);
    virtual ~State() = default;

	const bool& getQuit() const;
    void endState();
    void updateMousePos();
    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget& target);

protected:
    std::unique_ptr<sf::RenderWindow>& window;
    std::unordered_map<std::string, sf::Texture> textures;
	sf::Font font;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;
    std::vector<std::unique_ptr<State>>& states;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    static std::string player1car;
    static std::string player2car;

    bool quit;

    void initTextures();
    void initFont();
};
