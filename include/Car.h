#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <string>
#include <vector>

class Car {
  public:
    Car(std::unordered_map<std::string, sf::Keyboard::Key> keys, sf::Texture& texture);
    virtual ~Car() = default;
    Car(const Car&) = delete;
    Car& operator=(const Car&) = delete;
    Car(Car&&) noexcept = default;
    Car& operator=(Car&&) = delete;

    void set_position(sf::Vector2f position, float angle);
    void handle_collision(Car& other);
    void handle_wall_collision(const std::vector<sf::Vector2f>& wallVertices);
    bool check_winning(const std::vector<sf::Vector2f>& finishLine);
    void update(const float& dt);
    void render(sf::RenderTarget& target);

  private:
    std::unique_ptr<sf::Sprite> car;

    sf::Vector2f position;
    float speed;
    float angle;
    float length;
    float max_acceleration;
    float max_steering;
    float max_velocity;
    float brake_deceleration;
    float free_deceleration;
    float acceleration;
    float steering;

    std::unordered_map<std::string, sf::Keyboard::Key> keys;

    void ride(const float& dt);
    void veer(const float& dt);
    static float dot_product(const sf::Vector2f& a, const sf::Vector2f& b);
    static sf::Vector2f normalize_vector(const sf::Vector2f& v);
};

#endif // CAR_H