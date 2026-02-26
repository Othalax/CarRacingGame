#include "Button.h"

Button::Button(sf::RenderWindow* window, float x, float y, float width, float height, std::string message, sf::Texture& defaultTexture, sf::Texture& hoverTexture)
    : window(window),
      defaultTexture(defaultTexture),
      hoverTexture(hoverTexture),
      isStatePressed(false)
{
    if (!this->font.openFromFile("textures/fonts/NicoPaint-Monospaced.ttf")) {
        std::cout << "Error loading font\n";
    }

    this->button.setPosition(sf::Vector2f(x, y));
    this->button.setSize(sf::Vector2f(width, height));
    this->text = new sf::Text(this->font, message, 20);
    this->text->setFillColor(sf::Color::Black);

    this->text->setPosition( sf::Vector2f(
        this->button.getPosition().x + width / 2.f - this->text->getGlobalBounds().getCenter().x,
        this->button.getPosition().y + height / 2.f - this->text->getGlobalBounds().getCenter().y
    ));

    this->button.setTexture(&defaultTexture);

}

Button::~Button()
{

    delete this->text;
}

bool Button::isPressed()
{
    if(isStatePressed && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        return true;
    }
    return false;
}

void Button::changeState()
{
    this->isStatePressed = false;
}

void Button::update(const sf::Vector2f mousePos)
{
    if (this->button.getGlobalBounds().contains(mousePos))
    {
        this->state = hover;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            this->state = pressed;
            isStatePressed = true;
        }

    }
    else
    {
        this->state = idle;
    }

    switch(this->state)
    {
    case idle:
        this->button.setTexture(&defaultTexture);
        break;

    case hover:
        this->button.setTexture(&hoverTexture);
        break;

    default:
        break;
    }
}

void Button::render(sf::RenderTarget& target)
{
    target.draw(this->button);
    target.draw(*this->text);
}
