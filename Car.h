#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stack>
#include <map>
#include <memory>
#include <string>
#define PI 3.14159265358979323846

class Car {
private:
    sf::Sprite* car;
    sf::Vector2f rotateVector(const sf::Vector2f& vec, float angle_deg) {
        float angle_rad = angle_deg * PI / 180.0f;
        float cos_a = std::cos(angle_rad);
        float sin_a = std::sin(angle_rad);
        return sf::Vector2f(vec.x * cos_a - vec.y * sin_a, vec.x * sin_a + vec.y * cos_a);
    }

    std::unordered_map<std::string,sf::Keyboard::Key> keys;

public:
    Car(std::unordered_map<std::string,sf::Keyboard::Key> keys, sf::Texture& texture, float x, float y, float angle, float length = 100.0f, float max_steering = 30.0f, float max_acceleration = 200.0f);
    virtual ~Car();

    sf::Vector2f position;
    sf::Vector2f velocity;
    float angle;
    float length;
    float max_acceleration;
    float max_steering;
    float max_velocity;
    float brake_deceleration;
    float free_deceleration;
    float acceleration;
    float steering;

    void ride(const float& dt);
    void veer(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget& target);
};

#endif // CAR_H
