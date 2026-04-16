#include "Car.h"

Car::Car(std::unordered_map<std::string, sf::Keyboard::Key> keys, sf::Texture& texture, float x, float y, float angle, float length, float max_steering, float max_acceleration)
    : keys(keys), position(x, y), velocity(0.f, 0.f), angle(angle), length(length),
    max_acceleration(max_acceleration), max_steering(max_steering), max_velocity(100.f),
    brake_deceleration(20.f), free_deceleration(40.f), acceleration(0.f), steering(0.f)
{
    this->car = new sf::Sprite(texture);
    this->car->setOrigin({ 100.f, 50.f });
    this->car->setPosition({ x, y });
    this->car->rotate(sf::degrees(90));
}

Car::~Car()
{
    delete this->car;
}

void Car::ride(const float& dt){
    velocity.x += acceleration * dt;
    velocity.x = std::clamp(velocity.x, -max_velocity, max_velocity);

    if (sf::Keyboard::isKeyPressed(keys["forward"])) {
        acceleration = std::min(acceleration + 50.0f * dt, max_acceleration);
    } else if (sf::Keyboard::isKeyPressed(keys["backward"])) {
        acceleration = std::max(acceleration - 50.0f * dt, -max_acceleration);
    } else {
        if (std::abs(velocity.x) > dt * free_deceleration)
            acceleration = -std::copysign(free_deceleration, velocity.x);
        else
            acceleration = -velocity.x / dt;
    }
}

void Car::veer(const float& dt){
    float angular_velocity = 0.0f;
    if (std::abs(steering) > 0.01f) {
        float turning_radius = length / std::tan(steering * PI / 180.0f);
        angular_velocity = velocity.x / turning_radius;
    }
    position.x += velocity.x * std::cos(angle * PI / 180.0f) * dt;
    position.y += velocity.x * std::sin(angle * PI / 180.0f) * dt;
    angle += angular_velocity * dt * (180.0f / PI);

     if (sf::Keyboard::isKeyPressed(keys["left"])) {
        steering = std::max(steering - 50 * dt, -max_steering);
    } else if (sf::Keyboard::isKeyPressed(keys["right"])) {
        steering = std::min(steering + 50 * dt, max_steering);
    } else {
        steering *= 0.8f;
    }
}

void Car::update(const float& dt) {

    ride(dt);
    veer(dt);

    this->car->setPosition(position);
    this->car->setRotation(sf::degrees(angle));
}

void Car::render(sf::RenderTarget& target){
    target.draw(*this->car);
}
