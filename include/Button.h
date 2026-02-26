#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

enum buttonStates {idle = 0, hover, pressed, change};

class Button
{
    public:
        Button(sf::RenderWindow* window, float x, float y, float width, float height, std::string message, sf::Texture& defaultTexture, sf::Texture& hoverTexture);
        virtual ~Button();

        bool isPressed();
        void changeState();
        void update(const sf::Vector2f mousePos);
        void render(sf::RenderTarget& target);

    protected:

    private:
        sf::RenderWindow* window;
        short unsigned state;

        sf::RectangleShape button;
        sf::Font font;
        sf::Text* text;
        sf::Texture& defaultTexture;
        sf::Texture& hoverTexture;

        bool isStatePressed;
};

#endif // BUTTON_H
