#pragma once

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
        Button(float x, float y, float width, float height,
            std::string message, sf::Texture& defaultTexture, 
            sf::Texture& hoverTexture, sf::Font& font);
        virtual ~Button() = default;

        bool isPressed();
        void changeState();
        void update(const sf::Vector2f mousePos);
        void render(sf::RenderTarget& target);

    private:
        short unsigned state;
        bool clicked;

        sf::RectangleShape button;
        sf::Text text;
        sf::Texture& defaultTexture;
        sf::Texture& hoverTexture;

        bool isStatePressed;
};

