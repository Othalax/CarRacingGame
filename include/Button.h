#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <string>

enum class buttonStates : std::uint8_t { IDLE = 0, HOVER, PRESSED };

class Button {
  public:
    Button(float x, float y, float width, float height, const std::string& message,
           sf::Texture* defaultTexture, sf::Texture* hoverTexture, sf::Font& font);
    virtual ~Button() = default;
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) noexcept = default;
    Button& operator=(Button&&) = delete;

    bool is_pressed() const;
    void change_state();
    void update(sf::Vector2f mouse_pos);
    void render(sf::RenderTarget& target);

  private:
    buttonStates state;
    bool clicked;

    sf::RectangleShape button;
    sf::Text text;
    sf::Texture* defaultTexture;
    sf::Texture* hoverTexture;

    bool isStatePressed;
};

#endif // BUTTON_H