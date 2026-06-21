#include "Button.h"

Button::Button(float x, float y, float width, float height, 
               std::string message, sf::Texture& defaultTexture, 
                sf::Texture& hoverTexture, sf::Font& font)
    : defaultTexture(defaultTexture),
    hoverTexture(hoverTexture), isStatePressed(false), 
	text(font, message, 20), state(0), clicked(false)
{
    this->button.setPosition(sf::Vector2f(x, y));
    this->button.setSize(sf::Vector2f(width, height));

    text.setFillColor(sf::Color::Black);
    text.setPosition(sf::Vector2f(
        this->button.getPosition().x + width / 2.f - text.getGlobalBounds().getCenter().x,
        this->button.getPosition().y + height / 2.f - text.getGlobalBounds().getCenter().y
    ));

    this->button.setTexture(&defaultTexture);

}

void Button::changeState()
{
    this->isStatePressed = false;
}

bool Button::isPressed() {
    return this->clicked;
}

void Button::update(const sf::Vector2f mousePos) {
    this->state = idle;
    this->clicked = false; 

    bool isHovering = this->button.getGlobalBounds().contains(mousePos);

    if (isHovering) {
        this->state = hover;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            this->state = pressed;
            this->isStatePressed = true;
        }
        else if (this->isStatePressed) {
            this->clicked = true;
            this->isStatePressed = false;
        }
    }
    else {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            this->isStatePressed = false;
        }
    }

    if (this->state == hover || this->state == pressed)
        this->button.setTexture(&hoverTexture);
    else
        this->button.setTexture(&defaultTexture);
}

void Button::render(sf::RenderTarget& target) {
    target.draw(this->button);
    target.draw(text);
}