#ifndef STATE_H
#define STATE_H


#include <Car.h>

class State {
protected:
    sf::RenderWindow* window;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;
    std::vector<std::unique_ptr<State>>* states;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    static std::string player1car;
    static std::string player2car;

    void initTextures();

public:
    State(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states);
    virtual ~State();

    void ending();
    void endState();
    void updateMousePos();
    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget& target);

};

#endif // STATE_H
