#include "Button.h"

#include "Config.h"

Button::Button(float x, float y, float width, float height, const std::string& message,
               sf::Texture* defaultTexture, sf::Texture* hoverTexture, sf::Font& font)
    : defaultTexture(defaultTexture), hoverTexture(hoverTexture), isStatePressed(false),
      text(font, message, Config::instance().get_button_text_size()), state(buttonStates::IDLE),
      clicked(false) {
    this->button.setPosition(sf::Vector2f(x, y));
    this->button.setSize(sf::Vector2f(width, height));

    text.setFillColor(sf::Color::Black);
    text.setPosition(sf::Vector2f(
        this->button.getPosition().x + (width / 2.F) - text.getGlobalBounds().getCenter().x,
        this->button.getPosition().y + (height / 2.F) - text.getGlobalBounds().getCenter().y));

    this->button.setTexture(defaultTexture);
}

void Button::change_state() {
    this->isStatePressed = false;
}

bool Button::is_pressed() const {
    return this->clicked;
}

void Button::update(const sf::Vector2f mouse_pos) {
    this->state = buttonStates::IDLE;
    this->clicked = false;

    const bool is_hovering = this->button.getGlobalBounds().contains(mouse_pos);

    if (is_hovering) {
        this->state = buttonStates::HOVER;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            this->state = buttonStates::PRESSED;
            this->isStatePressed = true;
        } else if (this->isStatePressed) {
            this->clicked = true;
            this->isStatePressed = false;
        }
    } else {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            this->isStatePressed = false;
        }
    }

    if (this->state == buttonStates::HOVER || this->state == buttonStates::PRESSED) {
        this->button.setTexture(hoverTexture);
    } else {
        this->button.setTexture(defaultTexture);
    }
}

void Button::render(sf::RenderTarget& target) {
    target.draw(this->button);
    target.draw(text);
}